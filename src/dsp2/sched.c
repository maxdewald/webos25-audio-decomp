/*
 * dsp2 job scheduler: two FIFO job queues. Queue 0 is drained by the main thread
 * (sched_mai), queue 1 by a "sched1" XOS thread at priority 10. IMC command headers run
 * as queue-1 jobs (see debug.c). Module output threads (fader/queue, aud_module_output_
 * thread_create) and the mixer thread are separate XOS threads, NOT scheduler jobs, so a
 * waiting fader thread does not stop this queue and a stuck queue does not stop it either.
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp2.bin
 *   sha256 c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d
 *   main segment build/fw/audio_a0_dsp2.main_60000000.bin
 *   sha256 d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd
 * Evidence: build/scratch/faderq/sched.lst, misc.txt (tools/xtobjdump.sh, no FLIX bundles).
 * Critical sections are "raise PS.INTLEVEL to 3" (rsr/wsr ps); shown as sched_irq_*.
 * Thread-name malloc/sprintf are symbol-table look-alikes (_BufMonitor_RemoveBufferIdxLocal,
 * CommonBuffer_InsertAuInternal) used with size 16 and "sched%d".
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

struct aud_completion { int opaque[6]; };
struct sched_link { struct sched_link *next, *prev; };

struct sched_job {
    int (*fn)(int queue_id, struct sched_job *job);  /* +0x00 */
    void *priv;                                       /* +0x04 */
    struct sched_link link;                           /* +0x08 next==NULL: not queued */
    uint8_t running;                                  /* +0x10 */
    struct aud_completion done;                       /* +0x14 */
};

struct sched_queue {                                  /* stride 0x40 @0x60128470 */
    int id;                                           /* +0x00 */
    struct sched_link jobs;                           /* +0x04 push at next, pop at prev */
    struct aud_completion wake;                       /* +0x0c */
    int prio;                                         /* +0x1c */
    char *name;                                       /* +0x20 */
    int mutex[7];                                     /* +0x24 initialised, unused here */
};

extern struct sched_queue sched_queues[2];            /* 0x60128470 */

extern unsigned sched_irq_save(void);
extern void sched_irq_restore(unsigned ps);
extern void aud_init_completion(struct aud_completion *);       /* @0x600a6770 */
extern void aud_reinit_completion(struct aud_completion *);     /* @0x600a6784 */
extern void aud_wait_for_completion(struct aud_completion *);   /* @0x600a68c4 */
extern void aud_complete(struct aud_completion *);              /* @0x600a68f4 */
extern void aud_mutex_init(void *);
extern void *sched_name_alloc(unsigned size);
extern int sched_name_printf(char *dst, const char *fmt, ...);
extern void *aud_thread_create(const char *name, unsigned stack, int prio,
                               void *fn, void *arg);

static struct sched_job *job_of(struct sched_link *l)
{
    return (struct sched_job *)((char *)l - offsetof(struct sched_job, link));
}

/* Pop the oldest job, or NULL. Separate only for host tests; inlined in sched_process. */
struct sched_job *sched_pop(struct sched_queue *q)
{
    struct sched_link *l;
    struct sched_job *job = NULL;
    unsigned ps = sched_irq_save();

    l = q->jobs.prev;
    if (l != &q->jobs) {
        job = job_of(l);
        if (l->next) {
            l->next->prev = l->prev;
            l->prev->next = l->next;
        }
        l->next = NULL;
        job->running = 1;
    }
    sched_irq_restore(ps);
    return job;
}

/* @0x6007c3f8: never returns. Sleeps on q->wake only while the queue is empty; a job
 * that blocks keeps every later job (including IMC command headers) waiting. */
void sched_process(struct sched_queue *q)
{
    for (;;) {
        struct sched_job *job;

        aud_wait_for_completion(&q->wake);
        aud_reinit_completion(&q->wake);
        while ((job = sched_pop(q)) != NULL) {
            job->fn(q->id, job);
            job->running = 0;
            aud_complete(&job->done);
        }
    }
}

/* @0x6007c46c: queue 0 gets no thread (sched_mai runs it); queue 1 runs at prio 10. */
int sched_init(void)
{
    for (int i = 0; i < 2; i++) {
        struct sched_queue *q = &sched_queues[i];

        q->id = i;
        q->jobs.prev = q->jobs.next = &q->jobs;
        aud_init_completion(&q->wake);
        aud_mutex_init(q->mutex);
        q->name = sched_name_alloc(16);
        sched_name_printf(q->name, "sched%d", q->id);
        q->prio = 5 + 5 * i;
        if (i)
            aud_thread_create(q->name, 0x40000, q->prio, (void *)sched_process, q);
    }
    return 0;
}

/* @0x6007c4d0 */
int sched_mai(void)
{
    sched_process(&sched_queues[0]);
    return 0;
}

/* @0x6007c4e0: an already-queued job is not moved; the queue is woken either way. */
int sched_queue_front(int qidx, struct sched_job *job, int front)
{
    struct sched_queue *q = &sched_queues[qidx];
    unsigned ps = sched_irq_save();

    if (!job->link.next) {
        aud_init_completion(&job->done);
        job->running = 0;
        if (front) {
            if (&job->link != q->jobs.next) {
                job->link.prev = q->jobs.prev;
                job->link.next = &q->jobs;
                q->jobs.prev->next = &job->link;
                q->jobs.prev = &job->link;
            }
        } else if (&job->link != q->jobs.next) {
            job->link.next = q->jobs.next;
            job->link.prev = &q->jobs;
            q->jobs.next->prev = &job->link;
            q->jobs.next = &job->link;
        }
    }
    sched_irq_restore(ps);
    aud_complete(&q->wake);
    return 0;
}

/* @0x6007c554 */
int sched_queue(int qidx, struct sched_job *job)
{
    return sched_queue_front(qidx, job, 0);
}

/* @0x6007c564: wait for a queued or running job; 1 if it had to wait. */
int sched_sync(struct sched_job *job)
{
    unsigned ps = sched_irq_save();
    bool wait = job->link.next || job->running;

    if (wait)
        aud_reinit_completion(&job->done);
    sched_irq_restore(ps);
    if (!wait)
        return 0;
    aud_wait_for_completion(&job->done);
    return 1;
}
