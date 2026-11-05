/*
 * Red-black tree keyed by unsigned int (used by IMC for its event/action tables).
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 *
 * Evidence: Ghidra build/decomp/libaa.so.2.0.0.c @0x22d80..@0x233ea, strings at 0x34f78..0x34fe0
 * ("assert failed. (%s)\n" + condition). Return codes are positive ADEC_RESULT values.
 * Layouts (from the binary):
 *   tree 0xc bytes { root, nil sentinel, terminator(info) }, root == NULL when empty
 *   node 0x18 bytes { key, info, color, parent, left, right }; leaves point at the sentinel
 * Key 0 is reserved. The rotations / fix-ups are inlined in the binary; here they are
 * written out as small static helpers (textbook red-black tree).
 */
#include <stdlib.h>

#include "buffers.h"

enum { RB_RED, RB_BLACK };

typedef struct _RBNode {
    RBKey key;
    void *info;
    int color;
    struct _RBNode *parent;
    struct _RBNode *left;
    struct _RBNode *right;
} RBNode;

typedef struct _RBTree {
    RBNode *root;
    RBNode *nil;
    void (*terminator)(void *info);
} RBTree;

#define RB_ASSERT(cond, cond_str, line) do { \
        if (!(cond)) { \
            LOGE(aud, line, "assert failed. (%s)\n", cond_str); \
            return ADEC_ERR_NULL; \
        } \
    } while (0)

/* @0x22d80: default terminator, info is not owned by the tree */
void _list_default_terminator(void *info)
{
    (void)info;
}

/* @0x22d84: post-order free of a subtree, calling the terminator on every info */
int _RBTree_DeleteAllRBNode(RBTree *tree, RBNode *node)
{
    if (node == tree->nil || node == NULL)
        return 0;
    _RBTree_DeleteAllRBNode(tree, node->left);
    _RBTree_DeleteAllRBNode(tree, node->right);
    tree->terminator(node->info);
    free(node);
    return 0;
}

/* @0x22db4 */
int RBTree_InitTree(void *_tree, void (*terminator)(void *info))
{
    RBTree *tree = _tree;
    RBNode *nil = malloc(sizeof(*nil));

    tree->nil = nil;
    if (!nil)
        return ADEC_ERR_NOT_ENOUGH_MEM;
    tree->terminator = terminator ? terminator : _list_default_terminator;
    nil->key = 0;
    nil->color = RB_BLACK;
    nil->parent = nil->left = nil->right = nil;
    tree->root = NULL;
    return 0;
}

/* @0x22dec */
int RBTree_CreateTree(void **_tree, void (*terminator)(void *info))
{
    RBTree *tree = malloc(sizeof(*tree));

    *_tree = tree;
    if (!tree)
        return ADEC_ERR_NOT_ENOUGH_MEM;
    return RBTree_InitTree(tree, terminator);
}

/* @0x22e0c */
int RBTree_DeleteTree(void *_tree)
{
    RBTree *tree = _tree;

    RB_ASSERT(tree != NULL, "_tree != NULL", 0xfd);
    _RBTree_DeleteAllRBNode(tree, tree->root);
    free(tree->nil);
    free(tree);
    return 0;
}

static inline void rb_replace_child(RBTree *tree, RBNode *old, RBNode *new)
{
    RBNode *p = old->parent;

    if (old == p->left)
        p->left = new;
    else if (old == p->right)
        p->right = new;
    new->parent = p;
    (void)tree;
}

static inline void rb_rotate_left(RBTree *tree, RBNode *x)
{
    RBNode *y = x->right;

    rb_replace_child(tree, x, y);
    x->right = y->left;
    if (y->left != tree->nil)
        y->left->parent = x;
    y->left = x;
    x->parent = y;
    if (x == tree->root) {
        y->color = RB_BLACK;
        tree->root = y;
    }
}

static inline void rb_rotate_right(RBTree *tree, RBNode *x)
{
    RBNode *y = x->left;

    rb_replace_child(tree, x, y);
    x->left = y->right;
    if (y->right != tree->nil)
        y->right->parent = x;
    y->right = x;
    x->parent = y;
    if (x == tree->root) {
        y->color = RB_BLACK;
        tree->root = y;
    }
}

