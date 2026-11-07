#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define info(...) ((void) 0)
#define error(...) ((void) 0)
#define debug(...) ((void) 0)
#define trace(...) ((void) 0)
#define warn(...) ((void) 0)
#define noti(...) ((void) 0)
#define debug_call() ((void) 0)
#define ALSA_SNDOUT_DEC_NUM 4
#define ALSA_SNDOUT_MIX_NUM 8
#define ALSA_SNDOUT_INPUT_NUM 12
#define ALSA_ADEC_MAX 2
typedef int common_input_ext_type_t;
enum { COMMON_NO_INPUT = 0, COMMON_INPUT_ADEC = 1, COMMON_INPUT_AMIXER = 0x10 };

struct list_head
{
    struct list_head *next, *prev;
};
#define INIT_LIST_HEAD(head) (*(head) = (struct list_head) { (head), (head) })
#define list_empty(head) ((head)->next == (head))
#define list_entry(node, type, member) ((type *) ((char *) (node) - offsetof(type, member)))
#define list_first_entry(head, type, member) list_entry((head)->next, type, member)
#define list_last_entry(head, type, member) list_entry((head)->prev, type, member)
#define list_for_each_entry(entry, head, member) \
    for (entry = list_first_entry(head, __typeof__(*entry), member); \
         &entry->member != (head); \
         entry = list_entry(entry->member.next, __typeof__(*entry), member))
#define list_for_each_entry_safe(entry, next_entry, head, member) \
    for (entry = list_first_entry(head, __typeof__(*entry), member), \
         next_entry = list_entry(entry->member.next, __typeof__(*entry), member); \
         &entry->member != (head); \
         entry = next_entry, \
         next_entry = list_entry(next_entry->member.next, __typeof__(*next_entry), member))
#define PIPE_NULL_CHECK(pipe, action) do { if (!(pipe)) { action; } } while (0)
#define GFP_KERNEL 0
static bool fail_container, fail_name;
static bool modules_alive[256];
static const char *module_names[256];
static unsigned int next_uid = 3;
static void *kzalloc (size_t size, int flags)
{
    return fail_container ? NULL : calloc (1, size);
}
static char *kvasprintf (int flags, const char *format, va_list arguments)
{
    char buffer[80];
    if (fail_name)
        return NULL;
    int length = vsnprintf (buffer, sizeof (buffer), format, arguments);
    assert (length >= 0 && (size_t) length < sizeof (buffer));
    return strdup (buffer);
}
#define kfree(pointer) free(pointer)
static void list_del (struct list_head *node)
{
    node->prev->next = node->next;
    node->next->prev = node->prev;
}
static void aimc_module_destroy (unsigned int uid)
{
    assert (uid > 0 && uid < 256 && modules_alive[uid]);
    modules_alive[uid] = false;
}
// @match src/kernel/aud/pipeline.c (?s)struct element\s*\{.*?\n\};
// @match src/kernel/aud/pipeline.c (?s)struct apipeline\s*\{.*?\n\};
// @extract src/kernel/aud/pipeline.c apipeline_new2 apipeline_del apipeline_head apipeline_tail apipeline_get_name
// @extract src/kernel/aud/alsa_sndout.c common_input_convert_index

static void list_add (struct list_head *node, struct list_head *head)
{
    *node = (struct list_head) { head->next, head };
    head->next->prev = node;
    head->next = node;
}

static unsigned int add_module (struct apipeline *pipe, const char *name, bool at_tail)
{
    struct element *element = calloc (1, sizeof (*element));
    assert (pipe && element && next_uid < 256);
    element->uid = next_uid++;
    modules_alive[element->uid] = true;
    module_names[element->uid] = name;
    if (at_tail)
    {
        element->list = (struct list_head) { &pipe->element_list, pipe->element_list.prev };
        pipe->element_list.prev->next = &element->list;
        pipe->element_list.prev = &element->list;
    }
    else
        list_add (&element->list, &pipe->element_list);
    return element->uid;
}

