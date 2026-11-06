// SPDX-License-Identifier: GPL-2.0-only
/*
	SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
	Copyright(c) 2020 by LG Electronics Inc.

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	version 2 as published by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.
*/

/*
 * Modified to match the shipped kernel Image (sha256
 * 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db,
 * orig/boot/Image-5.4.268-329.decompressed).
 *
 * Base: lg-opensource/kdriver/core/aud/mixer.c (GPL-2.0), complete. All
 * functions compared with the shipped disassembly; log __LINE__ values are
 * identical to the published file. mixer_get_input_port,
 * mixer_set_encoder_connection, mixer_set_atmos_locking_mode and
 * mixer_set_sp50 have no symbol (inlined, or compiled out with MS12_VER < 26).
 * aud_mixer_info layout in the shipped image: list_module +0x18,
 * list_pipeline_input +0x28, list_prop +0x38, pipeline_bypass +0x48;
 * aud_mixer_list_pipeline_input: uid_fader +0x10, uid_delay +0x14,
 * uid_gain +0x18, port +0x1c, pipeline_input +0x20 (kzalloc 0x28).
 * webOS25-shipped: MS12_VER < 26 in this build (see mixer_resume).
 */


/** @defgroup Mixer
  * @{
  *
  * @brief
  * Mixer module for audio kernel driver
  * @detail
  *
  *
  * @}
  */

/** @file
  * @ingroup Mixer
  *
  * implementation for Mixer
  */

//#include <linux/spinlock.h>
#include <linux/slab.h>
#include "module/adec_module_list.h"    ///< ADEC_CORE_ARM

#include "log.h"
#include "kaimc.h"
#include "block.h"
#include "block_manager.h"
#include "mixer.h"

logm_define (aud_mixer, log_level_noti);
#define trace(fmt,args...)	logm_trace(aud_mixer,fmt,##args)
#define debug(fmt,args...)	logm_debug(aud_mixer,fmt,##args)
#define info(fmt,args...)	logm_info(aud_mixer,fmt,##args)
#define noti(fmt,args...)	logm_noti(aud_mixer,fmt,##args)
#define error(fmt,args...)	logm_error(aud_mixer,fmt,##args)

static DEFINE_MUTEX (_mixer_lock);

#define AUD_MIX_LOCK()		mutex_lock(&_mixer_lock)
#define AUD_MIX_UNLOCK()	mutex_unlock(&_mixer_lock)

#define DEFAULT_VOLUME		0x800000
#define DEFAULT_MUTE		0

struct aud_mixer_list_module
{
    struct list_head list;

    const char *name;
    unsigned int uid;
};


struct aud_mixer_list_pipeline_input
{
    struct list_head list;
    int uid_fader;
    int uid_delay;
    int uid_gain;

    int port;
    struct apipeline *pipeline_input;
};

struct aud_mixer_property_info
{
    struct list_head list;

    int port;
    int module_type;

    char *name;
    int data;
};

/**
 * AUDIO Mixer Information.
 */
struct aud_mixer_info
{
    struct aud_block_info parent;

    struct list_head list_module;       ///< see @aud_mixer_list_module
    struct list_head list_pipeline_input;       ///< see @aud_mixer_list_pipeline_input
    struct list_head list_prop;

    struct apipeline *pipeline_bypass;

    // ms mixer
    int main_audio_index;
    enum msmixer_mixing_mode mode;
    bool limiter_enable;

    // pcm renderer
    int drc_mode;
    int dmx_mode;
    int metadata_type;
#if (MS12_VER >= 26)
    int output_type;
    bool atmos_lock_mode;
    bool set_sp50;              ///< for mat encoder.
#endif
};


static struct aud_mixer_list_pipeline_input *mixer_get_input_port (struct aud_block_info *block,
    int port)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;
    struct aud_mixer_list_pipeline_input *list_pipeline;

    list_for_each_entry (list_pipeline, &mixer->list_pipeline_input, list)
    {
        if (list_pipeline->port == port)
            return list_pipeline;
    }

    return NULL;
}