/* @0x22e78: insert; an existing key gets its info replaced (ADEC_ERR_DUPLICATED) */
int RBTree_CreateNode(void *_tree, RBKey key, void *info)
{
    RBTree *tree = _tree;
    RBNode *nil, *parent, *node, *uncle, *gp;

    RB_ASSERT(tree != NULL && info != NULL, "_tree != NULL && _info != NULL", 0x111);
    if (key == 0)
        return ADEC_ERR_RANGE;

    nil = tree->nil;
    if (tree->root == NULL || tree->root == nil) {
        node = malloc(sizeof(*node));
        if (!node)
            return ADEC_ERR_NOT_ENOUGH_MEM;
        node->key = key;
        node->left = node->right = nil;
        node->info = info;
        node->color = RB_BLACK;
        node->parent = nil;
        tree->root = node;
        return 0;
    }

    for (parent = tree->root;;) {
        RBNode *next;

        if (parent->key == key) {
            parent->info = info;
            return ADEC_ERR_DUPLICATED;
        }
        next = key > parent->key ? parent->right : parent->left;
        if (next == nil)
            break;
        parent = next;
    }

    node = malloc(sizeof(*node));
    if (!node)
        return ADEC_ERR_NOT_ENOUGH_MEM;
    node->key = key;
    node->info = info;
    node->left = node->right = nil;
    node->color = RB_RED;
    node->parent = parent;
    if (key < parent->key)
        parent->left = node;
    else
        parent->right = node;

    /* fix-up: walk up while two reds are adjacent */
    while (node->parent->color == RB_RED) {
        parent = node->parent;
        gp = parent->parent;
        if (parent == gp->left) {
            uncle = gp->right;
            if (uncle->color == RB_RED) {
                if (gp != tree->root)
                    gp->color = RB_RED;
                parent->color = RB_BLACK;
                uncle->color = RB_BLACK;
                node = gp;
                continue;
            }
            if (node == parent->right) {
                node = parent;
                rb_rotate_left(tree, node);
                parent = node->parent;
            }
            parent->color = RB_BLACK;
            parent->parent->color = RB_RED;
            rb_rotate_right(tree, parent->parent);
        } else {
            uncle = gp->left;
            if (uncle->color == RB_RED) {
                if (gp != tree->root)
                    gp->color = RB_RED;
                parent->color = RB_BLACK;
                uncle->color = RB_BLACK;
                node = gp;
                continue;
            }
            if (node == parent->left) {
                node = parent;
                rb_rotate_right(tree, node);
                parent = node->parent;
            }
            parent->color = RB_BLACK;
            parent->parent->color = RB_RED;
            rb_rotate_left(tree, parent->parent);
        }
    }
    return 0;
}

/* @0x23094: *info = NULL and ADEC_ERR_NOT_FOUND when absent */
int RBTree_Search(void *_tree, RBKey key, void *info_out)
{
    RBTree *tree = _tree;
    void **info = info_out;
    RBNode *n;

    RB_ASSERT(info != NULL, "_info != NULL", 0x1a4);
    for (n = tree->root; n != NULL && n != tree->nil;) {
        if (n->key == key) {
            *info = n->info;
            return 0;
        }
        n = key < n->key ? n->left : n->right;
        if (n == NULL)
            return ADEC_ERR_CORRUPTED;
    }
    *info = NULL;
    return ADEC_ERR_NOT_FOUND;
}

/*
 * Delete fix-up as the binary does it (inlined in RBTree_Delete @0x231d0..0x233c0).
 * [observed] Unlike the textbook algorithm the sibling is ALWAYS p->left (only the
 * "x is a right child" mirror case exists, although x usually is a left child here),
 * and every case except "red parent, two black nephews" stops after at most one
 * rotation. Combined with RBTree_Delete skipping the fix-up for black leaves, the tree
 * is only approximately balanced; lookups stay correct.
 */
static inline void rb_delete_fixup(RBTree *tree, RBNode *x)
{
    RBNode *p, *s;

    while (x != tree->root && x->color == RB_BLACK && x != tree->nil) {
        p = x->parent;
        s = p->left;
        if (p->color == RB_RED && s->left->color == RB_BLACK &&
            s->right->color == RB_BLACK) {
            p->color = RB_BLACK;
            s->color = RB_RED;
            x = p;
            continue;
        }
        if (s->color == RB_RED) {
            rb_rotate_right(tree, p);
            p->color = RB_RED;
            s->color = RB_BLACK;
        } else if (s->left->color == RB_RED) {
            s->left->color = RB_BLACK;
            rb_rotate_right(tree, p);
        } else if (s->right->color == RB_RED) {
            rb_rotate_left(tree, s);
            s->parent->color = RB_BLACK;
            s->color = RB_RED;
        } else {
            s->color = RB_RED;
        }
        x = tree->root;
    }
    x->color = RB_BLACK;
}

/* @0x2312c: remove key; the terminator is called on its info first. With two children
 * the successor's key/info are copied into the found node and the successor is freed. */
int RBTree_Delete(void *_tree, RBKey key)
{
    RBTree *tree = _tree;
    RBNode *nil, *z, *y, *x;

    if (!tree || !tree->root)
        return ADEC_ERR_NOT_FOUND;
    nil = tree->nil;
    for (z = tree->root; z != nil;) {
        if (z->key == key)
            break;
        z = key < z->key ? z->left : z->right;
        if (z == NULL)
            return ADEC_ERR_CORRUPTED;
    }
    if (z == nil)
        return ADEC_ERR_NOT_FOUND;

    tree->terminator(z->info);

    if (z->left == nil || z->right == nil) {
        x = z->left != nil ? z->left : z->right;
        if (x == nil) {
            /* leaf */
            if (z == tree->root) {
                free(z);
                tree->root = NULL;
                return 0;
            }
            if (z->parent->left == z)
                z->parent->left = nil;
            else
                z->parent->right = nil;
            free(z);
            return 0;
        }
        /* one child: it takes z's place */
        if (z == tree->root) {
            x->color = RB_BLACK;
            x->parent = z->parent;
            tree->root = x;
        } else {
            rb_replace_child(tree, z, x);
        }
        free(z);
        return 0;
    }

    /* two children: splice out the in-order successor y (leftmost of the right subtree) */
    for (y = z->right;; y = y->left) {
        if (y == NULL)
            return ADEC_ERR_CORRUPTED;
        if (y->left == nil)
            break;
    }
    x = y->right;
    if (y->parent->left == y)
        y->parent->left = x;
    else
        y->parent->right = x;
    if (x != nil)
        x->parent = y->parent;
    z->key = y->key;
    z->info = y->info;
    if (y->color == RB_BLACK && x != nil)
        rb_delete_fixup(tree, x);
    free(y);
    return 0;
}
