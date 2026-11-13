/* DSP2 job scheduler: FIFO order, front insertion, no double queueing, sync, run loop. */
#include <assert.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../src/dsp2/sched.c"

struct sched_queue sched_queues[2];
struct sched_run_queue sched_run_queues[2];
static int irq_depth, completes, waits_for, thread_count, thread_prio;
static void *thread_fn, *thread_arg;
static jmp_buf stop_loop;
static int ran[8], ran_ids[8], ran_count;
static char diagnostic[4096];
static size_t diagnostic_size;

void logm_message(const char *format, ...)
{
    assert(irq_depth > 0);
    va_list args;
    va_start(args, format);
    int length = vsnprintf(diagnostic + diagnostic_size,
                           sizeof diagnostic - diagnostic_size, format, args);
    va_end(args);
    assert(length >= 0 && (size_t)length < sizeof diagnostic - diagnostic_size);
    diagnostic_size += (size_t)length;
}

const char *sched_result_string(int result)
{
    return result == -7 ? "failure" : "success";
}

unsigned sched_irq_save(void) { return (unsigned)irq_depth++; }
void sched_irq_restore(unsigned ps) { assert(--irq_depth == (int)ps); }
void aud_init_completion(struct aud_completion *c) { memset(c, 0, sizeof *c); }
void aud_reinit_completion(struct aud_completion *c) { c->opaque[0] = 0; }
void aud_complete(struct aud_completion *c) { ++c->opaque[0]; ++completes; }
void aud_wait_for_completion(struct aud_completion *c)
{
    (void)c;
    if (++waits_for > 1)
        longjmp(stop_loop, 1);
}
void aud_mutex_init(void *m) { (void)m; }
void *sched_name_alloc(unsigned size) { return calloc(1, size); }
int sched_name_printf(char *dst, const char *fmt, ...)
{
    va_list ap; va_start(ap, fmt); int n = vsnprintf(dst, 16, fmt, ap); va_end(ap); return n;
}
void *aud_thread_create(const char *name, unsigned stack, int prio, void *fn, void *arg)
{
    (void)name; (void)stack; ++thread_count; thread_prio = prio; thread_fn = fn; thread_arg = arg;
    return arg;
}

static int record(int id, struct sched_job *job)
{
    assert(job->running && !job->link.next);
    ran[ran_count] = (int)(intptr_t)job->priv; ran_ids[ran_count++] = id;
    return 0;
}
static void job_init(struct sched_job *j, int tag)
{
    memset(j, 0, sizeof *j); j->fn = record; j->priv = (void *)(intptr_t)tag;
}
static void reset(void)
{
    for (int i = 0; i < 2; ++i) free(sched_queues[i].name);
    memset(sched_queues, 0, sizeof sched_queues);
    completes = waits_for = thread_count = ran_count = 0;
    sched_init();
}

static void test_init(void)
{
    reset();
    assert(thread_count == 1 && thread_prio == 10 && thread_arg == &sched_queues[1]);
    assert(thread_fn == (void *)sched_process && sched_queues[0].prio == 5);
    assert(!strcmp(sched_queues[0].name, "sched0") && !strcmp(sched_queues[1].name, "sched1"));
}

static void test_order_and_requeue(void)
{
    struct sched_job a, b, c, d;
    reset(); job_init(&a, 1); job_init(&b, 2); job_init(&c, 3); job_init(&d, 4);
    sched_queue(1, &a); sched_queue(1, &b); sched_queue(1, &a);
    sched_queue_front(1, &c, 1); sched_queue(1, &d);
    assert(completes == 5 && irq_depth == 0);
    assert(sched_sync(&b) == 1);
    int order[] = {3, 1, 2, 4};
    for (int i = 0; i < 4; ++i) assert((int)(intptr_t)sched_pop(&sched_queues[1])->priv == order[i]);
    assert(!sched_pop(&sched_queues[1]));
    a.running = 0;
    assert(sched_sync(&a) == 0);
}

static void test_process_runs_until_empty(void)
{
    struct sched_job a, b;
    reset(); job_init(&a, 7); job_init(&b, 8);
    sched_queue(1, &a); sched_queue(1, &b);
    if (!setjmp(stop_loop))
        sched_process(&sched_queues[1]);
    assert(ran_count == 2 && ran[0] == 7 && ran[1] == 8 && ran_ids[0] == 1 && ran_ids[1] == 1);
    assert(!a.running && !b.running && a.done.opaque[0] == 1 && b.done.opaque[0] == 1);
    assert(waits_for == 2 && irq_depth == 0);
}

static void test_dump_run_list(void)
{
    for (int priority = 0; priority < 2; ++priority) {
        struct sched_link *head = &sched_run_queues[priority].modules;
        head->next = head->prev = head;
    }
    diagnostic_size = 0;
    assert(sched_dump_run_list() == 0 && irq_depth == 0);
    assert(!strcmp(diagnostic, "pri0. list mod list\npri1. list mod list\ndone\n"));

    struct sched_run_module *modules[3];
    for (int index = 0; index < 3; ++index) {
        modules[index] = calloc(1, sizeof *modules[index] + 16);
        assert(modules[index]);
        snprintf(modules[index]->name, 16, "module%d", index);
        memset(modules[index]->unknown_078, 0xa5, sizeof modules[index]->unknown_078);
        modules[index]->last_result = -7;
        for (int priority = 0; priority < 3; ++priority)
            modules[index]->stats[priority] = (struct sched_mod_stats){
                11, 12, 13, priority == 1 ? 0 : 14, 15, 16, 17, 18
            };
        struct sched_link *head = &sched_run_queues[index == 2].modules;
        struct sched_link *link = &modules[index]->run_link;
        link->prev = head->prev;
        link->next = head;
        head->prev->next = link;
        head->prev = link;
    }
    unsigned char before[3][sizeof(struct sched_run_module) + 16];
    for (int index = 0; index < 3; ++index)
        memcpy(before[index], modules[index], sizeof before[index]);
    struct sched_run_queue queues_before[2];
    memcpy(queues_before, sched_run_queues, sizeof queues_before);
    diagnostic_size = 0;
    irq_depth = 1;
    assert(sched_dump_run_list() == 0 && irq_depth == 1);
    irq_depth = 0;
    assert(strstr(diagnostic, "module1") < strstr(diagnostic, "module0"));
    assert(strstr(diagnostic, "module0") < strstr(diagnostic, "pri1."));
    assert(strstr(diagnostic, "pri1.") < strstr(diagnostic, "module2"));
    assert(strstr(diagnostic, "failure(-7)"));
    assert(strstr(diagnostic, "pri 0, time last       12/max       13/cum       14, sched  15, count  11\n"));
    assert(!strstr(diagnostic, "pri 1,"));
    assert(strstr(diagnostic, "pri 2,"));
    assert(strstr(diagnostic, "wait time last       16/max       17/cum       18\n"));
    for (int index = 0; index < 3; ++index) {
        unsigned char *expected = before[index];
        memset(expected + offsetof(struct sched_run_module, stats), 0,
               sizeof modules[index]->stats);
        assert(!memcmp(expected, modules[index], sizeof before[index]));
        free(modules[index]);
    }
    assert(!memcmp(queues_before, sched_run_queues, sizeof queues_before));
}

int main(void)
{
    test_init();
    test_order_and_requeue();
    test_process_runs_until_empty();
    test_dump_run_list();
    reset();
    for (int i = 0; i < 2; ++i) free(sched_queues[i].name);
    puts("DSP2 sched: FIFO/front, sync, run loop, module dump order and statistics reset passed");
    return 0;
}
