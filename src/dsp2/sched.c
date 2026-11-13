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

struct sched_mod_stats {
    uint32_t count, last, max, cumulative, scheduled;
    uint32_t wait_last, wait_max, wait_cumulative;
};

struct sched_run_module {
    uint8_t unknown_000[0x18];
    struct sched_mod_stats stats[3];                    /* +0x018, stride 0x20 */
    uint8_t unknown_078[0xa0];
    struct sched_link run_link;                        /* +0x118 */
    int last_result;                                  /* +0x120 */
    uint8_t unknown_124[0x104];
    char name[];                                      /* +0x228 */
};

struct sched_run_queue {
    uint32_t unknown_000;
    struct sched_link modules;                         /* +0x04 */
    uint8_t unknown_00c[0x40];
};

extern struct sched_run_queue sched_run_queues[2];      /* 0x601283d0, DSP stride 0x4c */
extern void logm_message(const char *format, ...);      /* @0x6007af14 */
/* @0x6006c508: called with last_result; symtab says ADEC_GetErrorString,
 * but that identity is unverified. Only the returned string is used here. */
extern const char *sched_result_string(int result);

/* @0x6007beb0: module scheduling/wait-time statistics, not an XOS wait object
 * or blocked PC. Printing clears all three 0x20-byte statistics blocks. */
static void print_mod_info(struct sched_run_module *module, uint32_t divisor)
{
    unsigned ps = sched_irq_save();
    uint32_t total = module->stats[0].cumulative + module->stats[1].cumulative
                   + module->stats[2].cumulative;

    logm_message("module : %p(%10s), %3u\xe2\x80\xb0\n",
                 (void *)module, module->name, total / divisor);
    logm_message("  last_result : %s(%d)\n",
                 sched_result_string(module->last_result), module->last_result);
    for (int priority = 0; priority < 3; ++priority) {
        struct sched_mod_stats *stats = &module->stats[priority];

        if (stats->cumulative) {
            logm_message("  pri %d, time last%9u/max%9u/cum%9u, sched%4u, count%4u\n",
                         priority, stats->last, stats->max, stats->cumulative,
                         stats->scheduled, stats->count);
            logm_message("    wait time last%9u/max%9u/cum%9u\n",
                         stats->wait_last, stats->wait_max, stats->wait_cumulative);
        }
    }
    for (int priority = 0; priority < 3; ++priority)
        module->stats[priority] = (struct sched_mod_stats){0};
    sched_irq_restore(ps);
}

/* @0x6007bf78: no arguments; traverses two module run lists, NOT sched_queues
 * jobs or XOS threads. No list mutation, but print_mod_info resets statistics.
 * Interrupt level is at least 3 throughout logging; returns 0.
 * Actual thread diagnostic: debug_xos_thread @0x600a6ec8 (TCB list head
 * 0x6016ff44, next +8, state byte +0x30, name +0x38). Its visible columns do
 * not identify a wait object or blocked PC. Like aud_show_thread @0x600a6d34,
 * it invokes display_cpu_load @0x600a6c04 and display_backtraces @0x600a6cd0.
 * display_cpu_load calls xos_get_cpu_load @0x600f27c8 with reset=1 and capacity
 * 32 (threads plus one interrupt entry). On success it clears each TCB's words
 * +0x70/+0x74/+0x78/+0x7c and interrupt counters at 0x6016dc80/+4, 0x6016dc8c;
 * insufficient capacity returns an error before resetting. display_backtraces
 * masks interrupts and temporarily clears byte 0x6016d4a1, restoring it later.
 * Neither thread entry consumes incoming arguments in this listing.
 * Passive condition-wait evidence (build/scratch/diag-thread):
 * aud_cond_wait @0x600a69d4 and aud_cond_wait_timeout @0x600a6950 clear cond+0,
 * unlock the supplied mutex, and, unless already signalled (or zero timeout),
 * pass cond+4 to xos_cond_wait @0x600f16b8. They reacquire the mutex on return.
 * xos_cond_wait reads current TCB from 0x60123a38, stores callback 0x600a6928 at
 * TCB+0x4c and argument at +0x50 (NULL untimed, cond timed). xos_block
 * @0x600f2c94 saves reason *(0x6003dc08)==0x6005a798 at +0x3c, clears ready
 * byte +0x30, and xos_q_push @0x600fbbf0 stores queue address cond+4 at +0x48.
 * Thus a coherent cached TCB with ready==0 and this reason identifies cond as
 * *(TCB+0x48)-4. +0x48 alone is NOT a wait object: ready queues use it too;
 * callback/argument alone may be stale. cond+0 is a signal count, not waiter
 * identity. A snapshot needs the condition queue/TCBs to validate membership;
 * it does not establish a live wait, mutex ownership, or a deadlock.
 * Saved backtrace seed: *(TCB+0x10), plus 0xa0 iff *(TCB+0x0c)==0x600f216f.
 * dump_backtrace @0x60089734 aligns that seed down to 4, reads saved return
 * word at frame-0x10 and previous SP at frame-0x0c, and resolves return address
 * (return_word & 0x3fffffff) | (previous_SP & 0xc0000000). Valid cached stack
 * bytes permit passive unwinding without diagnostic calls. These are saved
 * return addresses, NOT a proven exact suspended instruction PC. [gap] The
 * context-PC slot and indirect switch target at xos_switch_to @0x600f2bdc
 * (call through TCB+0x1c) are not recovered; no generic XOS frame is inferred.
 * Static waits: fader STOP @0x6008620c waits on fader+0x168 with mutex +0x13c;
 * starter @0x600784b4 waits on starter+0x50 with mutex +0x34, then synchronously
 * calls process_buf @0x60077f78. These do not prove a particular current wait.
 * This dump provides neither those saved thread fields nor stack bytes. */
int sched_dump_run_list(void)
{
    unsigned ps = sched_irq_save();

    for (int priority = 0; priority < 2; ++priority) {
        struct sched_link *head = &sched_run_queues[priority].modules;

        logm_message("pri%d. list mod list\n", priority);
        for (struct sched_link *link = head->prev; link != head; link = link->prev) {
            struct sched_run_module *module = (struct sched_run_module *)
                ((char *)link - offsetof(struct sched_run_module, run_link));

            print_mod_info(module, UINT32_MAX);
        }
    }
    logm_message("done\n");
    sched_irq_restore(ps);
    return 0;
}

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
