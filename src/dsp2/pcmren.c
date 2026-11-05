/*
 * dsp2 "pcmren": the Dolby MS12 PCM renderer module (pcm_ren_* library wrapper)
 * that sits INSIDE "mixerbin" (admix.out0 -> pcmren.in0, pcmren.out0..3 -> mux).
 * It is not a renderer-block head: the kernel's renderer block on dsp2 is
 * "renderer" = srcbin -> sola -> lipsync -> gain (PCM) and "renderer_bypass" =
 * queue -> lipsync (ES) / rts -> srcbin -> lipsync (rtsPCM), see
 * src/kernel/aud/renderer.c and evidence/runtime/idle-state.txt.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp2.bin
 *   sha256 c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d
 *   main segment build/fw/audio_a0_dsp2.main_60000000.bin
 *   sha256 d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd
 * Class descriptor @0x60102d70 {name "pcmren", ctor 0x60097440, dtor 0x60097534,
 * chain 0x60096354, event 0x60097278, cmd 0x600968f8, prop_updated 0x60097598,
 * size 0x270, props @0x60102dd0: gapless-mode +0x214, drc-mode +0x224,
 * dmx-mode +0x228, metadata-type +0x260}.
 * Evidence: ../../decomp/audio_a0_dsp2.main_60000000.bin.c, tmp/a18/asm/pcmren_*.asm.
 *
 * ES (observed): pcmren_chain only distinguishes META (type 2) from everything
 * else; anything else goes into pcm_ren_main_process_run as PCM. No "es" key,
 * no rejection. Only dolby=true mixerbin inputs reach it (via admix).
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct aud_event { char _h[0x0c]; int type; };
struct aud_data  { char _h[0x10]; int type; char _p[0x20]; int size; void *buf; };
struct aud_port  { int num; struct pcmren *module; };
typedef struct { int _m[7]; } aud_mutex_t;

struct pcmren {
    char        core[0xf0];              /* struct aud_module */
    char        lib[0x14];               /* +0x0f0 pcm_ren_main handle */
    char        msf_in[0xf4];            /* +0x104 metadata connector */
    int         ui_init_done;            /* +0x160 (inside lib state) */
    char        _164[0x1f8 - 0x164];
    int         in_ch, out_ch;           /* +0x1f8, +0x1fc */
    int         drc_mode_set, dmx_mode_set; /* +0x200, +0x204 copied from +0x224/+0x228 */
    int         _208, _20c;
    int         metadata_type_set;       /* +0x210 */
    int         gapless_mode;            /* +0x214 "gapless-mode" */
    int         _218, _21c, _220;
    unsigned    drc_mode;                /* +0x224 "drc-mode" */
    unsigned    dmx_mode;                /* +0x228 "dmx-mode" */
    bool        skip_metadata;           /* +0x22c */
    bool        flush_metadata;          /* +0x22d */
    char        _22e[0x238 - 0x22e];
    char        audmd[8];                /* +0x238 */
    aud_mutex_t lock;                    /* +0x240 */
    bool        init_done;               /* +0x25c */
    int         metadata_type;           /* +0x260 "metadata-type" */
    int         md_in_type;              /* +0x264 */
    int         discard_drc;             /* +0x268 */
};

extern volatile unsigned *pcmren_log_mask;              /* 0x4ff900c8 */
extern void *pcmren_log_ctx;                            /* 0x60102d60 */
extern void logmm_printl(void *ctx, int lvl, const char *fn, int line, const char *fmt, ...);
#define PLOG(lvl, ...) \
    do { if (*pcmren_log_mask & (1u << (lvl))) \
        logmm_printl(pcmren_log_ctx, lvl, __func__, __LINE__, __VA_ARGS__); } while (0)

extern void __assert_failed(const char *fn, int line, const char *expr, ...);
extern void aud_mutex_init(void *), aud_mutex_lock(void *), aud_mutex_unlock(void *);
extern void aud_object_put(void *);
extern const char *aud_event_name(struct aud_event *);
extern int  aud_module_default_event(void *, struct aud_port *, struct aud_event *);
extern int  aud_module_get_prop(void *, const char *);
extern int  aud_module_set_prop(void *, const char *, unsigned);
extern char *audutil_strdup_printf(const char *, ...);
extern void aud_free(void *);
extern size_t strlen(const char *);
extern void logm_message(const char *, ...);
extern void audmd_init(void *, const char *name);