static unsigned int apipeline_appendf (struct apipeline *pipe, const char *name, ...)
{
    return add_module (pipe, name, true);
}

static unsigned int apipeline_prependf (struct apipeline *pipe, const char *name, ...)
{
    return add_module (pipe, name, false);
}

enum { PORT_OUT, PORT_IN, BYPASS_PORT = 0x100, RENDERER_DAFC_INDEX = 12 };
struct aud_block_info
{
    struct apipeline *pipeline;
    struct apipeline *forward[2];
    int index;
};
struct aud_renderer_info
{
    struct aud_block_info parent;
    struct apipeline *pipeline_bypass;
    int uid_rts, uid_rts_bypass, uid_src, uid_pcmcvt_bypass;
    int uid_sola, uid_lipsync, uid_lipsync_bypass, uid_gain, uid_queue;
    bool is_bypass_created, is_bypass_rts_PCM, is_queue_created, is_rts_created;
    struct list_head list_module_param;
};
// @match src/kernel/aud/renderer.c (?s)struct renderer_param\s*\{.*?\n\};
// @match src/kernel/aud/renderer.c (?s)struct renderer_module_param\s*\{.*?\n\};
static struct renderer_param param_lipsync[1], param_gain[1], param_sola[1];
static struct renderer_param param_rts[1], param_src[1], param_global[1], param_queue[1];
static int bm_get_preffered_processor (struct aud_block_info *block) { return 4; }
static int bm_get_index_by_block_info (struct aud_block_info *block) { return block->index; }
static int aud_hal_get_number_of_dsp (void) { return 4; }
static void bm_register_port_forward (struct aud_block_info *block, bool in, int port,
    struct apipeline *pipe, int pipeline_port)
{
    assert (port == BYPASS_PORT && pipeline_port == 0);
    block->forward[in] = pipe;
}
static void renderer_set_default (struct aud_block_info *block, int type) { }
#define NULL_CHECK(pointer, format, action) do { if (!(pointer)) { action; } } while (0)
// @extract src/kernel/aud/renderer.c renderer_registry_bypass renderer_module_registry renderer_module_update_uid renderer_clear_registry renderer_add_rts renderer_add_queue renderer_create_bypass_pipeline renderer_create_pipeline renderer_get_uid_pcm_bypass_head renderer_get_uid_bypass_tail renderer_get_uid_es_bypass_head

static void assert_chain (struct apipeline *pipe, const char *const *names, size_t count)
{
    struct element *element;
    size_t index = 0;
    list_for_each_entry (element, &pipe->element_list, list)
    {
        assert (index < count && modules_alive[element->uid]);
        assert (strcmp (module_names[element->uid], names[index++]) == 0);
    }
    assert (index == count);
}

