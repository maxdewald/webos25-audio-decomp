/* DSP2 shared intrusive list, saved image VA6006c520..6006c7a0.
 * Semantic host layouts; target +0 prev,+4 next,+8 mutex,+24 root/serial,
 * +28 data/count,+2c node serial. No guessed generic-list cleanup behavior.
 */
#include <stddef.h>
#include <stdint.h>
struct aud_link {
    struct aud_link *prev,*next;
    unsigned mutex[7];
    union { struct aud_link *root; uint32_t sequence; } owner;
    union { void *data; uint32_t count; } payload;
    uint32_t serial;
};
extern void aud_mutex_init(void *),aud_mutex_lock(void *),aud_mutex_unlock(void *);
extern void *aud_link_malloc(unsigned);
extern void aud_link_free(void *);
/* @0x6006c520: does NOT reset sequence/count; framework zero-init supplies them. */
void aud_link_root_init(struct aud_link *root)
{
    root->prev=root->next=root;
    aud_mutex_init(root->mutex);
}
/* @0x6006c534: does NOT initialize prev/next/serial. */
void aud_link_node_init(struct aud_link *node,void *data)
{
    aud_mutex_init(node->mutex);node->payload.data=data;node->owner.root=NULL;
}
/* @0x6006c548: add at head. The odd prev==node guard skips relinking but still
 * increments count/sequence and changes node serial; no duplicate rejection. */
int aud_link_add_locked(struct aud_link *root,struct aud_link *node)
{
    if(root->prev!=node) {
        node->prev=root;node->next=root->next;
        root->next->prev=node;root->next=node;
    }
    node->owner.root=root;
    node->serial=++root->owner.sequence;
    ++root->payload.count;
    return 0;
}
/* @0x6006c584 */
int aud_link_add(struct aud_link *root,struct aud_link *node)
{
    aud_mutex_lock(root->mutex);aud_link_add_locked(root,node);
    aud_mutex_unlock(root->mutex);return 0;
}
/* @0x6006c5a4: tail insert, with the same prev==node guard. */
int aud_link_add_tail_locked(struct aud_link *root,struct aud_link *node)
{
    if(root->prev!=node) {
        node->next=root;node->prev=root->prev;
        root->prev->next=node;root->prev=node;
    }
    node->owner.root=root;
    node->serial=++root->owner.sequence;
    ++root->payload.count;
    return 0;
}
/* @0x6006c5e0 */
int aud_link_add_tail(struct aud_link *root,struct aud_link *node)
{
    aud_mutex_lock(root->mutex);aud_link_add_tail_locked(root,node);
    aud_mutex_unlock(root->mutex);return 0;
}
/* @0x6006c600: root must be valid. prev/next remain stale after unlink. */
int aud_link_del_locked(struct aud_link *node)
{
    struct aud_link *root=node->owner.root;
    if(node->prev!=NULL) {
        node->prev->next=node->next;node->next->prev=node->prev;
    }
    --root->payload.count;node->owner.root=NULL;return 0;
}
/* @0x6006c630 */
int aud_link_del(struct aud_link *node)
{
    struct aud_link *root=node->owner.root;
    aud_mutex_lock(root->mutex);aud_link_del_locked(node);
    aud_mutex_unlock(root->mutex);return 0;
}
/* @0x6006c654: called with root locked; snapshots serials, then reacquires root
 * per serial so callbacks can mutate list. Node mutex is locked before root
 * unlock. It is NOT unlocked here: callback/caller owns that handoff contract.
 * Any nonzero callback stops iteration but the result remains0. */
int _foreach_node(struct aud_link *root,void *context,int (*callback)(void *,void *),uint32_t *ids)
{
    uint32_t count=0;
    for(struct aud_link *n=root->next;n!=root;n=n->next) ids[count++]=n->serial;
    if(count!=root->payload.count) { aud_mutex_unlock(root->mutex);return -1; }
    aud_mutex_unlock(root->mutex);
    for(uint32_t i=0;i<count;++i) {
        aud_mutex_lock(root->mutex);
        struct aud_link *n=root->next;
        while(n!=root && n->serial!=ids[i]) n=n->next;
        if(n==root) { aud_mutex_unlock(root->mutex);continue; }
        aud_mutex_lock(n->mutex);aud_mutex_unlock(root->mutex);
        if(callback(context,n->payload.data)!=0) return 0;
    }
    return 0;
}
/* @0x6006c718: <=256 serials use stack, otherwise heap count*4. */
int aud_link_foreach_node(struct aud_link *root,void *context,int (*callback)(void *,void *))
{
    aud_mutex_lock(root->mutex);
    if(root->payload.count<=256) {
        uint32_t ids[256];return _foreach_node(root,context,callback,ids);
    }
    uint32_t *ids=aud_link_malloc(root->payload.count*4u);
    if(ids==NULL) { aud_mutex_unlock(root->mutex);return -1; }
    int result=_foreach_node(root,context,callback,ids);aud_link_free(ids);return result;
}