extern int  pcm_ren_main_process_open(void *), pcm_ren_main_process_close(void *);
extern void pcm_ren_main_connect_msf(void *);
extern int  pcm_ren_main_process_init(void *, void *, int);
extern int  pcm_ren_main_process_run(void *, void *buf, int size);
extern int  pcm_ren_main_process_setparam(void *, unsigned id, void *val);
extern void pcm_ren_set_metadataType(void *, ...);
extern void pcm_ren_set_discardDrc(void *, int);
extern int  pcm_ren_get_delay(void *);
extern void pcm_ren_get_ddp_metadata(void *, void *), pcm_ren_get_aac_metadata(void *, void *);
extern void pcm_ren_getPcmRuntimeParams(void *, void *);
extern void msf_flush_metadata(void *);
extern int  msf_process_metadata(void *, void *, int);

struct prop_desc { const char *n; int t; unsigned off; };
struct mixerbin_view { char m[0x11c]; void *pcmren; char _p[0x134 - 0x120]; void *nullgen; };

#define PCMR_PARAM_DRC_DMX   0x0          /* DAT_6006069c [GAP] value not read */
#define PCMR_PARAM_GAPLESS   0x0          /* DAT_600606a4 [GAP] */
#define PCMR_PARAM_CHANNELS  0x64010010u  /* DAT_60060794 */

/* @0x60096270 pcmren_init: (re)open the library with the stored DRC/DMX/gapless
 * settings ("initilize pcmrenderer"). Returns 0 when already initialized. */
int pcmren_init(struct pcmren *r)
{
    int drc_dmx[2], gapless;

    if (r->init_done)
        return 0;
    if (pcm_ren_main_process_open(r->lib) != 0)
        __assert_failed("pcmren_init", 0x71a, "result == 0");
    pcm_ren_main_connect_msf(r->lib);
    drc_dmx[0] = (int)r->drc_mode;
    drc_dmx[1] = (int)r->dmx_mode;
    r->drc_mode_set = drc_dmx[0];
    r->dmx_mode_set = drc_dmx[1];
    pcm_ren_main_process_setparam(r->lib, PCMR_PARAM_DRC_DMX, drc_dmx);
    if (pcm_ren_main_process_init(r->lib, NULL, 0) != 4) {
        if (r->ui_init_done)
            __assert_failed("pcmren_init", 0x727, "pcmren->dec.ui_init_done == 0");
        return -1;
    }
    gapless = r->gapless_mode;
    pcm_ren_main_process_setparam(r->lib, PCMR_PARAM_GAPLESS, &gapless);
    if (r->md_in_type) {
        pcm_ren_set_metadataType(r->lib);
        pcm_ren_set_discardDrc(r->lib, r->discard_drc);
        PLOG(3, "set pcmrenderer md_in_type.\n");
    }
    r->init_done = true;
    PLOG(3, "open pcmrenderer done.\n");
    return 0;
}

/* @0x60096354 pcmren_chain. Observed: lazy init, metadata path, the library
 * run and its error log. The per-output write-out loop ("pcmren_write_out":
 * aud_module_have_output -> aud_data_new -> copy -> audmd_serialize ->
 * aud_module_push_data(i), "write out%d %d bytes", "%d drop??") is entered via
 * FLIX bundles 0x600963d2..0x60096546 [FLIX]; its loop control is not decoded. */
int pcmren_chain(struct aud_port *port, struct aud_data *d)
{
    struct pcmren *r = port->module;
    int ret = 0;

    aud_mutex_lock(&r->lock);
    if (!r->init_done) {
        PLOG(3, "initilize pcmrenderer\n");
        if (pcmren_init(r) != 0) {
            PLOG(0, "error in initialize pcmrenderer...\n");
            ret = -1;
            goto out;
        }
    }
    if (r->flush_metadata) {
        msf_flush_metadata(r->msf_in);
        r->flush_metadata = false;
    }
    if (d->type == 2) {                                /* META */
        if (!r->skip_metadata)
            msf_process_metadata(r->msf_in, d->buf, d->size);
        aud_object_put(d);
        goto out;
    }
    /* everything else (PCM, and ES if it ever got here) is rendered as PCM */
    if (pcm_ren_main_process_run(r->lib, d->buf, d->size) != 0) {
        aud_object_put(d);
        PLOG(0, "error in pcm_ren_main_process_run\n");  /* [FLIX] @0x600963d2 */
        goto out;
    }
    aud_object_put(d);
    /* [FLIX] pcmren_write_out for outputs 0..3 */
out:
    aud_mutex_unlock(&r->lock);
    return ret;
}

