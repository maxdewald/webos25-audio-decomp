/*
 * libaa IMC core ("com_imc"): command / response / event routing between audio
 * processors. A message whose target lives on this processor is dispatched locally
 * (aimc_dispatch_command); anything else is wrapped in an IpcHeader and handed to
 * aimc_ipc_send (kernel /dev/lg/audio_uimc for DSPs, aipc socket for Linux processes).
 * Also contains aimc_set_property (linked right after IMC in the original).
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 * Evidence: decomp/libaa.so.2.0.0.c @0x20d80..@0x22d84, logmm "com_imc"/"aud" strings,
 * command name tables at VA 0x3c0f4 / 0x3c1a4 (IMC_CommandToString).
 */
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aimc_local.h"

LOGMM_MODULE_DEFINE(com_imc);

/* registered notification (callback action), on imc->noti_list */
struct imc_noti {                 /* 0x18 */
    struct link list;
    unsigned int repeat;
    PFN_ImcNoti noti;
    void *param;
    unsigned int action_id;
};

/* pending response, on imc->resp_list */
struct imc_resp {                 /* 0x20 */
    struct link list;
    unsigned long long requested; /* never filled in; only logged */
    PFN_ImcResponse callback;
    void *callback_param;
    unsigned int id;
};

/* action registered for an (event, owner) key */
struct imc_action {               /* 0x20 */
    ImcActionParameter ap;
    unsigned int action_id;
};

#define IMC_MAX_ACTIONS 50

/* RBTree info for one event key = (event << 8) | (owner & 0xff) */
struct imc_event_actions {        /* 0x64c */
    unsigned int unknown_0[2];
    struct imc_action action[IMC_MAX_ACTIONS];
    int count;
};

/* named event, on imc->event_list */
struct imc_named_event {          /* 0x40 */
    struct link list;
    int id;
    unsigned int from;            /* remote registrar processor, 0 = local callback */
    int once;
    unsigned int uid;             /* module that raises it */
    char name[32];
    int (*callback)(void *arg, void *param, unsigned int size);
    void *callback_arg;
};

/* one lock for noti/resp lists and the action tree, shared by all ImcInfo */
static pthread_mutex_t imc_lock = PTHREAD_MUTEX_INITIALIZER;

/* the list helpers below have no symbol in the binary: always inlined */
#define IMC_INLINE static inline __attribute__((always_inline))
static ImcInfo *local_imc;

void (*IMC_ModuleTriggerFunc)(unsigned int uid);

#define EVENT_KEY(event, owner)  (((owner) & 0xff) | ((event) << 8))

/* RBTree info destructor (0x20d78, no symbol) */
static void imc_free_actions(void *info)
{
    if (info)
        free(info);
}

/* @0x20d80: add_named_event (caller holds imc->lock) */
struct imc_named_event *add_named_event(ImcInfo *imc, int id, int once, const char *name,
                                        unsigned int uid, unsigned int from,
                                        int (*cb)(void *, void *, unsigned int), void *arg)
{
    struct imc_named_event *ev;

    LOGI(com_imc, 0x499, "add \"%s\" on %x. id %x from %d, %sonce\n",
         name, uid, id, from, once ? "" : "not ");

    ev = calloc(0x40, 1);
    if (!ev) {
        LOGE(com_imc, 0x49f, "no mem\n");
        return NULL;
    }
    ev->id = id;
    ev->from = from;
    ev->uid = uid;
    strncpy(ev->name, name, sizeof(ev->name) - 1);
    ev->once = once;
    ev->callback = cb;
    ev->callback_arg = arg;
    link_add(&imc->event_list, &ev->list);
    return ev;
}

/* @0x20e60: cold path of _IMC_InsertResponseToList (calloc failed) */
void imc_insert_response_nomem(void)
{
    LOGF(com_imc, log_level_error, "_IMC_InsertResponseToList", 0x1aa, "no mem\n");
    pthread_mutex_unlock(&imc_lock);
}

IMC_INLINE void imc_insert_response(ImcInfo *imc, PFN_ImcResponse cb, void *cb_param,
                                    unsigned int id)
{
    struct imc_resp *r;

    pthread_mutex_lock(&imc_lock);
    r = calloc(0x20, 1);
    if (!r) {
        imc_insert_response_nomem();
        return;
    }
    r->callback = cb;
    r->callback_param = cb_param;
    r->id = id;
    link_add(&imc->resp_list, &r->list);
    pthread_mutex_unlock(&imc_lock);
}

/* _IMC_FindAndClearResponse: unlink the response for id; returns it (caller frees) */
IMC_INLINE struct imc_resp *imc_take_response(ImcInfo *imc, unsigned int id)
{
    struct link *l;

    pthread_mutex_lock(&imc_lock);
    for_each_link(l, &imc->resp_list) {
        struct imc_resp *r = link_container(struct imc_resp, list, l);
        if (r->id == id) {
            link_del(&r->list);
            pthread_mutex_unlock(&imc_lock);
            return r;
        }
    }
    LOGF(com_imc, log_level_error, "_IMC_FindAndClearResponse", 0x270, "not found\n");
    pthread_mutex_unlock(&imc_lock);
    return NULL;
}

/*
 * _IMC_FindAndClearNotification: look up the notification registered under action_id,
 * remove it if it was a one-shot. Returns 0 and the callback, or ADEC_ERR_NOT_FOUND.
 */