static void test_renderer_construction (void)
{
    struct aud_renderer_info renderer = { .parent.index = 4 };
    renderer_create_pipeline (&renderer.parent);
    const char *const main_chain[] = { "srcbin", "sola", "lipsync", "gain" };
    assert_chain (renderer.parent.pipeline, main_chain, 4);
    assert (renderer_get_uid_bypass_tail (&renderer.parent) == -1);
    assert (!renderer.pipeline_bypass && !renderer.is_bypass_created);

    unsigned int es_head = renderer_get_uid_es_bypass_head (&renderer.parent);
    unsigned int es_tail = renderer_get_uid_bypass_tail (&renderer.parent);
    const char *const es_chain[] = { "queue", "lipsync" };
    assert_chain (renderer.pipeline_bypass, es_chain, 2);
    assert (renderer_get_uid_es_bypass_head (&renderer.parent) == es_head);
    assert (renderer.parent.forward[PORT_IN] == renderer.pipeline_bypass);
    assert (renderer.parent.forward[PORT_OUT] == renderer.pipeline_bypass);

    unsigned int pcm_head = renderer_get_uid_pcm_bypass_head (&renderer.parent);
    const char *const pcm_chain[] = { "rts", "srcbin", "lipsync" };
    assert_chain (renderer.pipeline_bypass, pcm_chain, 3);
    assert (!modules_alive[es_head] && !modules_alive[es_tail]);
    assert (modules_alive[pcm_head] && renderer.is_bypass_rts_PCM);
    assert (renderer_get_uid_pcm_bypass_head (&renderer.parent) == pcm_head);
    assert (renderer_get_uid_es_bypass_head (&renderer.parent) == pcm_head);
    assert (renderer.parent.forward[PORT_IN] == renderer.pipeline_bypass);
    assert (renderer.parent.forward[PORT_OUT] == renderer.pipeline_bypass);
    struct renderer_module_param *module;
    bool mirrored_lipsync = false;
    list_for_each_entry (module, &renderer.list_module_param, list)
    {
        assert (strcmp (module->name, "queue") != 0);
        if (strcmp (module->name, "lipsync") == 0)
            mirrored_lipsync = module->uid2 == renderer.uid_lipsync_bypass;
    }
    assert (mirrored_lipsync);
    apipeline_del (renderer.parent.pipeline);
    renderer.parent.pipeline = NULL;
    renderer_clear_registry (&renderer);
    renderer_create_pipeline (&renderer.parent);
    assert (!modules_alive[pcm_head]);
    assert (renderer.is_bypass_rts_PCM && renderer.is_bypass_created);
    assert_chain (renderer.parent.pipeline, main_chain, 4);
    assert_chain (renderer.pipeline_bypass, pcm_chain, 3);
    assert (renderer.parent.forward[PORT_IN] == renderer.pipeline_bypass);
    assert (renderer.parent.forward[PORT_OUT] == renderer.pipeline_bypass);
    apipeline_del (renderer.pipeline_bypass);
    apipeline_del (renderer.parent.pipeline);
    renderer_clear_registry (&renderer);

    renderer = (struct aud_renderer_info) { .parent.index = 11,
        .is_queue_created = true, .is_rts_created = true };
    renderer_create_pipeline (&renderer.parent);
    const char *const restored[] = { "rts", "queue", "srcbin", "sola", "lipsync", "gain" };
    assert_chain (renderer.parent.pipeline, restored, 6);
    apipeline_del (renderer.parent.pipeline);
    renderer_clear_registry (&renderer);

    renderer = (struct aud_renderer_info) { .parent.index = RENDERER_DAFC_INDEX };
    renderer_create_pipeline (&renderer.parent);
    const char *const dafc[] = { "srcbin" };
    assert_chain (renderer.parent.pipeline, dafc, 1);
    apipeline_del (renderer.parent.pipeline);
    renderer_clear_registry (&renderer);
}

typedef int common_output_ext_type_t;
enum { COMMON_NO_OUTPUT, COMMON_SPK, COMMON_OPTIC, COMMON_OPTIC_LG,
    COMMON_HP, COMMON_ARC, COMMON_WOWCAST };