/* shipped symbol: mixer_set_property_list.isra.0 */
/* @0xffffffc010cae244 */
static int mixer_set_property_list (struct aud_block_info *block, int port,
    enum mixer_module_type type, char *name, int data)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;
    struct aud_mixer_property_info *prop = NULL;

    list_for_each_entry (prop, &mixer->list_prop, list)
    {
        if ((prop->port == port) && (!strcmp (name, prop->name)))
        {
            prop->data = data;

            info ("update property, pipeline (%d), %s = %d\n", port, name, data);

            return 0;
        }
    }

    prop =
        (struct aud_mixer_property_info *) kzalloc (sizeof (struct aud_mixer_property_info),
        GFP_KERNEL);
    if (!prop)
    {
        error ("no mem\n");
        return -1;
    }

    prop->port = port;
    prop->module_type = type;
    prop->name = kstrdup (name, GFP_KERNEL);
    prop->data = data;

    list_add (&prop->list, &mixer->list_prop);

    debug ("mixer property list add, pipeline (%d), %s = %d\n", port, name, data);

    return 0;
}


/* @0xffffffc010cae840 */
int mixer_set_delay (struct aud_block_info *block, int port, int delay)
{
    struct aud_mixer_list_pipeline_input *list_pipeline;
    int ret = -1;

    list_pipeline = mixer_get_input_port (block, port);
    if (list_pipeline == NULL)
        goto exit;

    ret = aimc_set_property (list_pipeline->uid_delay, "delay=%d", delay);
  exit:
    mixer_set_property_list (block, port, MIXER_MODULE_TYPE_DELAY, "delay", delay);

    return ret;
}

/* @0xffffffc010cae8e0 */
int mixer_set_volume (struct aud_block_info *block, int port, unsigned int volume)
{
    struct aud_mixer_list_pipeline_input *list_pipeline;
    int ret = -1;

    list_pipeline = mixer_get_input_port (block, port);
    if (list_pipeline == NULL)
        goto exit;

    ret = aimc_set_property (list_pipeline->uid_gain, "gain=%d", volume);
  exit:
    mixer_set_property_list (block, port, MIXER_MODULE_TYPE_GAIN, "gain", volume);

    return ret;
}

/* @0xffffffc010cae980 */
int mixer_get_volume (struct aud_block_info *block, int port, unsigned int *volume)
{
    struct aud_mixer_list_pipeline_input *list_pipeline;
    int ret = -1;

    list_pipeline = mixer_get_input_port (block, port);
    if (list_pipeline == NULL)
    {
        noti ("pipeline of port %d is NULL\n", port);
        *volume = 0;
        goto exit;
    }

    ret = aimc_get_property (list_pipeline->uid_gain, "final-gain", volume);

    noti ("port %d volume is %d\n", port, *volume);
  exit:
    return ret;
}

/* @0xffffffc010caea94 */
int mixer_set_ease_volume (struct aud_block_info *block, int port, unsigned int volume,
    unsigned int duration, enum MIXER_EASE_TYPE_T type)
{
    struct aud_mixer_list_pipeline_input *list_pipeline;
    int ret = -1;

    list_pipeline = mixer_get_input_port (block, port);
    if (list_pipeline == NULL)
        goto exit;

    ret =
        aimc_set_property (list_pipeline->uid_gain, "target-gain=%d\\fade-type=%d\\fadetime-ms=%d",
        volume, type, duration);
  exit:
    mixer_set_property_list (block, port, MIXER_MODULE_TYPE_GAIN, "target-gain", volume);
    mixer_set_property_list (block, port, MIXER_MODULE_TYPE_GAIN, "fade-type", type);
    mixer_set_property_list (block, port, MIXER_MODULE_TYPE_GAIN, "fadetime-ms", duration);

    return ret;
}

/* @0xffffffc010caeb80 */
int mixer_set_mute (struct aud_block_info *block, int port, bool mute)
{
    struct aud_mixer_list_pipeline_input *list_pipeline;
    int ret = -1;

    list_pipeline = mixer_get_input_port (block, port);
    if (list_pipeline == NULL)
        goto exit;

    ret = aimc_set_property (list_pipeline->uid_gain, "mute-input=%d\\mute-output=%d", mute, mute);

  exit:
    mixer_set_property_list (block, port, MIXER_MODULE_TYPE_GAIN, "mute-input", mute);
    mixer_set_property_list (block, port, MIXER_MODULE_TYPE_GAIN, "mute-output", mute);

    return ret;
}

