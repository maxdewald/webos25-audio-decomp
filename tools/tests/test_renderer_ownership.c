/* Actual-source renderer ownership host harness. */
#include <assert.h>
#include <stdbool.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define info(...) ((void)0)
#define noti(...) ((void)0)
#define error(...) ((void)0)
#define debug(...) ((void)0)
#define TRUE true
#define FALSE false
#define GFP_KERNEL 0
#define RET_OK 0
#define MD_CLK_TYPE_UNKNOWN 0
#define PORT_IN 1
#define PORT_OUT 0
#define BYPASS_PORT 0x100
#define SINT32 int
struct mutex { int held; };
static void mutex_lock(struct mutex *m) { assert(!m->held); m->held = 1; }
static void mutex_unlock(struct mutex *m) { assert(m->held); m->held = 0; }
static struct mutex driver_lock;
#define DRIVER_LOCK() mutex_lock(&driver_lock)
#define DRIVER_UNLOCK() mutex_unlock(&driver_lock)
struct apipeline { int head, tail, count; };
struct aud_block_info { struct apipeline *pipeline; };
struct list_head { struct list_head *next, *prev; };
struct renderer_param { const char *name, *alt_name;
    int (*set_func)(struct aud_block_info *, unsigned);
    int (*get_func)(struct aud_block_info *, unsigned); };
struct renderer_module_param { struct list_head list; const char *name;
    struct renderer_param *param; int uid, uid2; };
#define list_for_each_entry(p, head, member) \
    for (p = (void *)(head)->next; &p->member != (head); p = (void *)p->member.next)
#define trace(...) ((void)0)
struct aud_renderer_info {
    struct aud_block_info parent;
    struct list_head list_module_param;
    struct apipeline *pipeline_bypass;
    bool sync;
    unsigned clock_type;
    int uid_lipsync, uid_gain, uid_sola, uid_rts_bypass, uid_pcmcvt_bypass, uid_lipsync_bypass;
    bool is_bypass_created, is_bypass_rts_PCM;
    int upperbound, upperbound_bypass;
};
typedef struct { unsigned bits_per_sample, channels, sampling_rate, byte_align, sign,
    clock_x4, pcm_bypass, pcm_multi_ch; } aren_pcm_info_t;
typedef struct { struct aud_block_info *ren_info; struct mutex lock; bool is_open;
    int clock_type; aren_pcm_info_t pcm_info; bool vsp_mode; int proc_id; } ARENDERER_INFO_T;
static ARENDERER_INFO_T arenderer_info[10];
struct audio_renderer_info { void *list[2]; unsigned index; };
struct file { void *private_data; };
struct inode { int unused; };
static int fail_alloc, fail_block, blocks, disconnects, destroys, pipeline_deletes, next_uid;
static int property_count, property_target[128], property_value[128];
static char property_name[128][32], appended[8][16];
static int append_count, registry_count, forward_count;
static const char *registry_name[8];
static void *kcalloc(size_t n, size_t s, int flags) { return fail_alloc ? NULL : calloc(n, s); }
static void kfree(void *p) { free(p); }
static int bm_get_preffered_processor(struct aud_block_info *b) { return 4; }
static struct apipeline *apipeline_new2(int proc, const char *name)
{ assert(proc == 4); assert(!strcmp(name, "renderer_bypass")); return calloc(1, sizeof(struct apipeline)); }
static int apipeline_appendf(struct apipeline *p, const char *name, ...)
{ assert(append_count < 8); strcpy(appended[append_count++], name); p->tail = ++next_uid;
  if (!p->count++) p->head = p->tail; return p->tail; }
static int apipeline_head(struct apipeline *p) { return p->head; }
static int apipeline_tail(struct apipeline *p) { return p->tail; }
static void apipeline_del(struct apipeline *p) { pipeline_deletes++; free(p); }
static int renderer_registry_bypass(struct aud_renderer_info *r, char *name, int uid)
{ assert(registry_count < 8); registry_name[registry_count++] = name; return 0; }
static int renderer_set_property(struct aud_renderer_info *r, unsigned uid, char *prop, unsigned val)
{ assert(property_count < 128); property_target[property_count] = uid;
  property_value[property_count] = val; strcpy(property_name[property_count++], prop); return 0; }
static int _renderer_set_property(struct aud_renderer_info *r, unsigned uid,
                                  const char *name, unsigned val, const char *str)
{ assert(!strcmp(str, "yes")); return renderer_set_property(r, uid, (char *)name, val); }
static int aimc_get_property(unsigned uid, const char *name, int *val)
{ *val = 123; return renderer_set_property(NULL, uid, (char *)name, 123); }
static void bm_register_port_forward(struct aud_block_info *b, int dir, int port,
                                    struct apipeline *p, int child_port)
{ assert(port == 0x100 && child_port == 0); forward_count++; }
static struct aud_block_info *bm_create_block(const char *name, int index)
{ assert(!strcmp(name, "renderer")); assert(arenderer_info[index].lock.held);
  if (fail_block) return NULL; blocks++; return calloc(1, sizeof(struct aud_renderer_info)); }
