/* usage: cc -std=c11 -Wall -Wextra -Werror -pedantic -fsanitize=address,undefined
 * tools/tests/dsp2/msf_metadata.c -o build/scratch/msf-metadata/test &&
 * build/scratch/msf-metadata/test
 */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../src/dsp2/msf_metadata.c"

static int live, allocations, fail_at, irq_depth, irq_calls;
void *aud_meta_data_new(unsigned size)
{
    allocations++;
    if (allocations == fail_at)
        return NULL;
    void *p = malloc(size);
    assert(p);
    live++;
    return p;
}
void aud_meta_data_free(void *p)
{
    assert(p && live > 0);
    live--;
    free(p);
}
unsigned irq_raise_level(void)
{
    unsigned old = (unsigned)irq_depth;
    irq_depth++;
    irq_calls++;
    return old;
}
void irq_restore(unsigned ps)
{
    assert(irq_depth == (int)ps + 1);
    irq_depth = (int)ps;
}
static struct msf_md_connector connector(int slots)
{
    struct msf_md_connector c = {0};
    c.max_md = slots;
    c.max_payload = 8;
    c.max_samples = 100;
    return c;
}
static void init(struct msf_md_connector *c)
{
    c->head.prev = c->head.next = &c->head;
}
static void check_list(struct msf_md_connector *c)
{
    int count = 0;
    for (struct msf_link *p = c->head.next; p != &c->head; p = p->next) {
        assert(p->next->prev == p && p->prev->next == p);
        assert(++count < 100);
    }
    assert(count == c->num_md);
}
static struct msf_metadata metadata(int id, int type, int offset, int duration,
                                    unsigned char *payload)
{
    struct msf_metadata md = {id, type, offset, duration, 4, 7, payload};
    return md;
}
static void put(struct msf_md_connector *c, int id, int type, int offset,
                int duration, unsigned char *payload)
{
    struct msf_metadata md = metadata(id, type, offset, duration, payload);
    assert(msf_cmd_put_metadata(c, &md) == 0);
    check_list(c);
}
static void test_put_find(void)
{
    unsigned char a[4] = {1, 2, 3, 4}, b[4] = {5, 6, 7, 8};
    struct msf_md_connector c = connector(8);
    init(&c);
    assert(msf_update_write_buf(&c, 10) == 0);
    struct msf_metadata md = metadata(7, 0, 2, 10, a);
    assert(msf_cmd_put_metadata(&c, &md) == 0 && md.offset == 12);
    a[0] = 9;
    struct msf_metadata *copy = msf_find_metadata(&c, 7, 13);
    assert(copy && copy->offset == 12 && ((unsigned char *)copy->payload)[0] == 1);
    aud_meta_data_free(copy);
    assert(!msf_find_metadata(&c, 7, 12));
    put(&c, 8, 2, 50, 1, b);
    int n = c.num_md;
    put(&c, 8, 2, 99, 2, a);
    assert(c.num_md == n);
    copy = msf_find_metadata_sorted(&c, 8, 123, -100);
    assert(copy && copy->offset == 99 && copy->duration == 2);
    assert(((unsigned char *)copy->payload)[0] == 9);
    aud_meta_data_free(copy);
    md.payload_size = 9;
    assert(msf_cmd_put_metadata(&c, &md) == -1 && c.num_md == n);
    assert(msf_get_avail_samples_read(&c) == 10);
    assert(msf_get_avail_samples_write(&c) == 90);
    assert(msf_get_max_samples(&c) == 100);
    assert(msf_get_curr_num_md(&c) == n);
    assert(msf_get_avail_md_slot(&c) == 8 - n);
    assert(msf_update_write_buf(&c, 91) == -1 && c.samples == 10);
    assert(msf_flush_metadata(&c) == 0);
    check_list(&c);
}
static void test_merge(void)
{
    unsigned char a[4] = {1}, b[4] = {2};
    struct msf_md_connector c = connector(5);
    init(&c);
    put(&c, 3, 0, 0, 5, a);
    put(&c, 3, 0, 10, 5, a);
    put(&c, 3, 0, 20, 5, a);
    put(&c, 3, 0, 5, 15, a);
    assert(c.num_md == 3);
    assert(_msf_combine_identical_metadata(&c) == 2);
    assert(c.num_md == 1);
    struct msf_metadata *md = &md_item(c.head.next)->md;
    assert(md->offset == 0 && md->duration == 25);
    put(&c, 3, 1, 0, 10, a);
    put(&c, 4, 1, 40, 10, a);
    put(&c, 4, 1, 40, 10, a);
    put(&c, 3, 0, 0, 10, b);
    assert(c.num_md == 4);
    assert(_msf_combine_identical_metadata(&c) == 0);
    check_list(&c);
    msf_flush_metadata(&c);
    c.max_md = 1;
    put(&c, 1, 1, 0, 5, a);
    struct msf_metadata incoming = metadata(2, 1, 0, 5, a);
    assert(msf_cmd_put_metadata(&c, &incoming) == -1);
    assert(c.num_md == 1);
    msf_flush_metadata(&c);
}
static void test_take_split_failure(void)
{
    unsigned char a[4] = {3};
    struct msf_md_connector c = connector(4);
    init(&c);
    put(&c, 1, 0, 0, 20, a);
    fail_at = allocations + 1;
    assert(!msf_get_metadata(&c, 5));
    assert(c.num_md == 1 && md_item(c.head.next)->md.duration == 20);
    fail_at = allocations + 2;
    assert(!msf_get_metadata(&c, 5));
    assert(c.num_md == 2 && md_item(c.head.next)->md.duration == 5);
    check_list(&c);
    fail_at = 0;
    struct msf_metadata *md = msf_get_metadata(&c, 5);
    assert(md && md->offset == 0 && md->duration == 5 && c.num_md == 1);
    aud_meta_data_free(md);
    c.max_md = 1;
    assert(!msf_get_metadata(&c, 10) && c.num_md == 1);
    c.max_md = 4;
    md = msf_get_metadata_by_id(&c, 1, 10);
    assert(md && md->offset == 5 && md->duration == 5 && c.num_md == 1);
    aud_meta_data_free(md);
    msf_flush_metadata(&c);
    put(&c, 5, 1, 20, 1, a);
    put(&c, 5, 1, 10, 1, a);
    assert(!msf_get_metadata_by_id(&c, 5, 10));
    md = msf_get_metadata_by_id(&c, 5, 11);
    assert(md && md->offset == 10);
    aud_meta_data_free(md);
    md = msf_get_metadata(&c, 21);
    assert(md && md->offset == 20);
    aud_meta_data_free(md);
    assert(!msf_get_metadata(&c, 100));
    assert(!msf_get_metadata_by_id(&c, 99, 100));
    msf_flush_metadata(&c);
    put(&c, 1, 2, 50, 1, a);
    md = msf_get_metadata_by_id(&c, 1, 0);
    assert(md && md->offset == 50);
    aud_meta_data_free(md);
}
static void test_read(void)
{
    unsigned char a[4] = {4};
    struct msf_md_connector c = connector(4);
    init(&c);
    put(&c, 1, 0, 5, 10, a);
    put(&c, 2, 1, 0, 1, a);
    put(&c, 3, 2, 20, 0, a);
    c.samples = 30;
    assert(msf_update_read_buf(&c, 5) == 0);
    assert(c.samples == 25 && c.num_md == 2);
    assert(md_item(c.head.next)->md.offset == 0);
    assert(md_item(c.head.next)->md.duration == 10);
    assert(msf_update_read_buf(&c, 3) == 0);
    assert(md_item(c.head.next)->md.duration == 7);
    assert(msf_update_read_buf(&c, 30) == -1);
    assert(c.samples == 0 && c.num_md == 0);
    check_list(&c);
    msf_flush_metadata(&c);
}
static void test_command_queue(void)
{
    unsigned char a[4] = {6};
    struct msf_link head = {&head, &head};
    struct msf_metadata md = metadata(42, 1, 3, 4, a);
    assert(msf_add_put_metadata(NULL, &md) == 0x86);
    assert(msf_add_put_metadata(&head, NULL) == 0x86);
    assert(msf_add_update_write_buf(NULL, 1) == 0x86);
    fail_at = allocations + 1;
    assert(msf_add_put_metadata(&head, &md) == -32640);
    fail_at = allocations + 1;
    assert(msf_add_update_write_buf(&head, 1) == -32640);
    fail_at = 0;
    assert(msf_add_put_metadata(&head, &md) == 0);
    a[0] = 99;
    assert(msf_add_update_write_buf(&head, 13) == 0);
    struct msf_md_command *cmd = (struct msf_md_command *)head.next;
    assert(cmd->opcode == 4 && cmd->value.md.id == 42);
    assert(((unsigned char *)cmd->value.md.payload)[0] == 6);
    cmd = (struct msf_md_command *)cmd->link.next;
    assert(cmd->opcode == 10 && cmd->value.samples == 13);
    while (head.next != &head) {
        struct msf_link *p = head.next;
        md_unlink(p);
        aud_meta_data_free(p);
    }
}
int main(void)
{
    test_put_find();
    test_merge();
    test_take_split_failure();
    test_read();
    test_command_queue();
    assert(msf_cmd_put_metadata(NULL, NULL) == -1);
    assert(msf_update_write_buf(NULL, 1) == -1);
    assert(msf_update_read_buf(NULL, 1) == -1);
    assert(msf_flush_metadata(NULL) == -1);
    assert(!msf_get_metadata(NULL, 0));
    assert(!msf_get_metadata_by_id(NULL, 0, 0));
    assert(!msf_find_metadata(NULL, 0, 0));
    assert(!msf_get_avail_samples_read(NULL));
    assert(!msf_get_avail_samples_write(NULL));
    assert(!msf_get_max_samples(NULL));
    assert(!msf_get_curr_num_md(NULL));
    assert(!msf_get_avail_md_slot(NULL));
    assert(live == 0 && irq_depth == 0 && irq_calls > 0);
    puts("MSF metadata: copy/replace/coalesce, queue, split/failures, read/flush, accessors: PASS");
    return 0;
}