IMC_INLINE int imc_take_notification(ImcInfo *imc, unsigned int action_id, PFN_ImcNoti *noti,
                                     void **param)
{
    struct link *l;

    pthread_mutex_lock(&imc_lock);
    for_each_link(l, &imc->noti_list) {
        struct imc_noti *n = link_container(struct imc_noti, list, l);
        if (n->action_id != action_id)
            continue;
        *noti = n->noti;
        *param = n->param;
        if (n->repeat == IMC_ACTION_ONCE) {
            link_del(&n->list);
            free(n);
        }
        pthread_mutex_unlock(&imc_lock);
        return ADEC_ERR_NONE;
    }
    LOGF(com_imc, log_level_error, "_IMC_FindAndClearNotification", 0x291, "not found\n");
    pthread_mutex_unlock(&imc_lock);
    return ADEC_ERR_NOT_FOUND;
}

/* _IMC_DeleteActionFromList */
IMC_INLINE int imc_delete_action(struct imc_event_actions *ea, unsigned int action_id)
{
    int i, ret = ADEC_ERR_NOT_FOUND;

    pthread_mutex_lock(&imc_lock);
    if (ea->count >= IMC_MAX_ACTIONS) {
        LOGF(com_imc, log_level_error, "_IMC_DeleteActionFromList", 0x229,
             "Action count is too big. Struct seems like to be broken.\n");
        pthread_mutex_unlock(&imc_lock);
        return ADEC_ERR_CORRUPTED;
    }
    for (i = 0; i < ea->count; i++) {
        struct imc_action *a = &ea->action[i];
        if (a->action_id != action_id)
            continue;
        if (a->ap.action_type == IMC_ACTION_EXECUTE_CMD && a->ap.u.cmd.param)
            free(a->ap.u.cmd.param);
        ea->count--;
        if (i < ea->count)
            memcpy(&ea->action[i], &ea->action[i + 1], (ea->count - i) * sizeof(*a));
        ret = ADEC_ERR_NONE;
        break;
    }
    pthread_mutex_unlock(&imc_lock);
    return ret;
}

/* store an action for key (tree node must exist); used by register and process_register */
IMC_INLINE int imc_store_action(struct imc_event_actions *ea, unsigned int action_id,
                                const ImcActionParameter *ap)
{
    struct imc_action *a;

    pthread_mutex_lock(&imc_lock);
    if (ea->count >= IMC_MAX_ACTIONS)
        ea->count = 0;                    /* silently recycle the table */
    a = &ea->action[ea->count];
    a->ap = *ap;
    a->action_id = action_id;
    if (ap->action_type == IMC_ACTION_EXECUTE_CMD) {
        if (ap->u.cmd.param && ap->u.cmd.param_len) {
            void *copy = malloc(ap->u.cmd.param_len);
            if (!copy) {
                pthread_mutex_unlock(&imc_lock);
                return ADEC_ERR_NOT_ENOUGH_MEM;
            }
            memcpy(copy, ap->u.cmd.param, ap->u.cmd.param_len);
            a->ap.u.cmd.param = copy;
        } else {
            a->ap.u.cmd.param = NULL;
            a->ap.u.cmd.param_len = 0;
        }
    } else if ((unsigned int)ap->u.noti.level - 1 < 1000) {
        a->ap.u.noti.level = a->ap.u.noti.level_cnt = ap->u.noti.level;
    } else {
        a->ap.u.noti.level = a->ap.u.noti.level_cnt = 1;
    }
    ea->count++;
    pthread_mutex_unlock(&imc_lock);
    return ADEC_ERR_NONE;
}

/* @0x20eb4 */
void IMC_Terminate(void *imc)
{
    if (imc)
        free(imc);
}

/* @0x20ebc */
int IMC_GetStructSize(void)
{
    return sizeof(ImcInfo);
}

/* @0x20ec0 */
int IMC_Init(void *_imc, unsigned int core)
{
    ImcInfo *imc = _imc;
    int ret;

    if (!imc) {
        LOGE(aud, 0x30a, "assert failed. (%s)\n", "_imc != NULL");
        return ADEC_ERR_NULL;
    }
    memset(imc, 0, sizeof(*imc));
    if (core == 0)
        core = ModuleList_GetPlatform();
    imc->core = core;
    ret = RBTree_CreateTree(&imc->event_tree, imc_free_actions);
    LINK_INIT(&imc->resp_list);
    imc->unread_counter = 0;
    LINK_INIT(&imc->noti_list);
    LINK_INIT(&imc->event_list);
    pthread_mutex_init(&imc->lock, NULL);
    return ret;
}

/* @0x20f60 */
int IMC_Create(void **imc, unsigned int core)
{
    *imc = malloc(sizeof(ImcInfo));
    if (!*imc)
        return ADEC_ERR_NOT_ENOUGH_MEM;
    return IMC_Init(*imc, core);
}

static void free_all(struct link *root)
{
    while (!link_empty(root)) {
        struct link *l = root->next;
        link_del(l);
        free(l);
    }
}

/* @0x20f80 */
int IMC_Finalize(void *_imc)
{
    ImcInfo *imc = _imc;

    if (!imc) {
        LOGE(aud, 0x326, "assert failed. (%s)\n", "_imc != NULL");
        return ADEC_ERR_NULL;
    }
    free_all(&imc->noti_list);
    free_all(&imc->resp_list);
    free_all(&imc->event_list);
    return RBTree_DeleteTree(imc->event_tree);
}