enum { OUTPUT_SPK, OUTPUT_HP, OUTPUT_OPTIC, OUTPUT_ARC, OUTPUT_OTHERS = 5, OUTPUT_MAX };
enum { SNDOUT_CODEC_PCM, DECODER_OUTPUT_MODE_BYPASS = 1 };
struct snd_kcontrol { int unused; };
struct snd_ctl_elem_value { struct { struct { long value[3]; } integer; } value; };
struct alsa_sndout_list_output
{
    common_output_ext_type_t output;
    struct aud_block_info *output_info;
    struct aud_block_info *alsa_se;
    bool bypass_connected;
};
// @match src/kernel/aud/alsa_sndout.c (?s)struct alsa_sndout_list_connect\s*\{.*?\n\};
typedef struct
{
    struct aud_block_info *mixer_info;
    struct list_head list_connection;
    int main_audio_input, main_audio_port, bypass_master_index;
    struct { int mute, delay, gain; } param_input[ALSA_SNDOUT_INPUT_NUM];
    bool earc_on, wowcast_on;
    struct { int codec_type, mix_option, channel_num; } earc, wowcast;
} ALSA_SNDOUT_INFO_T;
static ALSA_SNDOUT_INFO_T alsa_sndout_info[1];
static struct alsa_sndout_list_output test_outputs[COMMON_WOWCAST + 1];
static bool sndout_locked, fail_reservation, slot_edge[12], decoder_bypass[12], multi_pcm[2];
static int bypass_slot, bypass_delay, input_mute[257], pcm_output_port[COMMON_WOWCAST + 1];
static void sndout_lock (void) { assert (!sndout_locked); sndout_locked = true; }
static void sndout_unlock (void) { assert (sndout_locked); sndout_locked = false; }
#define SNDOUT_LOCK() sndout_lock()
#define SNDOUT_UNLOCK() sndout_unlock()
static struct alsa_sndout_list_output *output_get (int output)
{
    return output > 0 && output <= COMMON_WOWCAST && test_outputs[output].output_info
        ? &test_outputs[output] : NULL;
}
static bool output_is_added (int output) { return output_get (output) != NULL; }
static int common_output_convert_output (int output)
{
    switch (output)
    {
        case COMMON_SPK: return OUTPUT_SPK;
        case COMMON_HP: return OUTPUT_HP;
        case COMMON_OPTIC: case COMMON_OPTIC_LG: return OUTPUT_OPTIC;
        case COMMON_ARC: return OUTPUT_ARC;
        default: return -1;
    }
}
static int bm_connect_block_reserved_out (const char *name, int slot, int port,
    struct aud_block_info *mixer, int mixer_port)
{
    assert (!strcmp (name, "renderer") && slot >= 0 && slot < 12);
    assert (mixer == alsa_sndout_info[0].mixer_info);
    if (fail_reservation)
        return -1;
    if (port == BYPASS_PORT)
    {
        assert (mixer_port == BYPASS_PORT);
        bypass_slot = slot;
    }
    else
    {
        assert (port == 0 && mixer_port == slot);
        slot_edge[slot] = true;
    }
    return 0;
}
static void mixer_set_mute (struct aud_block_info *mixer, int port, bool mute)
{
    input_mute[port] = mute;
}
static void mixer_set_delay (struct aud_block_info *mixer, int port, int delay)
{
    assert (port == BYPASS_PORT);
    bypass_delay = delay;
}
static void decoder_set_output_mode (int slot, int mode, bool enable)
{
    assert (!sndout_locked && mode == DECODER_OUTPUT_MODE_BYPASS);
    decoder_bypass[slot] = enable;
}
static void decoder_set_multi_pcm_out (int index, bool enable) { multi_pcm[index] = enable; }
static struct aud_block_info *alsa_se_get_block (struct aud_block_info *block) { return block; }
static int bm_connect_block (struct aud_block_info *source, int source_port,
    struct aud_block_info *target, int target_port)
{
    assert (target_port == 0);
    pcm_output_port[target->index] = source_port;
    return 0;
}
static void output_set_sound_bar_on (struct aud_block_info *block, bool enable) { }
static void notifier_output_changed (void) { }
static void _set_input_gain (int slot, int gain) { }
static void *kcalloc (size_t count, size_t size, int flags) { return calloc (count, size); }
static void list_add_tail (struct list_head *node, struct list_head *head)
{
    *node = (struct list_head) { head, head->prev };
    head->prev->next = node;
    head->prev = node;
}
// @extract src/kernel/aud/alsa_sndout.c _is_output_input_connected _input_connected_any _output_connected_any _add_connection _connect_pcm
static void _connect_output_all (void) { }
// @extract src/kernel/aud/alsa_sndout.c sndout_connect