/* @0xffffffc010caec40 */
int mixer_get_main_audio_index (struct aud_block_info *block)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;

    return mixer->main_audio_index;
}

/* @0xffffffc010caec50 */
int mixer_set_main_audio_index (struct aud_block_info *block, int main_audio_index)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;

    if (mixer == NULL)
    {
        error ("mixer_info is NULL!!");
        return -1;
    }

    if (mixer->main_audio_index != main_audio_index)
    {
        struct aud_mixer_list_module *mixerbin = NULL;

        mixerbin = list_first_entry (&mixer->list_module, struct aud_mixer_list_module, list);
        if (!mixerbin)
        {
            error ("cannot find mixerbin\n");
            return -1;
        }

        aimc_set_property (mixerbin->uid, "main-audio-port=%d", main_audio_index);
        info ("main audio index changed %d -> %d", mixer->main_audio_index, main_audio_index);

        mixer_set_property_list (block, 0, MIXER_MODULE_TYPE_MIXER, "main-audio-port",
            main_audio_index);
    }

    mixer->main_audio_index = main_audio_index;

    return 0;
}

const char *msmixer_mixing_mode_name[] = {
    "stringent (main+sub)", "lenient (sysmix only)", "aggregate (main+main)"
};

/* @0xffffffc010caedc0 */
int mixer_set_mixing_mode (struct aud_block_info *block, enum msmixer_mixing_mode mode)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;

    if (mixer->mode != mode)
    {
        struct aud_mixer_list_module *mixerbin = NULL;

        mixerbin = list_first_entry (&mixer->list_module, struct aud_mixer_list_module, list);
        if (!mixerbin)
        {
            error ("cannot find mixerbin\n");
            return -1;
        }

        aimc_set_property (mixerbin->uid, "mixing-mode-int=%d", mode);
        info ("msmixer mixing mode changed %d (%s) -> %d (%s)", mixer->mode,
            msmixer_mixing_mode_name[mixer->mode], mode, msmixer_mixing_mode_name[mode]);

        mixer_set_property_list (block, 0, MIXER_MODULE_TYPE_MIXER, "mixing-mode-int", mode);
    }

    mixer->mode = mode;

    return 0;
}

/* @0xffffffc010caeef0 */
int mixer_set_limiter (struct aud_block_info *block, bool limiter_enable)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;

    if (mixer->limiter_enable != limiter_enable)
    {
        struct aud_mixer_list_module *mixerbin = NULL;

        mixerbin = list_first_entry (&mixer->list_module, struct aud_mixer_list_module, list);
        if (!mixerbin)
        {
            error ("cannot find mixerbin\n");
            return -1;
        }

        aimc_set_property (mixerbin->uid, "enable-limiter=%d", limiter_enable);
        info ("msmixer limiter statue changed %d -> %d", mixer->limiter_enable, limiter_enable);

        mixer_set_property_list (block, 0, MIXER_MODULE_TYPE_MIXER, "enable-limiter",
            limiter_enable);
    }

    mixer->limiter_enable = limiter_enable;

    return 0;
}

/* @0xffffffc010caf004 */
int mixer_set_pcmren_metadata_type (struct aud_block_info *block, unsigned int metadata_type)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;

    if (mixer->metadata_type != metadata_type)
    {
        struct aud_mixer_list_module *mixerbin = NULL;

        mixerbin = list_first_entry (&mixer->list_module, struct aud_mixer_list_module, list);
        if (!mixerbin)
        {
            error ("cannot find mixerbin\n");
            return -1;
        }

        aimc_set_property (mixerbin->uid, "metadata-type=%d", metadata_type);
        noti ("pcm renderer metadata type changed %d -> %d", mixer->metadata_type, metadata_type);

        mixer_set_property_list (block, 0, MIXER_MODULE_TYPE_PCMREN, "metadata-type",
            metadata_type);
    }

    mixer->metadata_type = metadata_type;

    return 0;
}

const char *pcmren_drc_mode_name[] = {
    "LINE", "RF"
};

