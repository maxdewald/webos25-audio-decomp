/* Mock matrix math/framework; validate recovered mixer control/byte paths only. */
#include <assert.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../../src/dsp2/mixer.c"

static unsigned mask;
volatile unsigned *mixer_log_mask=&mask;
void *mixer_log_ctx;
const struct layout_desc aud_basemixer_layouts[13] = {
    [2] = {"stereo", 0x02000003}
};
static struct lgmix_input inputs[3];
static int input_count, unmaps, flushes, maps, row_calls;
static uint8_t source_bytes[128];
static struct aud_data copied;
static int32_t scripted[3][4][2];
static int frames_seen[3];
void logmm_printl(void *ctx,int level,const char *fn,int line,const char *fmt,...)
{ (void)ctx;(void)level;(void)fn;(void)line;(void)fmt; }
void aud_mutex_lock(void *m) { (void)m; }
void aud_mutex_unlock(void *m) { (void)m; }
struct lgmix_input *lgmix_list_first(struct lgmixer *m) { (void)m; return input_count ? &inputs[0] : NULL; }
struct lgmix_input *lgmix_list_next(struct lgmixer *m,struct lgmix_input *in)
{ (void)m; return in-inputs+1<input_count ? in+1 : NULL; }
void aud_data_adapter_init(struct aud_adapter *a) { memset(a,0,sizeof(*a)); }
void list_add_tail_(void *head,void *node) { (void)head;(void)node; assert(!"unexpected input allocation"); }
void *aud_data_adapter_map(struct aud_adapter *a,int bytes)
{ (void)a;assert(bytes>0);++maps;return source_bytes; }
void aud_data_adapter_unmap(struct aud_adapter *a) { (void)a;++unmaps; }
void aud_data_adapter_flush(struct aud_adapter *a,int bytes) { a->rd+=bytes;++flushes; }
struct aud_data *aud_data_adapter_get_data(struct aud_adapter *a,int bytes)
{ assert(bytes==ADAPTER_AVAIL(a));return &copied; }
struct aud_data *aud_data_adapter_dequeue(struct aud_adapter *a) { a->rd=a->wr;return &copied; }
void aud_object_put(void *p) { (void)p; }
struct aud_data *aud_data_new(int bytes,...)
{
    struct aud_data *d=calloc(1,sizeof(*d));assert(d);
    d->size=bytes;d->buf=malloc((size_t)bytes);assert(d->buf);
    memset(d->buf,0xcc,(size_t)bytes);return d;
}
struct aud_data *aud_data_new_zero(int bytes)
{ struct aud_data *d=aud_data_new(bytes);memset(d->buf,0,(size_t)bytes);return d; }
static void release(struct aud_data *d) { free(d->buf);free(d); }
void lgmix_matrix_row32_flix(const void *frame,const int32_t *row,int n,int32_t *dst)
{
    assert(frame==source_bytes && n==2);
    assert(row==inputs[0].matrix+row_calls*12);
    *dst=17+row_calls++;
}
void lgmix_matrix_row16_flix(const void *frame,const int32_t *row,int n,int32_t *dst)
{ lgmix_matrix_row32_flix(frame,row,n,dst); }
static void fetch(struct basemixer *bm,struct lgmix_input *in,int32_t *dst)
{
    (void)bm;
    unsigned i=(unsigned)(in-inputs);
    int f=frames_seen[i]++;
    dst[0]=scripted[i][f][0];dst[1]=scripted[i][f][1];
}
static struct lgmixer fresh(int count)
{
    struct lgmixer lm={0};
    memset(inputs,0,sizeof(inputs));memset(frames_seen,0,sizeof(frames_seen));
    maps=unmaps=flushes=0;input_count=count;
    lm.out_layout=0x20000002;lm.output_connect=true;lm.out_channels=2;
    lm.main_audio_port=0;lm.nullgen_port=9;
    for (int i=0;i<count;++i) {
        inputs[i].c.port=i;inputs[i].c.enabled=true;inputs[i].c.frame_bytes=8;
        inputs[i].c.channels=2;inputs[i].c.fifo.wr=16;
        inputs[i].layoutch=lm.out_layout;inputs[i].bps_in=32;
        inputs[i].get_samples=fetch;
    }
    return lm;
}
static void row_traversal(void)
{
    struct lgmixer lm=fresh(1);
    struct lgmix_input *in=&inputs[0];
    int32_t out[2];
    in->out_ch=2;in->in_ch=8;in->n_ch=2;in->samples=(int32_t *)source_bytes;
    row_calls=0;get_samples_default32(&lm.bm,in,out);
    assert(row_calls==2 && out[0]==17 && out[1]==18);
    assert((uint8_t *)in->samples==source_bytes+32);
    in->samples=(int32_t *)source_bytes;row_calls=0;
    get_samples_default16(&lm.bm,in,out);
    assert(row_calls==2 && (uint8_t *)in->samples==source_bytes+16);
    in->out_ch=0;in->samples=(int32_t *)source_bytes;row_calls=0;
    get_samples_default32(&lm.bm,in,out);
    assert(!row_calls && (uint8_t *)in->samples==source_bytes+32);
}
static void clamp_and_unmap(void)
{
    struct lgmixer lm=fresh(2);
    scripted[0][0][0]=32767;scripted[0][0][1]=-32768;
    scripted[1][0][0]=0;scripted[1][0][1]=0;
    scripted[0][1][0]=32766;scripted[0][1][1]=-32767;
    scripted[1][1][0]=0;scripted[1][1][1]=0;
    struct aud_data *out=mix_signals(&lm,1,2);
    const int32_t *v=out->buf;
    assert(v[0]==INT32_MAX && v[1]==INT32_MIN);
    assert(v[2]==0x7ffe0000 && v[3]==(int32_t)0x80010000u);
    assert(maps==2 && unmaps==2 && !inputs[0].mapped && !inputs[1].mapped);
    assert(!flushes);release(out);

    lm=fresh(2);
    scripted[0][0][0]=INT32_MAX;scripted[0][0][1]=5;
    scripted[1][0][0]=1;scripted[1][0][1]=6;
    out=mix_signals(&lm,1,1);v=out->buf;
    assert(v[0]==INT32_MIN && v[1]==11*65536);
    release(out);
}
static void bypass_and_disconnect(void)
{
    struct lgmixer lm=fresh(1);
    uint32_t layout=lm.out_layout;
    assert(!lm.bm.n_main_ready);
    struct aud_data *out=lgmix_do_mix(&lm.bm,&layout,TODO_MIXING,2);
    assert(out==&copied && !maps && !unmaps && flushes==1);
    lm=fresh(1);lm.output_connect=false;layout=lm.out_layout;
    assert(!lgmix_do_mix(&lm.bm,&layout,TODO_MIXING,2));
    assert(!maps && !unmaps && flushes==1);
    lm=fresh(1);lm.out_layout=0x20000001;layout=0;
    assert(lgmix_do_mix(&lm.bm,&layout,TODO_MIXING,2)==&copied);
    assert(layout==0x20000002 && lm.out_layout==0x20000002);
    lm=fresh(1);inputs[0].c.low_delay=true;inputs[0].c.fifo.wr=0;
    out=do_bypass(&lm,1);
    assert(out && out!=&copied && out->size==2048 && inputs[0].c.zero_mixing);
    for (int i=0;i<out->size;++i) assert(!((uint8_t *)out->buf)[i]);
    release(out);
    inputs[0].c.fifo.wr=0;inputs[0].c.fifo.rd=1;
    assert(lgmix_can_mix(&lm.bm,&inputs[0].c)==1);
    inputs[0].c.fifo.rd=0;
    assert(lgmix_can_mix(&lm.bm,&inputs[0].c)==0);
}
int main(void)
{
    row_traversal();clamp_and_unmap();bypass_and_disconnect();
    puts("PASS mixer: matrix row traversal, wrap/clamp endpoints, unmap, no-main bypass, disconnected drain; physical audibility untested");
    return 0;
}