/* @0x21034: send cmd locally (direct dispatch) or to target's processor */
int IMC_SendCommand(void *_imc, ImcCommandParameter *cp, unsigned int *response_id)
{
    ImcInfo *imc = _imc;
    unsigned int proc, id;
    IpcHeader h;

    if (!imc) {
        LOGE(aud, 0x34e, "assert failed. (%s)\n", "_imc != NULL");
        return ADEC_ERR_NULL;
    }
    if (!cp) {
        LOGE(aud, 0x34f, "assert failed. (%s)\n", "_imcCommandParam != NULL");
        return ADEC_ERR_NULL;
    }

    proc = ModuleList_FindModule(cp->target);
    if (proc == imc->core) {
        id = imc->cmd_index++ & 0xffffff;
        if (response_id)
            *response_id = id;
        if (cp->request_response)
            imc_insert_response(imc, cp->callback, cp->callback_param, id);
        aimc_dispatch_command(imc, cp->target, cp->cmd, cp->param, cp->param_len, cp->source,
                              id, cp->request_response ? AIMC_DISPATCH_FLAG_REQUIRE_RESPONSE : 0);
        return ADEC_ERR_NONE;
    }

    id = imc->cmd_index++ & 0xffffff;
    *response_id = id;
    h.cmd = cp->cmd;
    h.identifier = id;
    h.sender = cp->source;
    h.action = IPC_ACTION_COMMAND;
    h.option = cp->request_response & 1;
    h.target = cp->target;
    h.ext = 0;                               /* left uninitialised in the original */
    h.param_len = cp->param_len;
    if (cp->request_response)
        imc_insert_response(imc, cp->callback, cp->callback_param, id);
    return aimc_ipc_send(imc, proc, &h, cp->param);
}

/* @0x211fc: answer a command (locally: run the waiting callback) */
int IMC_Response(void *_imc, ImcResponseParameter *rp)
{
    ImcInfo *imc = _imc;
    unsigned int proc;
    IpcHeader h;

    if (!imc) {
        LOGE(aud, 0x36c, "assert failed. (%s)\n", "_imc != NULL");
        return ADEC_ERR_NULL;
    }

    proc = ModuleList_FindModule(rp->target.module);
    if (proc == imc->core) {
        struct imc_resp *r = imc_take_response(imc, rp->target.id);
        PFN_ImcResponse cb;
        void *cb_param;

        if (!r) {
            LOGE(com_imc, 0x376, "Fail to find response (%d)\n", ADEC_ERR_NOT_FOUND);
            return ADEC_ERR_NOT_FOUND;
        }
        cb = r->callback;
        cb_param = r->callback_param;
        LOGD(com_imc, 0x37b, "Response requeseted 0x%016llX, posted 0x%016llX\n",
             r->requested, 0ULL);
        free(r);
        if (cb)
            cb(rp->result, rp->param, rp->param_len, cb_param);
        return ADEC_ERR_NONE;
    }

    /* the result travels in the option field */
    h.cmd = rp->cmd;
    h.identifier = rp->target.id;
    h.sender = rp->sender;
    h.action = IPC_ACTION_RESPONSE;
    h.option = rp->result;
    h.target = rp->target.module;
    h.ext = 0;
    h.param_len = rp->param_len;
    if (rp->param_len >= ADEC_CNST_IPC_MAX_PARAM_SIZE - 4) {
        LOGE(aud, 0x391, "assert failed. (%s)\n",
             "_ImcResponseParameter->paramLen < ADEC_CNST_IPC_MAX_PARAM_SIZE - 4");
        return ADEC_ERR_DATA_SIZE;
    }
    return aimc_ipc_send(imc, proc, &h, rp->param);
}

/* @0x213e0 */
int IMC_RegisterCmdProcessor(ImcInfo *imc, unsigned int target, PFN_ImcCmdProc proc,
                             void *module)
{
    return -aimc__register_command_handler(imc, target, proc, module);
}

/* @0x213ec: attach an action (run a command / call back) to event raised by owner */
int IMC_RegisterEvent(void *_imc, unsigned int event, unsigned int owner,
                      unsigned int *action_id, ImcActionParameter *ap)
{
    ImcInfo *imc = _imc;
    unsigned int core, proc, key, id;
    struct imc_event_actions *ea;
    struct link *l;
    int ret;

    if (!imc) {
        LOGE(aud, 0x3b1, "assert failed. (%s)\n", "_imc != NULL");
        return ADEC_ERR_NULL;
    }
    if (!ap) {
        LOGE(aud, 0x3b2, "assert failed. (%s)\n", "_imcActionParam != NULL");
        return ADEC_ERR_NULL;
    }
    core = imc->core;
    proc = ModuleList_FindModule(owner);
    key = EVENT_KEY(event, owner);

    /* allocate an action id that is not in use by a pending notification */
    pthread_mutex_lock(&imc_lock);
retry:
    id = (imc->action_index++ & 0xfffff) | ((imc->core & 0xf) << 20);
    *action_id = id;
    for_each_link(l, &imc->noti_list)
        if (link_container(struct imc_noti, list, l)->action_id == id)
            goto retry;
    pthread_mutex_unlock(&imc_lock);

    if (ap->action_type == IMC_ACTION_GET_CALLBACK) {
        /* _IMC_InsertNotiToList */
        struct imc_noti *n;

        pthread_mutex_lock(&imc_lock);
        n = calloc(0x18, 1);
        if (!n) {
            LOGF(com_imc, log_level_error, "_IMC_InsertNotiToList", 0x1c4, "no mem\n");
            pthread_mutex_unlock(&imc_lock);
            LOGE(aud, 0x3d7, "assert failed. (%s)\n", "result == ADEC_ERR_NONE");
            return ADEC_ERR_NOT_ENOUGH_MEM;
        }
        n->action_id = *action_id;
        n->noti = ap->u.noti.noti;
        n->repeat = ap->repeat_type;
        n->param = ap->u.noti.param;
        link_add(&imc->noti_list, &n->list);
        pthread_mutex_unlock(&imc_lock);
    }
    LOGD(com_imc, 0x3da, "Send Register_Notification(%s) : eventID(%x), actionID(%x) \n",
         "USER", key, *action_id);
    id = *action_id;

    if (core != proc) {
        /* _IMC_RegisterRemoteAction: the owner's processor keeps the action */
        IpcHeader h;

        if (ap->action_type & ~1u) {
            LOGF(aud, log_level_error, "_IMC_RegisterRemoteAction", 0x159,
                 "assert failed. (%s)\n", "(_imcActionParam->actionType & (~1)) == 0");
            return ADEC_ERR_RANGE;
        }
        if (ap->repeat_type & ~1u) {
            LOGF(aud, log_level_error, "_IMC_RegisterRemoteAction", 0x15a,
                 "assert failed. (%s)\n", "(_imcActionParam->repeatType & (~1)) == 0");
            return ADEC_ERR_RANGE;
        }
        h.identifier = id;
        h.sender = ap->target;
        h.action = IPC_ACTION_REGISTER;
        h.option = ap->repeat_type << 2 | ap->action_type << 1;
        h.target = ap->u.cmd.param_len;      /* == u.noti.level for callbacks */
        h.ext = key;
        if (ap->action_type == IMC_ACTION_EXECUTE_CMD) {
            h.cmd = ap->u.cmd.cmd;
            h.param_len = ap->u.cmd.param_len;
            return aimc_ipc_send(imc, proc, &h, ap->u.cmd.param);
        }
        h.cmd = 0;
        h.param_len = 0;
        return aimc_ipc_send(imc, proc, &h, NULL);
    }

    /* _IMC_RegisterLocalAction */
    if (RBTree_Search(imc->event_tree, key, &ea) == ADEC_ERR_NOT_FOUND) {
        ea = calloc(sizeof(*ea), 1);
        if (!ea)
            return ADEC_ERR_NOT_ENOUGH_MEM;
        RBTree_CreateNode(imc->event_tree, key, ea);
    }
    ret = imc_store_action(ea, id, ap);
    LOGF(com_imc, log_level_debug, "_IMC_RegisterLocalAction", 0x149,
         "Receive Register_Notification(%s) : eventID(%xd), actionID(%x) \n", "USER", key, id);
    return ret;
}