/* @0xffffffc010caf120 */
int mixer_set_pcmren_drc_mode (struct aud_block_info *block, unsigned int drc_mode)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;

    if (mixer->drc_mode != drc_mode)
    {
        struct aud_mixer_list_module *mixerbin = NULL;

        mixerbin = list_first_entry (&mixer->list_module, struct aud_mixer_list_module, list);
        if (!mixerbin)
        {
            error ("cannot find mixerbin\n");
            return -1;
        }

        aimc_set_property (mixerbin->uid, "drc-mode=%d", drc_mode);
        info ("pcm renderer drc mode changed %d (%s) -> %d (%s)", mixer->drc_mode,
            pcmren_drc_mode_name[mixer->drc_mode], drc_mode, pcmren_drc_mode_name[drc_mode]);

        mixer_set_property_list (block, 0, MIXER_MODULE_TYPE_PCMREN, "drc-mode", drc_mode);
    }

    mixer->drc_mode = drc_mode;

    return 0;
}

const char *pcmren_downmix_mode_name[] = {
    "LTRT", "LORO"
};

/* @0xffffffc010caf254 */
int mixer_set_pcmren_downmix_mode (struct aud_block_info *block, unsigned int downmix_mode)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;

    if (mixer->dmx_mode != downmix_mode)
    {
        struct aud_mixer_list_module *mixerbin = NULL;

        mixerbin = list_first_entry (&mixer->list_module, struct aud_mixer_list_module, list);
        if (!mixerbin)
        {
            error ("cannot find mixerbin\n");
            return -1;
        }

        aimc_set_property (mixerbin->uid, "dmx-mode=%d", downmix_mode);
        info ("pcm renderer downmix mode changed %d (%s) -> %d (%s)", mixer->dmx_mode,
            pcmren_downmix_mode_name[mixer->dmx_mode], downmix_mode,
            pcmren_downmix_mode_name[downmix_mode]);

        mixer_set_property_list (block, 0, MIXER_MODULE_TYPE_PCMREN, "dmx-mode", downmix_mode);
    }

    mixer->dmx_mode = downmix_mode;

    return 0;
}

#if (MS12_VER >= 26)
int mixer_set_encoder_connection (struct aud_block_info *block, bool connected, bool is_mat_enc)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;
    struct aud_mixer_list_module *mixerbin = NULL;

    int output_type = 0;        ///< 0 : SPK, 1 : wisa, 2 : ddp_enc, 3 : mat_enc

    if (!connected)
        output_type = 0;
    else if (is_mat_enc)
        output_type = 3;
    else
        output_type = 2;

    noti ("connected %d, ddpenc %d output type is %d\n", connected, is_mat_enc, output_type);

    if (mixer->output_type != output_type)
    {
        mixerbin = list_first_entry (&mixer->list_module, struct aud_mixer_list_module, list);
        if (!mixerbin)
        {
            error ("cannot find mixerbin\n");
            return -1;
        }

        aimc_set_property (mixerbin->uid, "output-type=%d", output_type);
        mixer_set_property_list (block, 0, MIXER_MODULE_TYPE_PCMREN, "output-type", output_type);
    }

    mixer->output_type = output_type;

    return 0;
}

int mixer_set_atmos_locking_mode (struct aud_block_info *block, bool atmos_lock_mode)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;
    struct aud_mixer_list_module *mixerbin = NULL;

    noti ("atmos_lock_mode : %d\n", atmos_lock_mode);

    if (mixer->atmos_lock_mode != atmos_lock_mode)
    {
        mixerbin = list_first_entry (&mixer->list_module, struct aud_mixer_list_module, list);
        if (!mixerbin)
        {
            error ("cannot find mixerbin\n");
            return -1;
        }

        noti ("set atmos_lock_mode %d to fw\n", atmos_lock_mode);
        aimc_set_property (mixerbin->uid, "atmos-lock=%d", atmos_lock_mode);
        mixer_set_property_list (block, 0, MIXER_MODULE_TYPE_PCMREN, "atmos-lock", atmos_lock_mode);
    }

    mixer->atmos_lock_mode = atmos_lock_mode;

    return 0;
}