static void test_sndout_routes (void)
{
    struct aud_block_info mixer = { 0 }, speaker = { .index = COMMON_SPK };
    struct aud_block_info hp = { .index = COMMON_HP }, arc = { .index = COMMON_ARC };
    ALSA_SNDOUT_INFO_T *sndout = alsa_sndout_info;
    sndout->mixer_info = &mixer;
    INIT_LIST_HEAD (&sndout->list_connection);
    test_outputs[COMMON_SPK] = (struct alsa_sndout_list_output) {
        .output = COMMON_SPK, .output_info = &speaker };
    test_outputs[COMMON_HP] = (struct alsa_sndout_list_output) {
        .output = COMMON_HP, .output_info = &hp };
    test_outputs[COMMON_ARC] = (struct alsa_sndout_list_output) {
        .output = COMMON_ARC, .output_info = &arc, .bypass_connected = true };
    struct snd_ctl_elem_value control = { .value.integer.value = {
        COMMON_SPK, COMMON_INPUT_AMIXER, 0 } };
    assert (sndout_connect (NULL, &control) == 0 && slot_edge[4]);
    assert (_is_output_input_connected (COMMON_SPK, COMMON_INPUT_AMIXER, 0));
    assert (sndout_connect (NULL, &control) == -EALREADY && !sndout_locked);

    slot_edge[4] = false;
    control.value.integer.value[0] = COMMON_HP;
    assert (sndout_connect (NULL, &control) == 0 && !slot_edge[4]);
    assert (_is_output_input_connected (COMMON_HP, COMMON_INPUT_AMIXER, 0));

    control.value.integer.value[0] = COMMON_SPK;
    control.value.integer.value[2] = 7;
    fail_reservation = true;
    assert (sndout_connect (NULL, &control) == 0 && !slot_edge[11]);
    assert (_is_output_input_connected (COMMON_SPK, COMMON_INPUT_AMIXER, 7));
    fail_reservation = false;
    control.value.integer.value[2] = 8;
    assert (sndout_connect (NULL, &control) == -EINVAL && !sndout_locked);
    control.value.integer.value[0] = COMMON_OPTIC;
    control.value.integer.value[2] = 1;
    assert (sndout_connect (NULL, &control) == -EACCES && !slot_edge[5]);

    sndout->main_audio_input = COMMON_INPUT_AMIXER;
    sndout->main_audio_port = 0;
    sndout->param_input[4].mute = 1;
    sndout->param_input[4].delay = 37;
    control.value.integer.value[0] = COMMON_ARC;
    control.value.integer.value[2] = 0;
    assert (sndout_connect (NULL, &control) == 0);
    assert (bypass_slot == 4 && sndout->bypass_master_index == 4 && decoder_bypass[4]);
    assert (bypass_delay == 37 && input_mute[BYPASS_PORT] && !sndout_locked);

    sndout->main_audio_input = COMMON_INPUT_ADEC;
    sndout->main_audio_port = 0;
    sndout->earc_on = true;
    sndout->earc.mix_option = 1;
    sndout->earc.channel_num = 8;
    _connect_pcm (&test_outputs[COMMON_ARC]);
    assert (pcm_output_port[COMMON_ARC] == 2 && multi_pcm[0]);
    sndout->earc.channel_num = 2;
    _connect_pcm (&test_outputs[COMMON_ARC]);
    assert (pcm_output_port[COMMON_ARC] == 0 && multi_pcm[0]);
    sndout->earc.mix_option = 0;
    _connect_pcm (&test_outputs[COMMON_ARC]);
    assert (pcm_output_port[COMMON_ARC] == 0 && !multi_pcm[0]);

    struct alsa_sndout_list_connect *connection, *next;
    list_for_each_entry_safe (connection, next, &sndout->list_connection, list)
    {
        list_del (&connection->list);
        free (connection);
    }
}