/* @0x2184c */
int IMC_CancelEvent(void *_imc, unsigned int event, unsigned int owner, unsigned int action_id)
{
    ImcInfo *imc = _imc;
    struct imc_event_actions *ea;
    struct link *l;
    unsigned int key, proc;
    int ret;

    if (!imc) {
        LOGE(aud, 0x3f6, "assert failed. (%s)\n", "_imc != NULL");
        return ADEC_ERR_NULL;
    }
    key = EVENT_KEY(event, owner);

    pthread_mutex_lock(&imc_lock);
    for_each_link(l, &imc->noti_list) {
        struct imc_noti *n = link_container(struct imc_noti, list, l);
        if (n->action_id == action_id) {
            link_del(&n->list);
            free(n);
            goto unlock;
        }
    }
    LOGE(com_imc, 0x407, "not found\n");
unlock:
    pthread_mutex_unlock(&imc_lock);
    LOGD(com_imc, 0x40a, "Send Cancel_Notification(%s) : eventID(%x), actionID(%x) \n",
         "USER", key, action_id);

    proc = ModuleList_FindModule(owner);
    if (imc->core != proc) {
        IpcHeader h;

        memset(&h, 0, sizeof(h));
        h.cmd = 4;
        h.identifier = action_id;
        h.action = IPC_ACTION_CANCEL;
        h.ext = key;
        return aimc_ipc_send(imc, proc, &h, NULL);
    }

    /* _IMC_CancelLocalAction */
    ret = RBTree_Search(imc->event_tree, key, &ea);
    if (ret != ADEC_ERR_NONE) {
        if (ret != ADEC_ERR_NOT_FOUND)
            LOGF(aud, log_level_error, "_IMC_CancelLocalAction", 0x17d, "assert failed. (%s)\n",
                 "result == ADEC_ERR_NOT_FOUND");
        return ret;
    }
    ret = imc_delete_action(ea, action_id);
    if (ea->count == 0)
        ret = RBTree_Delete(imc->event_tree, key);
    LOGF(com_imc, log_level_debug, "_IMC_CancelLocalAction", 0x189,
         "Receive Cancel_Notification(%s) : eventID(%x), actionID(%x) \n", "USER", key, action_id);
    return ret;
}