// set_sp50 true(1)          : AVR/Sound Bar support only MAT profile 1 or 2.
// set_sp50 false(0) default : AVR/Sound Bar MAT profile 3 & 4.
int mixer_set_sp50 (struct aud_block_info *block, bool set_sp50)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;
    struct aud_mixer_list_module *mixerbin = NULL;

    noti ("set_sp50 is %s.\n", set_sp50 ? "true" : "false");

    if (mixer->set_sp50 != set_sp50)
    {
        mixerbin = list_first_entry (&mixer->list_module, struct aud_mixer_list_module, list);
        if (!mixerbin)
        {
            error ("cannot find mixerbin\n");
            return -1;
        }

        aimc_set_property (mixerbin->uid, "disable-multi-oa=%d", set_sp50);
        mixer_set_property_list (block, 0, MIXER_MODULE_TYPE_PCMREN, "disable-multi-oa", set_sp50);
    }

    mixer->set_sp50 = set_sp50;

    return 0;
}
#endif

/* @0xffffffc010cad7a0 */
static int mixer_get_module_uid (struct aud_mixer_info *mixer, int port,
    enum mixer_module_type type)
{
    struct aud_mixer_list_module *mixerbin = NULL;
    struct aud_mixer_list_pipeline_input *pipeline = NULL;
    struct aud_mixer_list_pipeline_input *list_pipeline = NULL;
    int uid = -1;

    mixerbin = list_first_entry (&mixer->list_module, struct aud_mixer_list_module, list);
    if (!mixerbin)
    {
        error ("cannot find mixerbin\n");
        return -1;
    }

    list_for_each_entry (list_pipeline, &mixer->list_pipeline_input, list)
    {
        if (list_pipeline->port == port)
        {
            pipeline = list_pipeline;
            break;
        }
    }

    if (!pipeline)
    {
        error ("cannot find list_pipeline, port: %d\n", port);
        return -1;
    }

    switch (type)
    {
        case MIXER_MODULE_TYPE_DELAY:
            uid = pipeline->uid_delay;
            break;
        case MIXER_MODULE_TYPE_FADER:
            uid = pipeline->uid_fader;
            break;
        case MIXER_MODULE_TYPE_GAIN:
            uid = pipeline->uid_gain;
            break;
        case MIXER_MODULE_TYPE_MIXER:
        case MIXER_MODULE_TYPE_PCMREN:
            uid = mixerbin->uid;
            break;
        default:
            error ("unknown module type, %d\n", type);
            break;
    }

    return uid;
}

/*
 * Shipped symbol mixer_set_bypass_pipeline.isra.0; logic identical to the
 * published source (strings "mixer_bypass", "bypass", "mixer_byin", "delay",
 * "gain", gain=0x800000, mute 0/0, port 0x100).
 *
 * ES bypass, mixer part (all on DSP2: mixer impl preffered_processor = 4):
 *   renderer "renderer_bypass" tail (lipsync) --BYPASS_PORT-->
 *   "mixer_byin": delay -> gain(gain=0x800000, mute-input=0, mute-output=0)
 *   --> "mixer_bypass": bypass  --BYPASS_PORT--> output block (dsp1).
 * No fader in this path (mixer_set_bypass_fader / AREN_SET_BYPASS_FADER act
 * on uid_fader, which is 0 for the BYPASS_PORT entry, i.e. on port <index>
 * PCM inputs only). Created once in mixer_create, not per connection.
 * The bypass forward is independent of mixerbin's output ports 0/1/2.
 * Selecting it for ARC does not disconnect mixerbin -> HP/SPK/OPTIC edges;
 * that policy lives in alsa_sndout.c. This establishes topology, not what
 * DSP2's mixer algorithm places on PCM outputs (separately reconstructed).
 */
