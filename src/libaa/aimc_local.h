/*
 * libaa IMC/IPC layer: private types shared by aimc.c, aipc.c and imc.c.
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 *
 * Layouts were recovered from the binary (Ghidra addresses):
 *   struct aimc      aimc_create @0x1cd94 (calloc 0x158), aimc_dispatch_command @0x1dd84
 *   ImcInfo          IMC_Init @0x20ec0 (memset 0x4c, IMC_GetStructSize @0x20ebc)
 *   IpcHeader        aimc_ipc_send @0x1d920 (0x20-byte header + payload)
 *   struct aipc      aipc _create @0x1ec0c (calloc 0x58), aipc_get_pollfd @0x1f8cc
 *   auimc ioctls     /dev/lg/audio_uimc request codes ('M', 1..20) used in aimc.c
 * Field names follow LG's (older) kdriver headers where they still fit.
 */
#ifndef LIBAA_AIMC_LOCAL_H
#define LIBAA_AIMC_LOCAL_H

#include <poll.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/ioctl.h>

#include "types.h"

/* ADEC_ERR_* (enum adec_result) and LOGF come from types.h */

#define AUD_ASSERT_LOG(line, expr) \
    LOGE(aud, line, "assert failed. (%s)\n", #expr)

/* ------------------------------------------------------------------------- */
/* IMC commands: cmd = (tag << 16) | sizeof(param)                            */
/* ------------------------------------------------------------------------- */
#define ADEC_CMD(id, size)              ((((unsigned int)(id)) << 16) | ((size) & 0xffff))
#define ADEC_CMD_INIT                   ADEC_CMD(0x1000, 0)
#define ADEC_CMD_START                  ADEC_CMD(0x1001, 0)
#define ADEC_CMD_STOP                   ADEC_CMD(0x1002, 0)
#define ADEC_CMD_FLUSH                  ADEC_CMD(0x1003, 0x2c)
#define ADEC_CMD_GET_DECINFO            ADEC_CMD(0x1004, 0)
#define ADEC_CMD_SET_OUTMODE            ADEC_CMD(0x1005, 4)
#define ADEC_CMD_SET_DEBUGCNT           ADEC_CMD(0x1006, 4)
#define ADEC_CMD_MODULE_STATUS          ADEC_CMD(0x1007, 0)
#define ADEC_CMD_SET_DEBUGTIME          ADEC_CMD(0x1008, 4)
#define ADEC_CMD_RESET_PROCESSING_CNT   ADEC_CMD(0x1011, 0)
#define ADEC_CMD_REPORT_PROCESSING_CNT  ADEC_CMD(0x1012, 0)
#define ADEC_CMD_SET_BYPASS             ADEC_CMD(0x1013, 4)
#define ADEC_CMD_SHOW_METADATA          ADEC_CMD(0x1014, 0)
#define ADEC_CMD_CONNECT_METADATA       ADEC_CMD(0x1015, 0)
#define ADEC_CMD_DISCONNECT_METADATA    ADEC_CMD(0x1016, 0x2c)
#define ADEC_CMD_GET_PROP               ADEC_CMD(0x1017, 0x20)   /* char name[32] */
#define ADEC_CMD_SET_PROP               ADEC_CMD(0x1018, 4)      /* 0x10180004 */
#define ADEC_CMD_ASYNC_EVENT            ADEC_CMD(0x1019, 0)
#define ADEC_CMD_SYNC                   ADEC_CMD(0x101a, 0)
#define ADEC_CMD_SET_INDEX              ADEC_CMD(0x101b, 4)
#define ADEC_CMD_SET_PROP_SINGLE        ADEC_CMD(0x101c, 4)
#define ADEC_CMD_SET_BASETIME           ADEC_CMD(0x1020, 8)

/* ADEC_CMD_SET_PROP payload: "key=value\key=value..." (receiver: aud_module_set_prop) */
typedef struct ImcCmdSetProp {
    int len;                     /* strlen(str), without NUL */
    char str[];                  /* NUL terminated */
} ImcCmdSetProp;

/* ------------------------------------------------------------------------- */
/* IMC parameter blocks                                                       */
/* ------------------------------------------------------------------------- */
typedef void (*PFN_ImcResponse)(int result, void *param, int param_len, void *cb_param);
typedef void (*PFN_ImcNoti)(void *param, int param_len, void *cb_param);

typedef struct ImcSenderInfo {
    unsigned int module;         /* sender uid */
    unsigned int core;
    unsigned int request_response;
    unsigned int id;             /* message id */
} ImcSenderInfo;

/* legacy (DSP-style) command processor, registered through aimc__register_command_handler */
typedef int (*PFN_ImcCmdProc)(void *module, unsigned int cmd, ImcSenderInfo *sender,
                              void *param, unsigned int param_len);
/* aimc_register_command_handler() style */
typedef int (*aimc_cmd_handler_fn)(void *arg, unsigned int cmd, void *param,
                                   unsigned int param_len);

typedef struct ImcCommandParameter {          /* 0x24 */
    unsigned int cmd;
    unsigned int target;
    unsigned int source;
    unsigned int request_response;
    unsigned int request_interrupt;
    PFN_ImcResponse callback;
    void *callback_param;
    unsigned int param_len;
    void *param;
} ImcCommandParameter;

typedef struct ImcResponseParameter {         /* 0x28 */
    ImcSenderInfo target;                     /* whom to answer */
    unsigned int sender;
    unsigned int request_interrupt;
    unsigned int cmd;
    int result;
    void *param;
    unsigned int param_len;
} ImcResponseParameter;

enum { IMC_ACTION_EXECUTE_CMD, IMC_ACTION_GET_CALLBACK };
enum { IMC_ACTION_ONCE, IMC_ACTION_REPEAT };

typedef struct ImcActionParameter {           /* 0x1c */
    unsigned int action_type;
    unsigned int repeat_type;
    unsigned int target;
    union {
        struct { void *param; unsigned int cmd; unsigned int param_len; } cmd;
        struct { PFN_ImcNoti noti; void *param; int level; int level_cnt; } noti;
    } u;
} ImcActionParameter;

typedef struct ImcEventParameter {
    unsigned int request_interrupt;
    void *param;
    unsigned int param_len;
} ImcEventParameter;

/* ------------------------------------------------------------------------- */
/* wire header for every inter-processor IMC message (kernel uimc or aipc)    */
/* ------------------------------------------------------------------------- */
enum ipc_action {
    IPC_ACTION_COMMAND = 1,
    IPC_ACTION_RESPONSE,
    IPC_ACTION_REGISTER,
    IPC_ACTION_CANCEL,
    IPC_ACTION_NOTIFICATION,
    IPC_ACTION_NAMED_EVENT_REGISTER,
    IPC_ACTION_NAMED_EVENT_UNREGISTER,
    IPC_ACTION_NAMED_EVENT_HANDLE,
};

typedef struct IpcHeader {                    /* 0x20 + payload */
    unsigned int cmd;
    unsigned int identifier;                  /* message / action / event id */
    unsigned int sender;
    unsigned int action;                      /* enum ipc_action */
    unsigned int option;                      /* bit0 response/once, bit1 type, bit2 repeat */
    unsigned int target;
    unsigned int ext;                         /* event key / named event id */
    unsigned int param_len;
    unsigned char param[];
} IpcHeader;

#define ADEC_CNST_IPC_MAX_PARAM_SIZE  0xfdc

/* ------------------------------------------------------------------------- */
/* per-processor IMC state (IMC_GetStructSize() == 0x4c)                      */
/* ------------------------------------------------------------------------- */
typedef struct ImcInfo {
    struct link noti_list;        /* 0x00 struct imc_noti */
    struct link resp_list;        /* 0x08 struct imc_resp */
    pthread_mutex_t lock;         /* 0x10 protects event_list */
    struct link event_list;       /* 0x28 struct imc_named_event */
    int last_event_id;            /* 0x30 */
    void *event_tree;             /* 0x34 RBTree: key -> struct imc_event_actions */
    unsigned int cmd_index;       /* 0x38 message ids */
    unsigned int action_index;    /* 0x3c action ids */
    unsigned int unread_counter;  /* 0x40 */
    unsigned int core;            /* 0x44 processor id */
    void *tracer;                 /* 0x48 */
} ImcInfo;

/* ------------------------------------------------------------------------- */
/* struct aimc (0x158)                                                        */
/* ------------------------------------------------------------------------- */
struct aimc {
    int fd;                                   /* 0x000 /dev/lg/audio_uimc, -1 if ignored */
    char name[16];                            /* 0x004 "aimc_%d" */
    unsigned int processor_id;                /* 0x014 */
    pthread_mutex_t lock;                     /* 0x018 protects event_list */
    ImcInfo imc_info;                         /* 0x030 */
    volatile int running_handler;             /* 0x07c */
    struct aud_thread *handler_thread;        /* 0x080 */
    int handler_pipe[2];                      /* 0x084 */
    pthread_cond_t handler_sync_cond;         /* 0x090 */
    pthread_mutex_t handler_sync_lock;        /* 0x0c0 */
    IpcHeader *ipc_header;                    /* 0x0d8 read buffer for AUIMC_IO_IPC_READ */
    unsigned int ipc_param_length;            /* 0x0dc */
    struct link command_list;                 /* 0x0e0 struct command_handler */
    pthread_mutex_t command_lock;             /* 0x0e8 */
    bool command_need_response;               /* 0x100 */
    ImcResponseParameter command_response;    /* 0x104 */
    struct link event_list;                   /* 0x12c struct aimc_event */
    struct aipc *aipc;                        /* 0x134 socket IPC (aimc_use_aipc) */
    struct adebug *adbg;                      /* 0x138 */
    pthread_mutex_t imc_process_lock;         /* 0x13c */
    int unknown_154;
};

#define aimc_of_info(info)  link_container(struct aimc, imc_info, (info))

_Static_assert(offsetof(struct aimc, imc_info) == 0x30, "aimc.imc_info");
_Static_assert(offsetof(struct aimc, handler_sync_cond) == 0x90, "aimc.cond");
_Static_assert(offsetof(struct aimc, command_list) == 0xe0, "aimc.command_list");
_Static_assert(offsetof(struct aimc, command_response) == 0x104, "aimc.command_response");
_Static_assert(offsetof(struct aimc, aipc) == 0x134, "aimc.aipc");
_Static_assert(sizeof(struct aimc) == 0x158, "aimc size");
_Static_assert(sizeof(ImcInfo) == 0x4c, "ImcInfo size");

/* ------------------------------------------------------------------------- */
/* /dev/lg/audio_uimc ioctl ABI                                               */
/* ------------------------------------------------------------------------- */
struct auimc_ipc_header {
    int timeout;
    uint32_t param_length;
    union { IpcHeader *header; uint64_t compat_sizer; };
};
struct auimc_info { int proc_id; };
struct auimc_module {
    char name[32];
    int preferred_processor;
    unsigned int uid;                         /* out */
};
struct auimc_connect {
    unsigned int out_uid; int out_port;
    unsigned int in_uid;  int in_port;
};
struct auimc_disconnect { unsigned int uid; int port; };
struct auimc_implement { char name[32]; unsigned int rank; };
struct auimc_implement_port_buf_def {
    char name[32];
    int def_num, max_num_au, max_access_size;
};
struct auimc_role { char name[32]; unsigned int uid; };
struct auimc_cache {
    unsigned int direction;                   /* 0 from device, 1 to device */
    unsigned int shbuf_offset, offset, size;
};

#define AUIMC_IOC_MAGIC                   'M'
#define AUIMC_IO_GET_INFO                 _IOR(AUIMC_IOC_MAGIC, 1, struct auimc_info)
#define AUIMC_IO_IPC_WRITE                _IOW(AUIMC_IOC_MAGIC, 2, struct auimc_ipc_header)
#define AUIMC_IO_IPC_READ                 _IOW(AUIMC_IOC_MAGIC, 3, struct auimc_ipc_header)
#define AUIMC_IO_ADD_IMPL                 _IOW(AUIMC_IOC_MAGIC, 4, struct auimc_implement)
#define AUIMC_IO_ADD_IMPL_PORT_BUF_DEF    _IOW(AUIMC_IOC_MAGIC, 5, struct auimc_implement_port_buf_def)
#define AUIMC_IO_CREATE_MODULE            _IOWR(AUIMC_IOC_MAGIC, 6, struct auimc_module)
#define AUIMC_IO_DESTROY_MODULE           _IOW(AUIMC_IOC_MAGIC, 7, unsigned int)
#define AUIMC_IO_CONNECT_MODULE           _IOW(AUIMC_IOC_MAGIC, 8, struct auimc_connect)
#define AUIMC_IO_CREATE_LEGACY_MODULE     _IOWR(AUIMC_IOC_MAGIC, 9, unsigned int)
#define AUIMC_IO_DESTROY_LEGACY_MODULE    _IOWR(AUIMC_IOC_MAGIC, 10, unsigned int)
#define AUIMC_IO_SET_ROLE                 _IOW(AUIMC_IOC_MAGIC, 11, struct auimc_role)
#define AUIMC_IO_SEARCH_ROLE              _IOWR(AUIMC_IOC_MAGIC, 11, struct auimc_role)
#define AUIMC_IO_CACHE_SYNC               _IOW(AUIMC_IOC_MAGIC, 12, struct auimc_cache)
#define AUIMC_IO_MUNMAP                   _IOW(AUIMC_IOC_MAGIC, 13, unsigned int)
#define AUIMC_IO_DISCONNECT_MODULE_OUTPUT _IOW(AUIMC_IOC_MAGIC, 14, struct auimc_disconnect)
#define AUIMC_IO_DISCONNECT_MODULE_INPUT  _IOW(AUIMC_IOC_MAGIC, 15, struct auimc_disconnect)
#define AUIMC_IO_SET_PID                  _IOW(AUIMC_IOC_MAGIC, 20, unsigned int)

/* processors below this id are reached through the kernel driver, the rest via aipc */
#define AIMC_FIRST_AIPC_PROCESSOR 6

/* ------------------------------------------------------------------------- */
/* aipc: AF_UNIX SOCK_SEQPACKET RPC between Linux processes                   */
/* ------------------------------------------------------------------------- */
enum ipc_argtype {
    ipc_argtype_end = 0,
    ipc_argtype_bool,
    ipc_argtype_int,
    ipc_argtype_longlong,
    ipc_argtype_double,
    ipc_argtype_str,
    ipc_argtype_mem,
};

struct aipc_ipc_arg {                         /* 0x18 */
    enum ipc_argtype type;
    union {
        int b;
        int i;
        long long ll;
        double d;
        char *s;
        void *m;
    };
    unsigned int size;
};

typedef int (*aipc_command_fn)(void *arg, int cfd, const char *name, int nargs,
                               struct aipc_ipc_arg *args);

struct aipc_server {                          /* one outgoing connection */
    int fd;
    int proc_id;
    int refs;
};

struct aipc {                                 /* 0x58 */
    struct link command_list;                 /* 0x00 struct aipc_command */
    int proc_id;                              /* 0x08 */
    int listen_fd;                            /* 0x0c <base>.<proc_id> */
    int nclients;                             /* 0x10 */
    int *client_fds;                          /* 0x14 */
    struct link additional_list;              /* 0x18 struct aipc_additional */
    void *last_additional;                    /* 0x20 from the last aipc_call */
    unsigned int last_additional_size;        /* 0x24 */
    int npoll_servers;                        /* 0x28 servers in the last pollfd set */
    int nservers;                             /* 0x2c servers[0] is the controller */
    struct aipc_server *servers;              /* 0x30 */
    int (*cb_client_disconnect)(void *priv, int cfd); /* 0x34 */
    void *cb_priv;                            /* 0x38 */
    bool is_controller;                       /* 0x3c aipc_create0() */
    pthread_mutex_t call_lock;                /* 0x40 */
};

_Static_assert(sizeof(struct aipc) == 0x58, "aipc size");

/* request flags in the 12-byte message header */
#define AIPC_FLAG_NEED_RESPONSE   0x1
#define AIPC_FLAG_NO_ADDITIONAL   0x2

/* ------------------------------------------------------------------------- */
/* prototypes (exported by this layer)                                        */
/* ------------------------------------------------------------------------- */
struct aipc *aipc_create0(void);
struct aipc *aipc_create(void);
void aipc_destroy(struct aipc *ipc);
int aipc_send_to(struct aipc *ipc, int target, const char *name, int argc,
                 struct aipc_ipc_arg *args);
int aipc_senda(struct aipc *ipc, int target, const char *name, struct aipc_ipc_arg *args);
int aipc_sendv(struct aipc *ipc, int target, const char *name, va_list ap);
int aipc_send(struct aipc *ipc, int target, const char *name, ...);
int aipc_call_to(struct aipc *ipc, int target, int *call_ret, const char *name, int argc,
                 struct aipc_ipc_arg *args);
int aipc_calla(struct aipc *ipc, int target, int *call_ret, const char *name,
               struct aipc_ipc_arg *args);
int aipc_callv(struct aipc *ipc, int target, int *call_ret, const char *name, va_list ap);
int aipc_call(struct aipc *ipc, int target, int *call_ret, const char *name, ...);
int aipc_get_last_additional_data(struct aipc *ipc, void **data);
int aipc_register_cb(struct aipc *ipc, int (*cb)(void *priv, int cfd), void *priv);
int aipc_get_proc_id(struct aipc *ipc);
int aipc_get_pollfd(struct aipc *ipc, struct pollfd *fds);
int aipc_handle_pollfd(struct aipc *ipc, struct pollfd *fds);
int aipc_register_command(struct aipc *ipc, const char *name, aipc_command_fn func, void *arg);
int aipc_append_additional(struct aipc *ipc, void *data, unsigned int size);

int IMC_Create(void **imc, unsigned int core);
void IMC_Terminate(void *imc);
int IMC_GetStructSize(void);
int IMC_Init(void *imc, unsigned int core);
int IMC_Finalize(void *imc);
int IMC_SendCommand(void *imc, ImcCommandParameter *cp, unsigned int *response_id);
int IMC_Response(void *imc, ImcResponseParameter *rp);
int IMC_RegisterCmdProcessor(ImcInfo *imc, unsigned int target, PFN_ImcCmdProc proc, void *module);
int IMC_RegisterEvent(void *imc, unsigned int event, unsigned int owner,
                      unsigned int *action_id, ImcActionParameter *ap);
int IMC_CancelEvent(void *imc, unsigned int event, unsigned int owner, unsigned int action_id);
int IMC_RaiseEvent(void *imc, unsigned int event, unsigned int owner, ImcEventParameter *ep);
int IMC_RegisterNamedEvent(ImcInfo *imc, const char *name, unsigned int uid, int once,
                           int (*cb)(void *arg, void *param, unsigned int size), void *arg);
void IMC_UnregisterNamedEvent(ImcInfo *imc, int event_id);
void IMC_UnregisterNamedEvent_no_ipc(ImcInfo *imc, int event_id);
void IMC_UnregisterNamedEvent_onUID(ImcInfo *imc, unsigned int uid);
int IMC_RaiseNamedEvent(ImcInfo *imc, unsigned int source, const char *name,
                        void *param, unsigned int size);
int IMC_ProcessHeader(ImcInfo *imc, IpcHeader *header);
int IMC_SetLocalImc(void *imc);
ImcInfo *IMC_GetLocalImc(int create_if_missing);
void IMC_SetImcTracer(void *imc, void *tracer);
int IMC_GetIPCUnreadCounter(void *imc);
extern void (*IMC_ModuleTriggerFunc)(unsigned int uid);

struct aimc_event;
struct aimc *aimc_create(void);
void aimc_finalize(struct aimc *imc);
int aimc_handler_create(struct aimc *imc);
int aimc_create_handler(struct aimc *imc);
void aimc_handler_destory(struct aimc *imc);
void aimc_destroy_handler(struct aimc *imc);
void aimc_handler_wait(struct aimc *imc);
int aimc_process(struct aimc *imc);
int aimc_ipc_header_get(ImcInfo *info, unsigned int from, IpcHeader **h);
int aimc_ipc_header_done(ImcInfo *info, unsigned int from);
int aimc_raise_event(struct aimc *imc, unsigned int event, unsigned int owner, void *param,
                     unsigned int param_len);
struct aimc_event *aimc_register_event(struct aimc *imc, unsigned int event, unsigned int target,
                                       int repeat, void (*callback)(void *, int, void *),
                                       void *arg);
void aimc_cancel_event(struct aimc *imc, struct aimc_event *ev);
int aimc_command_response(struct aimc *imc, int result, void *param, unsigned int param_size);

int aimc_dispatch_command(ImcInfo *info, unsigned int target, unsigned int cmd, void *param,
                          unsigned int param_len, unsigned int sender, unsigned int message_id,
                          unsigned int flags);
#define AIMC_DISPATCH_FLAG_REQUIRE_RESPONSE 1
int aimc_ipc_send(ImcInfo *info, unsigned int target_processor, IpcHeader *header,
                  const void *param);
int aimc__register_command_handler(ImcInfo *info, unsigned int target, PFN_ImcCmdProc proc,
                                   void *module);
void aimc__unregister_command_handler(ImcInfo *info, unsigned int target);

/* ------------------------------------------------------------------------- */
/* imported from other libaa files                                            */
/* ------------------------------------------------------------------------- */
/* aud_thread_*, adebug_*, ModuleList_* come from types.h, RBTree_* from buffers.h */
#include "buffers.h"

#endif /* LIBAA_AIMC_LOCAL_H */