static void bm_disconnect_block(struct aud_block_info *b, int port)
{ assert(!driver_lock.held); disconnects++; }
static void bm_destroy_block(struct aud_block_info *b)
{ struct aud_renderer_info *r = (void *)b; assert(!driver_lock.held); destroys++;
  if (r) { if (r->pipeline_bypass) apipeline_del(r->pipeline_bypass); blocks--; free(r); } }
static int bm_get_block_head(struct aud_block_info *b) { return b ? 777 : 0; }
static int bm_get_block_tail(struct aud_block_info *b) { return b ? 778 : 0; }
// @match src/kernel/aud/aapi_media.c (?m)^#define AREN_MAX \d+
// @match src/kernel/aud/aapi_media.c (?s)#define ARENDERER_CHECK_INDEX_AND_OPENED_AND_LOCK.*?(?=struct str_table)
// @match src/kernel/aud/renderer.c (?ms)struct renderer_param param_(?:src|gain)\[\] = \{.*?^};
// @extract src/kernel/aud/renderer.c renderer_run_param renderer_set_default renderer_create_bypass_pipeline renderer_get_uid_pcm_bypass_head renderer_get_uid_es_bypass_head renderer_get_uid_bypass_tail renderer_set_pcm_bypass_rts_out_channels
// @extract src/kernel/aud/aapi_media.c aren_open aren_close aren_get_uid aren_get_tail_uid aren_set_proc_id
// @extract src/kernel/aud/driver.c driver_aren_open driver_aren_close
/* The driver ioctl case for SET_INDEX, run on its own. */
#define AREN_IOW_SET_INDEX 1
static int select_index(struct file *filp, int index)
{
    struct audio_renderer_info *info = filp->private_data;
    struct { int index; } args = {index};
    int ret = -1;
    switch (AREN_IOW_SET_INDEX) {
// @match src/kernel/aud/driver.c (?s)case AREN_IOW_SET_INDEX:.*?(?=\n\s*case AREN_IO_ADD_QUEUE:)
    }
    return ret;
}
static void unlocked(void)
{ for (int i = 0; i < AREN_MAX; i++) assert(!arenderer_info[i].lock.held); assert(!driver_lock.held); }
static void fd_open(struct file *f) { assert(driver_aren_open(NULL, f) == 0); }
static void fd_close(struct file *f) { assert(driver_aren_close(NULL, f) == 0); unlocked(); }
static void reset_trace(void) { property_count = append_count = registry_count = forward_count = 0; }
static int prop(int uid, const char *name, int val)
{
    for (int i = 0; i < property_count; i++) {
        if (property_target[i] == uid && !strcmp(property_name[i], name) &&
            property_value[i] == val) return 1;
    }
    return 0;
}
int main(void)
{
    struct file owner = {0}, observer = {0}; unsigned uid;
    fail_alloc = 1; assert(driver_aren_open(NULL, &owner) == -ENOMEM); fail_alloc = 0;
    assert(aren_open(-1) == -1 && aren_open(10) == -1 && aren_close(-1) == -1);
    assert(aren_set_proc_id(-1, 81) == -1 && aren_set_proc_id(AREN_MAX, 81) == -1);
    assert(aren_set_proc_id(9, 81) == -1); unlocked();
    assert(aren_get_uid(10, &uid, true) == -1); unlocked();
    fd_open(&owner); assert(select_index(&owner, 9) == 0);
    assert(arenderer_info[9].proc_id == -1);
    assert(aren_set_proc_id(9, 81) == 0 && arenderer_info[9].proc_id == 81);
    fd_open(&observer); assert(select_index(&observer, 9) == -1);
    assert(((struct audio_renderer_info *)observer.private_data)->index == 9);
    assert(arenderer_info[9].proc_id == 81);
    assert(aren_set_proc_id(9, 82) == 0 && arenderer_info[9].proc_id == 82); unlocked();
    assert(aren_get_tail_uid(9, &uid, true) == 0 && uid == UINT_MAX);
    assert(!((struct aud_renderer_info *)arenderer_info[9].ren_info)->pipeline_bypass);
    reset_trace(); assert(aren_get_uid(9, &uid, true) == 0);
    assert(append_count == 2 && !strcmp(appended[0], "queue") && !strcmp(appended[1], "lipsync"));
    unsigned old = uid; assert(aren_get_uid(9, &uid, true) == 0 && uid == old && append_count == 2);
    assert(aren_get_tail_uid(9, &uid, true) == 0 && uid == old + 1);
    assert(aren_get_tail_uid(9, &uid, false) == 0 && uid == 778); unlocked();
    fd_close(&observer); assert(!arenderer_info[9].is_open && blocks == 0);
    assert(arenderer_info[9].proc_id == 82);
    assert(aren_set_proc_id(9, 83) == -1 && arenderer_info[9].proc_id == 82); unlocked();
    uid = 123; assert(aren_get_tail_uid(9, &uid, true) == -1 && uid == 123); unlocked();
    assert(aren_get_uid(9, &uid, false) == -1); int deleted = destroys;
    fd_close(&owner); assert(destroys == deleted);
    assert(aren_open(9) == 0 && arenderer_info[9].proc_id == -1);
    assert(aren_close(9) == 0); unlocked();
    fd_open(&owner); assert(select_index(&owner, 0) == 0);
    fd_open(&observer); fd_close(&observer); assert(!arenderer_info[0].is_open); fd_close(&owner);
    fd_open(&owner); assert(select_index(&owner, -100) == 0);
    assert(((struct audio_renderer_info *)owner.private_data)->index == 0); fd_close(&owner);
    fd_open(&owner); assert(select_index(&owner, 10) == -1); fd_close(&owner);
    fd_open(&owner); assert(select_index(&owner, 2) == 0);
    assert(select_index(&owner, 3) == 0); fd_close(&owner);
    assert(arenderer_info[2].is_open); assert(aren_close(2) == 0);
    fail_block = 1; assert(aren_open(4) == 0 && arenderer_info[4].is_open);
    assert(!arenderer_info[4].ren_info); assert(aren_get_uid(4, &uid, false) == -1);
    assert(aren_close(4) == 0); fail_block = 0;
    assert(aren_open(5) == 0);
    ARENDERER_INFO_T *slot = &arenderer_info[5];
    slot->pcm_info.pcm_multi_ch = 1; reset_trace();
    assert(aren_get_uid(5, &uid, true) == 0);
    struct aud_renderer_info *r = (void *)slot->ren_info;
    assert(uid == (unsigned)r->uid_rts_bypass && append_count == 3);
    assert(!strcmp(appended[0], "rts") && !strcmp(appended[1], "srcbin") && !strcmp(appended[2], "lipsync"));
    assert(registry_count == 2 && !strcmp(registry_name[0], "rts") && !strcmp(registry_name[1], "lipsync"));
    assert(forward_count == 2 && prop(r->uid_pcmcvt_bypass, "out-channels", 2));
    for (int i = 0; i < property_count; i++) if (property_target[i] == r->uid_pcmcvt_bypass)
        assert(!strcmp(property_name[i], "out-channels"));
    old = uid; slot->pcm_info.pcm_multi_ch = 0;
    assert(aren_get_uid(5, &uid, true) == 0 && uid == old && append_count == 3);
    renderer_set_pcm_bypass_rts_out_channels(slot->ren_info, 8);
    assert(prop(r->uid_pcmcvt_bypass, "out-channels", 8));
    assert(aren_close(5) == 0);
    assert(aren_open(6) == 0); reset_trace();
    assert(aren_get_uid(6, &uid, true) == 0); deleted = pipeline_deletes;
    arenderer_info[6].pcm_info.pcm_bypass = 1; append_count = registry_count = 0;
    assert(aren_get_uid(6, &uid, true) == 0 && pipeline_deletes == deleted + 1);
    assert(aren_close(6) == 0 && !arenderer_info[6].pcm_info.pcm_bypass);
    unlocked(); assert(blocks == 0 && disconnects == destroys);
    struct aud_renderer_info routing = {0};
    struct renderer_module_param src = {.name = "src", .param = param_src, .uid = 401, .uid2 = -1};
    struct renderer_module_param gain = {.name = "gain", .param = param_gain, .uid = 501, .uid2 = -1};
    routing.list_module_param.next = &src.list; routing.list_module_param.prev = &gain.list;
    src.list.next = &gain.list; src.list.prev = &routing.list_module_param;
    gain.list.next = &routing.list_module_param; gain.list.prev = &src.list;
    reset_trace();
    assert(renderer_run_param(&routing.parent, "pcm_gain", 0x400000, NULL, NULL, true) == 0);
    assert(property_count == 1 && prop(401, "gain", 0x400000));
    assert(renderer_run_param(&routing.parent, "gain-enable", 1, NULL, NULL, true) == 0);
    assert(prop(401, "gain-enable", 1));
    assert(renderer_run_param(&routing.parent, "gain", 0x800000, NULL, NULL, true) == 0);
    assert(prop(501, "gain", 0x800000));
    assert(renderer_run_param(&routing.parent, "{ease-volume}", 0, "yes", NULL, true) == 0);
    assert(prop(501, "{ease-volume}", -1));
    int value = 0;
    assert(renderer_run_param(&routing.parent, "pcm_gain", 0, NULL, &value, false) == 0 && value == 123);
    assert(renderer_run_param(&routing.parent, "unknown", 0, NULL, NULL, true) == -1);
    src.uid2 = 601;
    assert(renderer_run_param(&routing.parent, "gain-enable", 0, "yes", NULL, true) == 0);
    assert(prop(401, "gain-enable", -1) && prop(601, "gain-enable", -1));
    puts("PASS actual-source renderer ownership, second-fd teardown, bounds, bypass transitions/default targets");
    return 0;
}