/* @0xffffffc010cadc20 */
static int mixer_set_bypass_pipeline (struct aud_block_info *block,
    struct aud_mixer_list_pipeline_input *lp)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;
    struct aud_mixer_list_pipeline_input *list_pipeline = lp;
    int preferred_proc = bm_get_preffered_processor (block);
    struct aud_mixer_property_info *prop;

    info ("add bypass pipeline");

    mixer->pipeline_bypass = apipeline_new2 (preferred_proc, "mixer_bypass");
    apipeline_addf (mixer->pipeline_bypass, "bypass");

    if (list_pipeline == NULL)
    {
        // alloc input pipeline
        list_pipeline =
            (struct aud_mixer_list_pipeline_input *) kzalloc (sizeof (struct
                aud_mixer_list_pipeline_input), GFP_KERNEL);
        if (!list_pipeline)
        {
            error ("no mem\n");
            return -1;
        }

        list_add (&list_pipeline->list, &mixer->list_pipeline_input);
    }

    list_pipeline->port = BYPASS_PORT;
    list_pipeline->pipeline_input = apipeline_new2 (preferred_proc, "mixer_byin");

    // add modules into input pipeline
    list_pipeline->uid_delay = apipeline_appendf (list_pipeline->pipeline_input, "delay");
    list_pipeline->uid_gain = apipeline_appendf (list_pipeline->pipeline_input, "gain");

    // set gain default property
    aimc_set_property (list_pipeline->uid_gain, "gain=%d", DEFAULT_VOLUME);
    aimc_set_property (list_pipeline->uid_gain, "mute-input=%d\\mute-output=%d", DEFAULT_MUTE,
        DEFAULT_MUTE);

    // set properties
    list_for_each_entry (prop, &mixer->list_prop, list)
    {
        if (prop)
        {
            if (prop->port == BYPASS_PORT)
            {
                int uid = -1;
                uid = mixer_get_module_uid (mixer, BYPASS_PORT, prop->module_type);

                if (uid >= 0)
                {
                    info ("set property, %s = %d\n", prop->name, prop->data);
                    aimc_set_property (uid, "%s=%d", prop->name, prop->data);
                }
            }
        }
    }

    // connect input pipeline to mixer
    apipeline_connect (list_pipeline->pipeline_input, 0, mixer->pipeline_bypass, 0);

    bm_register_port_forward (block, PORT_IN, BYPASS_PORT, list_pipeline->pipeline_input, 0);
    bm_register_port_forward (block, PORT_OUT, BYPASS_PORT, mixer->pipeline_bypass, 0);

    info ("done");
    return 0;
}

/* @0xffffffc010cad940 */
static int mixer_add_input_pipeline (struct aud_block_info *block,
    struct aud_mixer_list_pipeline_input *lp, int port)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;
    struct aud_mixer_list_pipeline_input *list_pipeline = lp;
    int preferred_proc = bm_get_preffered_processor (block);
    struct aud_mixer_property_info *prop;

    info ("add input port %d", port);

    if (list_pipeline == NULL)
    {
        // alloc input pipeline
        list_pipeline =
            (struct aud_mixer_list_pipeline_input *) kzalloc (sizeof (struct
                aud_mixer_list_pipeline_input), GFP_KERNEL);
        if (!list_pipeline)
        {
            error ("no mem\n");
            return -1;
        }

        list_add (&list_pipeline->list, &mixer->list_pipeline_input);
    }

    list_pipeline->port = port;
    list_pipeline->pipeline_input = apipeline_new2 (preferred_proc, "mixer_input%d", port);

    // add modules into input pipeline
    list_pipeline->uid_fader = apipeline_appendf (list_pipeline->pipeline_input, "fader");
    list_pipeline->uid_delay = apipeline_appendf (list_pipeline->pipeline_input, "delay");
    list_pipeline->uid_gain = apipeline_appendf (list_pipeline->pipeline_input, "gain");

    // set gain default property
    aimc_set_property (list_pipeline->uid_gain, "gain=%d", DEFAULT_VOLUME);
    aimc_set_property (list_pipeline->uid_gain, "mute-input=%d\\mute-output=%d", DEFAULT_MUTE,
        DEFAULT_MUTE);

    // set properties
    list_for_each_entry (prop, &mixer->list_prop, list)
    {
        if (prop)
        {
            int uid = -1;
            uid = mixer_get_module_uid (mixer, port, prop->module_type);

            if ((uid >= 0) && (prop->port == port))
            {
                info ("set property, %s = %d\n", prop->name, prop->data);
                aimc_set_property (uid, "%s=%d", prop->name, prop->data);
            }
        }
    }

    // connect input pipeline to mixer
    apipeline_connect (list_pipeline->pipeline_input, 0, block->pipeline, port);

    bm_register_port_forward (block, PORT_IN, port, list_pipeline->pipeline_input, 0);

    info ("done");
    return 0;
}