/* @0x21ae8: run every action registered for (event, owner) */
int IMC_RaiseEvent(void *_imc, unsigned int event, unsigned int owner, ImcEventParameter *ep)
{
    ImcInfo *imc = _imc;
    struct imc_event_actions *ea = NULL;
    unsigned int key;
    int ret, i;

    if (!imc) {
        LOGE(aud, 0x42e, "assert failed. (%s)\n", "_imc != NULL");
        return ADEC_ERR_NULL;
    }
    key = EVENT_KEY(event, owner);
    ret = RBTree_Search(imc->event_tree, key, &ea);
    if (ret != ADEC_ERR_NONE || !ea)
        return ret;

    if (ea->count > IMC_MAX_ACTIONS) {
        LOGE(com_imc, 0x43d, "Action count is too big. Struct seems like to be broken.\n");
        for (;;)
            ;                                   /* the original hangs here on purpose */
    }

    ret = ADEC_ERR_NONE;
    for (i = 0; i < ea->count;) {
        struct imc_action *a = &ea->action[i];

        if (a->ap.action_type == IMC_ACTION_EXECUTE_CMD) {
            ImcCommandParameter cp = {
                .cmd = a->ap.u.cmd.cmd,
                .target = a->ap.target,
                .source = owner,
                .request_response = 0,
                .request_interrupt = 1,
                .param_len = a->ap.u.cmd.param_len,
                .param = a->ap.u.cmd.param,
            };
            unsigned int rid;

            ret = IMC_SendCommand(imc, &cp, &rid);
            if (ret != ADEC_ERR_NONE)
                LOGE(com_imc, 0x454, "IMC_SendCommand fail\n");
        } else if (a->ap.action_type == IMC_ACTION_GET_CALLBACK) {
            /* fire on the first raise, then every `level` raises */
            if (a->ap.u.noti.level == a->ap.u.noti.level_cnt) {
                unsigned int target = a->ap.target;
                unsigned int proc = ModuleList_FindModule(target);

                if (proc == imc->core) {
                    /* _IMC_Notification */
                    PFN_ImcNoti noti;
                    void *param;

                    if (imc_take_notification(imc, a->action_id, &noti, &param) == 0) {
                        if (noti)
                            noti(ep->param, ep->param_len, param);
                    } else {
                        LOGF(com_imc, log_level_error, "_IMC_Notification", 0x2c0,
                             "Fail to find notification (%d)\n", ADEC_ERR_NOT_FOUND);
                    }
                } else {
                    IpcHeader h = {
                        .cmd = 5,
                        .identifier = a->action_id,
                        .sender = owner,
                        .action = IPC_ACTION_NOTIFICATION,
                        .option = 0,
                        .target = target,
                        .ext = 0,
                        .param_len = ep->param_len,
                    };
                    aimc_ipc_send(imc, proc, &h, ep->param);
                }
            }
            if (--a->ap.u.noti.level_cnt == 0)
                a->ap.u.noti.level_cnt = a->ap.u.noti.level;
        } else {
            LOGE(com_imc, 0x469, "Unknown action type - %d\n", a->ap.action_type);
            for (;;)
                ;                               /* hangs, as in the original */
        }

        if (a->ap.repeat_type == IMC_ACTION_ONCE) {
            if (a->ap.action_type == IMC_ACTION_EXECUTE_CMD && a->ap.u.cmd.param)
                free(a->ap.u.cmd.param);
            ea->count--;
            if (ea->count <= i)
                break;
            memcpy(&ea->action[i], &ea->action[i + 1], (ea->count - i) * sizeof(*a));
        } else {
            i++;
        }
    }
    if (ea->count == 0)
        ret = RBTree_Delete(imc->event_tree, key);
    return ret;
}

/* @0x21dfc: named event raised by module uid; returns the event id or -1 */
int IMC_RegisterNamedEvent(ImcInfo *imc, const char *name, unsigned int uid, int once,
                           int (*cb)(void *arg, void *param, unsigned int size), void *arg)
{
    struct imc_named_event *ev;
    int id;

    pthread_mutex_lock(&imc->lock);
    id = imc->last_event_id + 1;
    if (id < 0)
        id = 1;
    imc->last_event_id = id;

    ev = add_named_event(imc, id, once, name, uid, 0, cb, arg);
    if (!ev) {
        pthread_mutex_unlock(&imc->lock);
        return -1;
    }
    if (uid >> 24 != imc->core) {
        /* let the raising processor know that we listen */
        IpcHeader h = {
            .cmd = 0,
            .identifier = 0,
            .sender = imc->core,
            .action = IPC_ACTION_NAMED_EVENT_REGISTER,
            .option = once != 0,
            .target = uid,
            .ext = id,
            .param_len = sizeof(ev->name),
        };
        aimc_ipc_send(imc, uid >> 24, &h, ev->name);
    }
    pthread_mutex_unlock(&imc->lock);
    return id;
}

/* @0x21e8c */
void IMC_UnregisterNamedEvent(ImcInfo *imc, int event_id)
{
    struct link *l;

    pthread_mutex_lock(&imc->lock);
    for_each_link(l, &imc->event_list) {
        struct imc_named_event *ev = link_container(struct imc_named_event, list, l);
        if (ev->id != event_id)
            continue;
        LOGI(com_imc, 0x501, "unregister event \"%s\" on %x.\n", ev->name, ev->uid);
        if (ev->uid >> 24 != imc->core) {
            IpcHeader h;

            memset(&h, 0, sizeof(h));
            h.action = IPC_ACTION_NAMED_EVENT_UNREGISTER;
            h.sender = imc->core;
            h.target = ev->uid;
            h.ext = event_id;
            aimc_ipc_send(imc, ev->uid >> 24, &h, NULL);
        }
        link_del(&ev->list);
        free(ev);
        pthread_mutex_unlock(&imc->lock);
        return;
    }
    LOGE(com_imc, 0x4fc, "unknown event_id. %x\n", event_id);
    pthread_mutex_unlock(&imc->lock);
}

/* @0x21f84 */
void IMC_UnregisterNamedEvent_no_ipc(ImcInfo *imc, int event_id)
{
    struct link *l;

    pthread_mutex_lock(&imc->lock);
    for_each_link(l, &imc->event_list) {
        struct imc_named_event *ev = link_container(struct imc_named_event, list, l);
        if (ev->id != event_id)
            continue;
        LOGI(com_imc, 0x52e, "unregister event \"%s\" on %x.\n", ev->name, ev->uid);
        link_del(&ev->list);
        free(ev);
        pthread_mutex_unlock(&imc->lock);
        return;
    }
    LOGE(com_imc, 0x529, "unknown event_id. %x\n", event_id);
    pthread_mutex_unlock(&imc->lock);
}

/* @0x22040: drop every named event raised by uid (module destroyed) */
void IMC_UnregisterNamedEvent_onUID(ImcInfo *imc, unsigned int uid)
{
    struct link *l;

    pthread_mutex_lock(&imc->lock);
restart:
    for_each_link(l, &imc->event_list) {
        struct imc_named_event *ev = link_container(struct imc_named_event, list, l);
        if (ev->uid != uid)
            continue;
        LOGF(com_imc, log_level_info, "UnregisterNamedEvent_onUID", 0x54e,
             "unregister event \"%s\" on %x.\n", ev->name, uid);
        link_del(&ev->list);
        free(ev);
        goto restart;
    }
    /* printed unconditionally once the list is clean */
    LOGF(com_imc, log_level_info, "UnregisterNamedEvent_onUID", 0x54a,
         "no named event on uid. %x\n", uid);
    pthread_mutex_unlock(&imc->lock);
}