/* @0x600966e8 PcmRenCmd_DebugStatus: dump library state with logm_message */
int PcmRenCmd_DebugStatus(struct pcmren *r)
{
    int md_type, dmx, rt[2][6], dmx_rt[6], drc[2];
    char ddp[0xdc], aac[0xd0];

    pcm_ren_main_process_setparam(r->lib, 0 /* get metadata type [GAP] */, &md_type);
    pcm_ren_main_process_setparam(r->lib, 0 /* get downmix mode [GAP] */, &dmx);
    pcm_ren_get_ddp_metadata(r->lib, ddp);
    pcm_ren_get_aac_metadata(r->lib, aac);
    logm_message("================================\n");
    logm_message("=== %s Info \n", *(const char **)r->core);
    logm_message("= Param : Input  Channel     : %d \n", r->in_ch);
    logm_message("= Param : Output Channel     : %d \n", r->out_ch);
    logm_message("= Param : DRC mode           : %d \n", r->drc_mode_set);
    logm_message("= drc mode, 0: line, 1: RF        \n");
    logm_message("= Param   : metadata type    : %d \n", r->metadata_type_set);
    logm_message("= Working : metadata type    : %d \n", md_type);
    logm_message("= Param   : Gapless mode     : %d \n", r->gapless_mode);
    logm_message("= Param : Downmix mode       : %d \n", r->dmx_mode_set);
    logm_message("= Working : Downmix mode     : %d \n", dmx);
    logm_message("= Internal Delay (samples)   : %d \n", pcm_ren_get_delay(r->lib));
    pcm_ren_getPcmRuntimeParams(r->lib, rt);
    for (int i = 0; i < 2; i++) {
        logm_message("=== multichannel_out[%d]=====================\n", i);
        logm_message("drc_cut[%d], drc_boost[%d], drc_mode[%d], dmx_mode[%d], dual_mode[%d], b_output_active[%d] \n",
                     rt[i][0], rt[i][1], rt[i][2], rt[i][3], rt[i][4], rt[i][5]);
    }
    (void)dmx_rt;
    (void)drc;
    return 0;
}

/* @0x600968f8 pcmren_cmd: PcmRenCmd_SetParam {in_ch, out_ch, ...}. Only the
 * NULL assert, the channel logs and the invalid-rate error are observed; the
 * table dispatch over the PcmRenCmd_* handlers is FLIX [FLIX]. */
int pcmren_cmd(struct pcmren *r, unsigned cmd, const int *param)
{
    (void)r;
    (void)cmd;
    if (!param)
        __assert_failed("pcmren_cmd", 0x8c4, "param != NULL", 1);
    PLOG(3, "input channel      : %d\n", param[0]);
    PLOG(3, "output channel     : %d\n", param[1]);
    if (param[0] && param[1] && (unsigned)param[2] < 2) {
        /* [FLIX] accepted: store and pcm_ren_main_process_setparam(PCMR_PARAM_CHANNELS) */
        return 0;
    }
    PLOG(0, "Invalid parameter : Input Sampling rate : %d\n", param[0]);
    return -1;
}

/* @0x60097278 pcmren_event: LAST closes the library (re-opened lazily by chain) */
int pcmren_event(struct pcmren *r, struct aud_port *port, struct aud_event *e)
{
    PLOG(2, "event %s\n", aud_event_name(e));
    if (e->type == 2) {
        aud_mutex_lock(&r->lock);
        if (r->init_done) {
            msf_flush_metadata(r->msf_in);
            pcm_ren_main_process_close(r->lib);
            r->init_done = false;
        }
        aud_mutex_unlock(&r->lock);
    }
    return aud_module_default_event(r, port, e);
}

/* @0x60097440 pcmren_constructor */
int pcmren_constructor(struct pcmren *r)
{
    int ch[4] = { 8, 8, 0, 0 };

    aud_mutex_init(&r->lock);
    r->_220 = 0;
    r->_20c = 0;
    if (pcm_ren_main_process_open(r->lib) != 0)
        __assert_failed("pcmren_constructor", 0, "result == 0");
    pcm_ren_main_connect_msf(r->lib);
    r->in_ch = 8;
    r->out_ch = 8;
    r->drc_mode_set = 0;
    r->dmx_mode_set = 0;
    pcm_ren_main_process_setparam(r->lib, PCMR_PARAM_CHANNELS, ch);
    r->gapless_mode = 1;
    r->skip_metadata = false;
    r->flush_metadata = false;
    if (pcm_ren_main_process_init(r->lib, NULL, 0) != 4) {
        if (r->ui_init_done)
            __assert_failed("pcmren_constructor", 0, "pcmren->dec.ui_init_done == 0");
        return -1;
    }
    if (!r->ui_init_done)
        __assert_failed("pcmren_constructor", 0, "pcmren->dec.ui_init_done != 0");
    audmd_init(r->audmd, "pcmren");
    r->init_done = true;
    r->md_in_type = 1;
    r->metadata_type = 0;
    r->discard_drc = 0;
    pcm_ren_set_metadataType(r->lib, 1);
    pcm_ren_set_discardDrc(r->lib, r->discard_drc);
    return 0;
}