/* @0xffffffc010caf390 */
int mixer_set_bypass_fader (struct aud_block_info *block, int port, bool onOff)
{
    struct aud_mixer_list_pipeline_input *list_pipeline;
    int ret = -1;

    list_pipeline = mixer_get_input_port (block, port);
    if (list_pipeline == NULL)
        goto exit;

    ret = aimc_set_property (list_pipeline->uid_fader, "bypass=%d", onOff);

  exit:
    mixer_set_property_list (block, port, MIXER_MODULE_TYPE_FADER, "fader", onOff);

    return ret;
}

/*
 * Only pre_connect is implemented for the mixer (impl: post_connect = NULL,
 * pre/post_disconnect = NULL). For port BYPASS_PORT the input pipeline
 * already exists (created by mixer_create), so mixer_get_input_port() hits,
 * "port %d is already added, not add" is logged and -1 is returned; the
 * return value is ignored by _bm_connect_block_locked.
 */
/* @0xffffffc010cae6f4 */
int mixer_preconnect (struct aud_block_info *block, int port, bool in)
{
    int ret = -1;

    if (in == PORT_IN)
    {
        if (mixer_get_input_port (block, port))
        {
            info ("port %d is already added, not add", port);
            return -1;
        }

        // add input port pipeline before connection
        ret = mixer_add_input_pipeline (block, NULL, port);
        if (ret)
        {
            error ("error in add input port%d pipeline!!", port);
            return ret;
        }
    }

    return 0;
}

/* shipped symbol: mixer_add_module.constprop.0.isra.0 */
/* @0xffffffc010cad430 */
static int mixer_add_module (struct aud_mixer_info *mixer, const char *name)
{
    struct aud_block_info *block = &mixer->parent;
    struct aud_mixer_list_module *list_module;

    info ("add %s module", name);

    list_module =
        (struct aud_mixer_list_module *) kzalloc (sizeof (struct aud_mixer_list_module),
        GFP_KERNEL);
    if (!list_module)
    {
        error ("no mem\n");
        return -1;
    }

    list_module->name = name;
    list_module->uid = apipeline_appendf (block->pipeline, name);
    list_add (&list_module->list, &mixer->list_module);

    return 0;
}

/* @0xffffffc010cae120 */
int mixer_create (struct aud_block_info *block)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;
    int preferred_proc = bm_get_preffered_processor (block);

    noti ("create mixer");

    INIT_LIST_HEAD (&mixer->list_module);
    INIT_LIST_HEAD (&mixer->list_pipeline_input);
    INIT_LIST_HEAD (&mixer->list_prop);

    block->pipeline = apipeline_new2 (preferred_proc, "mixer");
    mixer_add_module (mixer, "mixerbin");

    mixer_set_bypass_pipeline (block, NULL);

    block->isOpened = true;

    noti ("done");
    return 0;
}


/**
 * destroy mixer block
 *
 * @param block [in] context of this block
 * @return void
 * @see aud_mixer_info
*/
/* @0xffffffc010cae430 */
static void mixer_destroy (struct aud_block_info *block)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;
    struct aud_mixer_list_pipeline_input *list_pipeline, *next;
    struct aud_mixer_property_info *prop, *prop_next;

    noti ("called");

    AUD_MIX_LOCK ();

    if (block == NULL)
    {
        error ("mixer_info is NULL!!");
        goto exit;
    }

    if (block->isOpened == false)
    {
        error ("mixer is already closed. \n");
        goto exit;
    }

    // destroy pipeline (inside modules are destroyed automatically)
    if (block->pipeline)
        apipeline_del (block->pipeline);

    list_for_each_entry_safe (list_pipeline, next, &mixer->list_pipeline_input, list)
    {
        debug ("delete input pipeline %d", list_pipeline->port);
        apipeline_del (list_pipeline->pipeline_input);
    }

    if (mixer->pipeline_bypass)
        apipeline_del (mixer->pipeline_bypass);

    list_for_each_entry_safe (prop, prop_next, &mixer->list_prop, list)
    {
        debug ("remove property list, %s=%d\n", prop->name, prop->data);
        list_del (&prop->list);
        kfree (prop->name);
        kfree (prop);
    }

    // Clear a mixer resource.
    block->isOpened = false;

  exit:
    AUD_MIX_UNLOCK ();
    noti ("done\n");
}