/* @0x22118 */
int IMC_RaiseNamedEvent(ImcInfo *imc, unsigned int source, const char *name,
                        void *param, unsigned int size)
{
    struct link *l, *next;

    pthread_mutex_lock(&imc->lock);
    LOGD(com_imc, 0x582, "raise event %s on %x\n", name, source);
    for (l = imc->event_list.next, next = l->next; l != &imc->event_list;
         l = next, next = l->next) {
        struct imc_named_event *ev = link_container(struct imc_named_event, list, l);

        if (ev->uid != source || strcmp(ev->name, name) != 0)
            continue;
        if (ev->from == 0) {
            LOGI(com_imc, 0x58c, "call event callback function %p, event \"%s\" on %x\n",
                 ev->callback, name, source);
            ev->callback(ev->callback_arg, param, size);
        } else {
            IpcHeader h;

            memset(&h, 0, sizeof(h));
            LOGI(com_imc, 0x594, "send IPC for event \"%s\" on %x to %d\n", name, source,
                 ev->from);
            h.sender = source;
            h.action = IPC_ACTION_NAMED_EVENT_HANDLE;
            h.target = ev->from << 24;
            h.ext = ev->id;
            h.param_len = size;
            aimc_ipc_send(imc, ev->from, &h, param);
        }
        if (ev->once) {
            link_del(&ev->list);
            free(ev);
        }
    }
    pthread_mutex_unlock(&imc->lock);
    return 0;
}

/* process_register: remote processor attaches an action to one of our events */
static int process_register(ImcInfo *imc, IpcHeader *h)
{
    ImcActionParameter ap;
    struct imc_event_actions *ea;
    unsigned int key = h->ext;
    int ret;

    memset(&ap, 0, sizeof(ap));
    ap.action_type = (h->option >> 1) & 1;
    ap.repeat_type = (h->option & 7) >> 2;
    ap.target = h->sender;
    if (ap.action_type == IMC_ACTION_EXECUTE_CMD) {
        ap.u.cmd.cmd = h->cmd;
        ap.u.cmd.param = h->param;
        ap.u.cmd.param_len = h->param_len;
    } else {
        ap.u.noti.level = h->target;
    }

    ret = RBTree_Search(imc->event_tree, key, &ea);
    if (ret == ADEC_ERR_NOT_FOUND) {
        ea = calloc(sizeof(*ea), 1);
        if (!ea)
            return ADEC_ERR_NOT_ENOUGH_MEM;
        ret = RBTree_CreateNode(imc->event_tree, key, ea);
        if (ret != ADEC_ERR_NONE) {
            LOGF(com_imc, log_level_error, "process_register", 0x661,
                 "Fail to insert event(0x%x) to tree with err %d\n", key, ret);
            free(ea);
            return ret;
        }
    }
    imc_store_action(ea, h->identifier, &ap);
    LOGF(com_imc, log_level_debug, "process_register", 0x66a,
         "Receive Register_Notification(%s) : eventID(%x), actionID(%x) \n", "USER", key,
         h->identifier);
    return ADEC_ERR_NONE;
}

static int process_cancel(ImcInfo *imc, IpcHeader *h)
{
    struct imc_event_actions *ea;
    unsigned int key = h->ext;
    int ret;

    ret = RBTree_Search(imc->event_tree, key, &ea);
    if (ret == ADEC_ERR_NOT_FOUND)
        return ret;
    if (ret != ADEC_ERR_NONE) {
        LOGF(aud, log_level_error, "process_cancel", 0x684, "assert failed. (%s)\n",
             "result == ADEC_ERR_NONE");
        return ret;
    }
    imc_delete_action(ea, h->identifier);
    if (ea->count == 0)
        RBTree_Delete(imc->event_tree, key);
    LOGF(com_imc, log_level_debug, "process_cancel", 0x68d,
         "Receive Cancel_Notification(%s) : eventID(%x), actionID(%x) \n", "USER", key,
         h->identifier);
    return ADEC_ERR_NONE;
}