/* @0x60097534 pcmren_destructor */
void pcmren_destructor(struct pcmren *r)
{
    PLOG(3, "destroy PCMREN\n");
    if (!r)
        __assert_failed("pcmren_destructor", 0, "module != NULL");
    msf_flush_metadata(r->msf_in);
    if (pcm_ren_main_process_close(r->lib) != 0) {
        r->init_done = false;
        __assert_failed("pcmren_destructor", 0, "result == 0");
    }
    r->init_done = false;
}

/* @0x60097598 pcmren_prop_updated: drc-mode/dmx-mode must be < 2 (else
 * "Invalid parameter : Downmix mode", -1); metadata-type handled in FLIX. [FLIX] */
int pcmren_prop_updated(struct pcmren *r, const struct prop_desc *prop)
{
    if (prop->off == 0x224 || prop->off == 0x228) {
        if (r->drc_mode < 2 && r->dmx_mode < 2)        /* [FLIX?] second test */
            return 0;                                  /* re-init happens lazily */
        PLOG(0, "Invalid parameter : Downmix mode : %d\n", r->dmx_mode);
        return -1;
    }
    if (prop->off != 0x260)
        return 0;
    /* [FLIX] metadata-type: md_in_type / discard_drc update */
    return 0;
}

/* @0x60098360 pcmren_get_prop: mixerbin -> pcmren child (+0x11c) */
char *pcmren_get_prop(struct mixerbin_view *mb, const char *const *prop)
{
    int v;

    if (!mb->pcmren || !prop[0])
        return NULL;
    v = aud_module_get_prop(mb->pcmren, prop[0]);
    PLOG(3, "%s get %s : %d\n", "mixerbin", prop[0], v);
    return audutil_strdup_printf("%d", v);
}

/* @0x600983b0 pcmren_set_prop: mixerbin "<name>=<val>" forwarded verbatim to the
 * pcmren child (drc-mode, dmx-mode, metadata-type, output-type, atmos-lock).
 * The child then does its own exact-name lookup; pcmren has no "output-type"
 * or "atmos-lock" property (table @0x60102dd0), so those are dropped silently
 * by aud_module_set_prop. */
int pcmren_set_prop(struct mixerbin_view *mb, const char *const *prop, const char *val)
{
    char *s;

    PLOG(3, "%s: set %s : %s\n", "mixerbin", prop[0], val);
    if (!mb->pcmren || !(s = audutil_strdup_printf("%s=%s", prop[0], val)))
        return 0;
    aud_module_set_prop(mb->pcmren, s, (unsigned)strlen(s));
    aud_free(s);
    return 0;
}

/* @0x6009840c dtsren_get_prop: "enc-mode" stub */
char *dtsren_get_prop(void)
{
    return NULL;
}

/* @0x60098414 dtsren_set_prop: "enc-mode" stub (ignored) */
int dtsren_set_prop(void)
{
    return 0;
}

/* @0x6009841c ullgen_get_prop: "nullgen-start" -> nullgen child (+0x134) */
char *ullgen_get_prop(struct mixerbin_view *mb, const char *const *prop)
{
    int v;

    if (!mb->nullgen || !prop[0])
        return NULL;
    v = aud_module_get_prop(mb->nullgen, prop[0]);
    PLOG(3, "%s get %s : %d\n", "mixerbin", prop[0], v);
    return audutil_strdup_printf("%d", v);
}

/* @0x6009846c ullgen_set_prop */
int ullgen_set_prop(struct mixerbin_view *mb, const char *const *prop, const char *val)
{
    char *s;

    PLOG(3, "%s: set %s : %s\n", "mixerbin", prop[0], val);
    if (!mb->nullgen || !(s = audutil_strdup_printf("%s=%s", prop[0], val)))
        return 0;
    aud_module_set_prop(mb->nullgen, s, (unsigned)strlen(s));
    aud_free(s);
    return 0;
}