/* @0xffffffc010cad534 */
static int mixer_suspend (struct aud_block_info *block)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;
    struct aud_mixer_list_module *list_module, *module_next;
    struct aud_mixer_list_pipeline_input *list_pipeline, *pipeline_next;

    noti ("called\n");

    if (block->pipeline)
        apipeline_del (block->pipeline);

    list_for_each_entry_safe (list_module, module_next, &mixer->list_module, list)
    {
        info ("delete module %s\n", list_module->name);
        list_del (&list_module->list);
        kfree (list_module);
    }

    list_for_each_entry_safe (list_pipeline, pipeline_next, &mixer->list_pipeline_input, list)
    {
        info ("delete input pipeline %d\n", list_pipeline->port);

        if (list_pipeline->port == BYPASS_PORT)
        {
            bm_unregister_port_forward (block, PORT_IN, BYPASS_PORT);
            bm_unregister_port_forward (block, PORT_OUT, BYPASS_PORT);
        }
        else
        {
            bm_unregister_port_forward (block, PORT_IN, list_pipeline->port);
        }

        apipeline_del (list_pipeline->pipeline_input);
        list_pipeline->pipeline_input = NULL;
    }

    if (mixer->pipeline_bypass)
        apipeline_del (mixer->pipeline_bypass);

    block->pipeline = NULL;
    mixer->pipeline_bypass = NULL;

    noti ("done\n");

    return 0;
}

/*
 * webOS25-shipped: no "output-type", "atmos-lock", "disable-multi-oa"
 * replays: the build has MS12_VER < 26 (mixer_set_output_type /
 * mixer_set_atmos_locking_mode / mixer_set_sp50 are absent from kallsyms).
 */
/* @0xffffffc010cadef0 */
static int mixer_resume (struct aud_block_info *block)
{
    struct aud_mixer_info *mixer = (struct aud_mixer_info *) block;
    struct aud_mixer_list_module *mixerbin = NULL;
    struct aud_mixer_list_pipeline_input *list_pipeline = NULL;
    int preferred_proc = bm_get_preffered_processor (block);

    noti ("called\n");

    block->pipeline = apipeline_new2 (preferred_proc, "mixer");
    mixer_add_module (mixer, "mixerbin");

    mixerbin = list_first_entry (&mixer->list_module, struct aud_mixer_list_module, list);
    if (!mixerbin)
    {
        error ("cannot find mixerbin\n");
        return -1;
    }
    aimc_set_property (mixerbin->uid, "main-audio-port=%d", mixer->main_audio_index);
#if (MS12_VER >= 26)
    aimc_set_property (mixerbin->uid, "output-type=%d", mixer->output_type);
    aimc_set_property (mixerbin->uid, "atmos-lock=%d", mixer->atmos_lock_mode);
    aimc_set_property (mixerbin->uid, "disable-multi-oa=%d", mixer->set_sp50);
#endif
    list_for_each_entry (list_pipeline, &mixer->list_pipeline_input, list)
    {
        if (list_pipeline->port == BYPASS_PORT)
        {
            mixer_set_bypass_pipeline (block, list_pipeline);
            info ("create bypass pipeline %d\n", list_pipeline->port);
        }
        else
        {
            mixer_add_input_pipeline (block, list_pipeline, list_pipeline->port);
            info ("create input pipeline %d\n", list_pipeline->port);
        }
    }

    noti ("done\n");

    return 0;
}

static struct audio_block_impl block_impl = {
    .block_info_size = sizeof (struct aud_mixer_info),
    .block_type = "mixer",
    .preffered_processor = ADEC_CORE_DSP2,
    .constructor = mixer_create,
    .destructor = mixer_destroy,
    .pre_connect = mixer_preconnect,
    .suspend = mixer_suspend,
    .resume = mixer_resume,
};

/* @0xffffffc010caf430 */
void mixer_initializer (void)
{
    bm_block_registry (&block_impl);
}