/* @0x2229c: handle one IpcHeader received from another processor */
int IMC_ProcessHeader(ImcInfo *imc, IpcHeader *h)
{
    struct link *l;

    LOGT(com_imc, 0x697, "cmd %x, target %x, source %x\n", h->cmd, h->target, h->sender);

    switch (h->action) {
    case IPC_ACTION_COMMAND: {
        int r;

        LOGT(com_imc, 0x69b, "command\n");
        ModuleList_FindModule(h->sender);
        r = aimc_dispatch_command(imc, h->target, h->cmd, h->param, h->param_len, h->sender,
                                  h->identifier, h->option & 1);
        if ((r == -ADEC_ERR_NEED_MORE_PROCESS || h->cmd == 0x61010018 ||
             h->cmd == ADEC_CMD_START) && IMC_ModuleTriggerFunc)
            IMC_ModuleTriggerFunc(h->target);
        return -r;
    }

    case IPC_ACTION_RESPONSE: {
        struct imc_resp *r;
        PFN_ImcResponse cb;
        void *cb_param;

        LOGT(com_imc, 0x6a0, "response\n");
        r = imc_take_response(imc, h->identifier);
        if (!r) {
            LOGF(com_imc, log_level_error, "process_response", 0x607,
                 "Fail to find response (%d)\n", ADEC_ERR_NOT_FOUND);
            return ADEC_ERR_NOT_FOUND;
        }
        cb = r->callback;
        cb_param = r->callback_param;
        free(r);
        if (cb)
            cb(h->option, h->param, h->param_len, cb_param);
        return ADEC_ERR_NONE;
    }

    case IPC_ACTION_REGISTER:
        LOGT(com_imc, 0x6aa, "register\n");
        return process_register(imc, h);

    case IPC_ACTION_CANCEL:
        LOGT(com_imc, 0x6af, "cancel\n");
        return process_cancel(imc, h);

    case IPC_ACTION_NOTIFICATION: {
        PFN_ImcNoti noti;
        void *param;

        LOGT(com_imc, 0x6a5, "notification\n");
        if (imc_take_notification(imc, h->identifier, &noti, &param) != 0) {
            LOGF(com_imc, log_level_error, "process_notification", 0x620,
                 "Fail to find notification (%d)\n", ADEC_ERR_NOT_FOUND);
            return ADEC_ERR_NOT_FOUND;
        }
        if (!noti) {
            LOGF(com_imc, log_level_error, "process_notification", 0x62a,
                 "null noti function\n");
            return ADEC_ERR_NONE;
        }
        LOGF(com_imc, log_level_trace, "process_notification", 0x626, "call noti %ps\n", noti);
        noti(h->param, h->param_len, param);
        return ADEC_ERR_NONE;
    }

    case IPC_ACTION_NAMED_EVENT_REGISTER:
        LOGT(com_imc, 0x6b4, "named event register\n");
        pthread_mutex_lock(&imc->lock);
        add_named_event(imc, h->ext, h->option & 1, (const char *)h->param, h->target,
                        h->sender, NULL, NULL);
        pthread_mutex_unlock(&imc->lock);
        return ADEC_ERR_NONE;

    case IPC_ACTION_NAMED_EVENT_UNREGISTER:
        LOGT(com_imc, 0x6ba, "named event unregister\n");
        pthread_mutex_lock(&imc->lock);
        for_each_link(l, &imc->event_list) {
            struct imc_named_event *ev = link_container(struct imc_named_event, list, l);
            if (ev->id == (int)h->ext && ev->from == h->sender) {
                link_del(&ev->list);
                free(ev);
                pthread_mutex_unlock(&imc->lock);
                return ADEC_ERR_NONE;
            }
        }
        LOGF(com_imc, log_level_error, "process_named_event_unregister", 0x56f,
             "unknown event id. %x from %d\n", h->ext, h->sender);
        pthread_mutex_unlock(&imc->lock);
        return 1;

    case IPC_ACTION_NAMED_EVENT_HANDLE:
        LOGT(com_imc, 0x6c0, "named event handle\n");
        pthread_mutex_lock(&imc->lock);
        for_each_link(l, &imc->event_list) {
            struct imc_named_event *ev = link_container(struct imc_named_event, list, l);
            if (ev->uid != h->sender || ev->id != (int)h->ext)
                continue;
            LOGF(com_imc, log_level_info, "process_named_event_handle", 0x5c8,
                 "call event callback function %p, event \"%s\" on %x\n",
                 ev->callback, ev->name, h->sender);
            ev->callback(ev->callback_arg, h->param, h->param_len);
            if (ev->once) {
                link_del(&ev->list);
                free(ev);
            }
            goto out;
        }
        LOGF(com_imc, log_level_error, "process_named_event_handle", 0x5c3,
             "unknown event id %x from %x\n", h->ext, h->sender);
out:
        pthread_mutex_unlock(&imc->lock);
        return ADEC_ERR_NONE;

    default:
        LOGE(com_imc, 0x6c6, "unknown IPC command. %x\n", h->action);
        return ADEC_ERR_NOT_SUPPORT;
    }
}

/* @0x22bcc */
int IMC_SetLocalImc(void *imc)
{
    local_imc = imc;
    return 0;
}

/* @0x22bdc */
ImcInfo *IMC_GetLocalImc(int create_if_missing)
{
    if (!local_imc && create_if_missing)
        IMC_Create((void **)&local_imc, ModuleList_GetPlatform());
    return local_imc;
}

/* @0x22c0c */
void IMC_SetImcTracer(void *imc, void *tracer)
{
    if (imc)
        ((ImcInfo *)imc)->tracer = tracer;
}

/* @0x22c14 */
int IMC_GetIPCUnreadCounter(void *imc)
{
    return imc ? ((ImcInfo *)imc)->unread_counter : 0;
}

struct cmd_name {
    unsigned int key;
    const char *name;
};