static void test_input_slots (void)
{
    for (int port = 0; port < 4; port++)
        assert (common_input_convert_index (COMMON_INPUT_ADEC, port) == port);
    for (int port = 0; port < 8; port++)
        assert (common_input_convert_index (COMMON_INPUT_AMIXER, port) == port + 4);

    const int invalid_ports[] = { INT_MIN, -1, 8, INT_MAX };
    for (unsigned int index = 0; index < sizeof (invalid_ports) / sizeof (*invalid_ports); index++)
    {
        assert (common_input_convert_index (COMMON_INPUT_ADEC, invalid_ports[index]) == -1);
        assert (common_input_convert_index (COMMON_INPUT_AMIXER, invalid_ports[index]) == -1);
    }
    assert (common_input_convert_index (COMMON_INPUT_ADEC, 4) == -1);
    assert (common_input_convert_index (COMMON_NO_INPUT, 0) == -1);
    assert (common_input_convert_index (COMMON_INPUT_AMIXER | COMMON_INPUT_ADEC, 0) == -1);
}

static void test_passive_endpoints (void)
{
    struct apipeline pipe = { .name = "existing", .preferred_proc = 4 };
    struct element first = { .uid = 0x04001001 }, last = { .uid = 0x04001002 };
    INIT_LIST_HEAD (&pipe.element_list);

    assert (apipeline_head (NULL) == 0 && apipeline_tail (NULL) == 0);
    assert (apipeline_head (&pipe) == 0 && apipeline_tail (&pipe) == 0);
    assert (list_empty (&pipe.element_list));

    pipe.element_list.next = &first.list;
    pipe.element_list.prev = &last.list;
    first.list = (struct list_head) { &last.list, &pipe.element_list };
    last.list = (struct list_head) { &pipe.element_list, &first.list };
    assert (apipeline_head (&pipe) == first.uid && apipeline_tail (&pipe) == last.uid);
    assert (pipe.element_list.next == &first.list && pipe.element_list.prev == &last.list);
    assert (first.list.next == &last.list && last.list.prev == &first.list);
    assert (pipe.preferred_proc == 4);
}

static void test_pipeline_lifetime (void)
{
    fail_container = true;
    assert (apipeline_new2 (4, "renderer") == NULL);
    fail_container = false;
    fail_name = true;
    struct apipeline *pipe = apipeline_new2 (4, "renderer");
    assert (pipe && apipeline_get_name (pipe) == NULL && list_empty (&pipe->element_list));
    apipeline_del (pipe);
    fail_name = false;

    pipe = apipeline_new2 (4, "mixer_input%d", 11);
    assert (pipe && pipe->preferred_proc == 4);
    assert (strcmp (apipeline_get_name (pipe), "mixer_input11") == 0);
    assert (apipeline_get_name (NULL) == NULL);
    assert (apipeline_head (pipe) == 0 && apipeline_tail (pipe) == 0);
    struct element *first = calloc (1, sizeof (*first));
    struct element *last = calloc (1, sizeof (*last));
    assert (first && last);
    first->uid = 1;
    last->uid = 2;
    first->list = (struct list_head) { &last->list, &pipe->element_list };
    last->list = (struct list_head) { &pipe->element_list, &first->list };
    pipe->element_list.next = &first->list;
    pipe->element_list.prev = &last->list;
    modules_alive[1] = modules_alive[2] = true;
    assert (apipeline_head (pipe) == 1 && apipeline_tail (pipe) == 2);
    apipeline_del (pipe);
    assert (!modules_alive[1] && !modules_alive[2]);
    apipeline_del (NULL);
}

int main (void)
{
    test_input_slots ();
    test_passive_endpoints ();
    test_pipeline_lifetime ();
    test_renderer_construction ();
    test_sndout_routes ();
    puts ("PASS: slot/route errors, membership vs graph, eARC mixing, pipeline ownership and ES/PCM reconstruction");
    return 0;
}