/* @0x22c1c: generic commands by full id, module commands by (cmd >> 24) */
const char *IMC_CommandToString(unsigned int cmd)
{
    static const struct cmd_name generic[] = {
        { ADEC_CMD_INIT, "ADEC_CMD_INIT" },
        { ADEC_CMD_START, "ADEC_CMD_START" },
        { ADEC_CMD_STOP, "ADEC_CMD_STOP" },
        { ADEC_CMD_FLUSH, "ADEC_CMD_FLUSH" },
        { ADEC_CMD_GET_DECINFO, "ADEC_CMD_GET_DECINFO" },
        { ADEC_CMD_SET_OUTMODE, "ADEC_CMD_SET_OUTMODE" },
        { ADEC_CMD_SET_DEBUGCNT, "ADEC_CMD_SET_DEBUGCNT" },
        { ADEC_CMD_MODULE_STATUS, "ADEC_CMD_MODULE_STATUS" },
        { ADEC_CMD_SET_DEBUGTIME, "ADEC_CMD_SET_DEBUGTIME" },
        { ADEC_CMD_RESET_PROCESSING_CNT, "ADEC_CMD_RESET_PROCESSING_CNT" },
        { ADEC_CMD_REPORT_PROCESSING_CNT, "ADEC_CMD_REPORT_PROCESSING_CNT" },
        { ADEC_CMD_SET_BYPASS, "ADEC_CMD_SET_BYPASS" },
        { ADEC_CMD_SHOW_METADATA, "ADEC_CMD_SHOW_METADATA" },
        { ADEC_CMD_CONNECT_METADATA, "ADEC_CMD_CONNECT_METADATA" },
        { ADEC_CMD_DISCONNECT_METADATA, "ADEC_CMD_DISCONNECT_METADATA" },
        { ADEC_CMD_GET_PROP, "ADEC_CMD_GET_PROP" },
        { ADEC_CMD_SET_PROP, "ADEC_CMD_SET_PROP" },
        { ADEC_CMD_ASYNC_EVENT, "ADEC_CMD_ASYNC_EVENT" },
        { ADEC_CMD_SYNC, "ADEC_CMD_SYNC" },
        { ADEC_CMD_SET_INDEX, "ADEC_CMD_SET_INDEX" },
        { ADEC_CMD_SET_PROP_SINGLE, "ADEC_CMD_SET_PROP_SINGLE" },
        { ADEC_CMD_SET_BASETIME, "ADEC_CMD_SET_BASETIME" },
    };
    static const struct cmd_name module[] = {
        { 0x20, "CMD_PCMDEC" }, { 0x21, "CMD_DDC" }, { 0x22, "CMD_DDT" },
        { 0x23, "CMD_DRA" }, { 0x24, "CMD_MP3" }, { 0x25, "CMD_WMA" },
        { 0x26, "CMD_DTS" }, { 0x27, "CMD_DTS_X" }, { 0x28, "CMD_OGG" },
        { 0x29, "CMD_AMRWB" }, { 0x2a, "CMD_AMRNB" }, { 0x2b, "CMD_RA8" },
        { 0x2c, "CMD_FLAC" }, { 0x2d, "CMD_AACENC" }, { 0x2e, "CMD_AC3ENC" },
        { 0x2f, "CMD_DTSENC" }, { 0x30, "CMD_MP3ENC" }, { 0x31, "CMD_MPEGH" },
        { 0x32, "CMD_AC4" }, { 0x33, "CMD_MAT" }, { 0x34, "CMD_AACELD" },
        { 0x40, "CMD_SIF" }, { 0x41, "CMD_AAD" }, { 0x42, "CMD_ADC" },
        { 0x43, "CMD_HDMI" }, { 0x44, "CMD_I2S" }, { 0x45, "CMD_TP" },
        { 0x46, "CMD_REFORM" }, { 0x47, "CMD_BYPASS" }, { 0x50, "CMD_PCMCVT" },
        { 0x51, "CMD_SOLA" }, { 0x52, "CMD_LIPSYNC" }, { 0x53, "CMD_INPUT_CTRL" },
        { 0x54, "CMD_MIXER" }, { 0x55, "CMD_DAP" }, { 0x56, "CMD_SE" },
        { 0x57, "CMD_OUT_CTRL" }, { 0x58, "CMD_ES_OUT_CTRL" }, { 0x59, "CMD_PCMOUT" },
        { 0x5a, "CMD_SPDIF_OUT" }, { 0x5b, "CMD_CTRL_SRC" }, { 0x5c, "CMD_DTO_CTRL" },
        { 0x5d, "CMD_RTS" }, { 0x5e, "CMD_TIMER" }, { 0x5f, "CMD_DEBUG" },
        { 0x60, "CMD_ARC_OUT" }, { 0x61, "CMD_MSMIXER" }, { 0x62, "CMD_DDPENC" },
        { 0x63, "CMD_MSSRC" }, { 0x64, "CMD_PCMREN" }, { 0x65, "CMD_DTSVIRTUALX" },
        { 0x66, "CMD_GAIN" }, { 0x67, "CMD_OPUS" }, { 0x80, "CMD_MANAGER" },
        { 0x90, "CMD_WIN_SINGEN" }, { 0x91, "CMD_WIN_PCMOUT" }, { 0x92, "CMD_WIN_FILEDUMP" },
    };
    size_t i;

    for (i = 0; i < sizeof(generic) / sizeof(generic[0]); i++)
        if (generic[i].key == cmd)
            return generic[i].name;
    for (i = 0; i < sizeof(module) / sizeof(module[0]); i++)
        if (module[i].key == cmd >> 24)
            return module[i].name;
    return "UNKNOWN";
}

/* @0x22ca0: printf-style property string ("key=value\key=value") sent as
 * ADEC_CMD_SET_PROP {int len; char str[len+1]} to module `target`. Fire and forget:
 * no response is requested and the IMC_SendCommand result is ignored.
 */
int aimc_set_property(struct aimc *imc, unsigned int target, const char *prop_fmt, ...)
{
    ImcCmdSetProp *prop;
    va_list ap, ap2;
    int len;

    va_start(ap, prop_fmt);
    va_copy(ap2, ap);
    len = vsnprintf(NULL, 0, prop_fmt, ap);
    prop = malloc(len + 5);
    if (!prop) {
        LOGE(aimc, 0x22, "no mem. %s\n", prop_fmt);
        va_end(ap2);
        va_end(ap);
        return -1;
    }
    prop->len = len;
    vsnprintf(prop->str, len + 1, prop_fmt, ap2);
    LOGI(aimc, 0x2c, "'%08x' len %d, prop '%s'\n", target, len, prop->str);
    aimc_send_command(imc, ADEC_CMD_SET_PROP, 0, target, prop, len + 5);
    free(prop);
    va_end(ap2);
    va_end(ap);
    return 0;
}
