/*
 * libaa legacy buffer layer: ADEC_BUF_T, the common (ring) buffer shared between the
 * ARM and the DSPs, the inter-buffer reader/writer front-ends, buffer debug helpers,
 * the RB tree used by IMC, and the static module list.
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 *
 * Layouts were read from the shipped code (Ghidra addresses):
 *   ADEC_BUF_T          InterBufReader_Create @0x2405c / _InterBufWriter_Create @0x24ccc
 *                       (memset 0x134, method slots 0x80..0xdc, mutexes at 0xf8/0x114,
 *                       ref_count 0x110, aimc 0x12c, coworker 0x130)
 *   ADEC_COMMON_BUF_T   CommonBuffer_GetStructSize @0x25104 (0x38 + n * 0x28),
 *                       CommonBuffer_Init @0x25118 (entry 0 at +0x10, bufOffset at +0x30)
 *   status              CommonBuffer_GetStatusStructSize @0x25114 (0x1c)
 *   BM event            ashbuf_notify_read @0x269dc (0x2c bytes, cmd 0xf101002c)
 * Field names follow LG's kdriver tree (adec_buf.h, adec_common_buf.h, adec_au_info.h,
 * module_cmd_buffer.h, adec_module_list.h); only names/layouts were taken from there.
 *
 */
#ifndef LIBAA_BUFFERS_H
#define LIBAA_BUFFERS_H

#include <pthread.h>
#include <stddef.h>

#include "types.h"

/* result codes: enum adec_result (ADEC_ERR_*) in types.h */

/* ------------------------------------------------------------------------- */
/* access unit info                                                          */
/* ------------------------------------------------------------------------- */
typedef enum _AUDIO_DATA_TYPE_T {
    AUDIO_DATA_TYPE_PCM,
    AUDIO_DATA_TYPE_ES,
    AUDIO_DATA_TYPE_META,
    AUDIO_DATA_TYPE_META_EXT,
    AUDIO_DATA_TYPE_EVENT,
    AUDIO_DATA_TYPE_MIXED,
} AUDIO_DATA_TYPE_T;

#define AUI_FLAG_ERROR          1
#define AUI_FLAG_MUTE           2
#define AUI_FLAG_FADEOUT        4
#define AUI_FLAG_DISCONTINUITY  8
#define AUI_FLAG_LAST_AU        16

typedef struct _ADEC_AU_INFO_T {             /* 0x20 bytes, copied as a block */
    unsigned int size;                       /* 0x00 */
    unsigned int timestamp;                  /* 0x04 */
    unsigned long long timestamp64;          /* 0x08 */
    unsigned int gstc;                       /* 0x10 */
    short index;                             /* 0x14 */
    unsigned char flags;                     /* 0x16 AUI_FLAG_* */
    unsigned char acmod;                     /* 0x17 */
    unsigned int first_module_gstc;          /* 0x18 */
    AUDIO_DATA_TYPE_T dataType;              /* 0x1c */
} ADEC_AU_INFO_T;

/* ------------------------------------------------------------------------- */
/* common buffer (lives at the start of the shared memory block)             */
/* ------------------------------------------------------------------------- */
typedef struct _ADEC_COMMON_BUF_STATUS_T {   /* 0x1c bytes, first member of ADEC_BUF_T */
    int indexAllocated;                      /* 0x00 an AU is open for writing */
    int readOffset;                          /* 0x04 */
    int writeOffset;                         /* 0x08 */
    short readIndex;                         /* 0x0c */
    short writeIndex;                        /* 0x0e */
    int written;                             /* 0x10 bytes in the open AU */
    int passed;                              /* 0x14 bytes flushed since last GetPassSize */
    int repeat_count;                        /* 0x18 */
} ADEC_COMMON_BUF_STATUS_T;

typedef struct _ADEC_COM_BUF_AU_INFO_T {     /* 0x28 bytes */
    ADEC_AU_INFO_T base;
    int bufOffset;                           /* 0x20 ring offset of the AU's first byte */
    int __not_used;
} ADEC_COM_BUF_AU_INFO_T;

typedef struct _ADEC_COMMON_BUF_T {          /* 0x10 + (n + 1) * 0x28 */
    int maxSize;                             /* 0x00 ring size (without the mirror area) */
    int accessSize;                          /* 0x04 mirror area = max single access */
    int maxIndex;                            /* 0x08 index table entries */
    ADEC_COM_BUF_AU_INFO_T indexTable[1];    /* 0x10 */
} ADEC_COMMON_BUF_T;

/* ------------------------------------------------------------------------- */
/* ADEC_BUF_T                                                                */
/* ------------------------------------------------------------------------- */
struct _ADEC_BUF_T;

/* cache maintenance / address translation callbacks, ADEC_PROCESSOR_ADAP_IF_T */
typedef struct _ADEC_PROCESSOR_ADAP_IF_T {
    void (*pfnInvalidate)(void *addr, int len, struct _ADEC_BUF_T *buf);
    void (*pfnWriteback)(void *addr, int len, struct _ADEC_BUF_T *buf);
    void *(*pfnAddrTrans)(void *addr, struct _ADEC_BUF_T *buf);
} ADEC_PROCESSOR_ADAP_IF_T;

typedef enum _ADEC_BUF_TYPE_T {
    INTER_BUF_READER_TYPE,
    INTER_BUF_WRITER_TYPE,
    MULTI_SINK_BUF_TYPE,
    MULTI_SINK_BUF_CLIENT_TYPE,
    INTERNAL_BUF_TYPE,
} ADEC_BUF_TYPE_T;

/* legacy kernel-style module, only the fields BufDbg_Dump/modlink print */
typedef struct _AdecModule {
    unsigned int unknown_000;
    unsigned int module_id;                  /* 0x004 */
    unsigned char unknown_008[0x21c];
    char module_name[16];                    /* 0x224 */
} AdecModule;

typedef struct _AdecModuleLink {             /* 0x18 bytes */
    unsigned int ext_addr;
    AdecModule *module;
    unsigned int port;
    struct _ADEC_BUF_T *buf;
    struct link link;
} AdecModuleLink;

typedef struct _ADEC_BUF_T {                 /* 0x134 bytes (Get*StructSize) */
    ADEC_COMMON_BUF_STATUS_T status;         /* 0x000 */
    ADEC_COMMON_BUF_T *commonBuf;            /* 0x01c */
    unsigned int max_access_size;            /* 0x020 */
    unsigned int max_size;                   /* 0x024 */
    unsigned int aui_count;                  /* 0x028 (indices are taken modulo, unsigned) */
    void *boundary_buf;                      /* 0x02c reader: private copy for wrapped reads */
    unsigned char *data;                     /* 0x030 [mirror (max_access_size)][ring (max_size)] */
    ADEC_PROCESSOR_ADAP_IF_T *adap;          /* 0x034 */
    unsigned int offset;                     /* 0x038 system-wide id of the shared buffer */
    AdecModuleLink input;                    /* 0x03c */
    AdecModuleLink output;                   /* 0x054 */
    unsigned char unknown_06c[0x14];         /* never touched by libaa */

    int (*init)(struct _ADEC_BUF_T *buf);                                       /* 0x080 */
    int (*get_free_size)(struct _ADEC_BUF_T *buf);                              /* 0x084 */
    int (*get_pass_size)(struct _ADEC_BUF_T *buf);                              /* 0x088 */
    int (*get_used_size)(struct _ADEC_BUF_T *buf);                              /* 0x08c */
    int (*get_au_cnt)(struct _ADEC_BUF_T *buf);                                 /* 0x090 */
    int (*get_max_size)(struct _ADEC_BUF_T *buf);                               /* 0x094 */
    int (*get_max_au)(struct _ADEC_BUF_T *buf);                                 /* 0x098 */
    int (*get_max_au_length)(struct _ADEC_BUF_T *buf);                          /* 0x09c */
    int (*create_new_au)(ADEC_AU_INFO_T *info, struct _ADEC_BUF_T *buf);        /* 0x0a0 */
    void *(*request_mem)(ADEC_AU_INFO_T *info, unsigned int size,
                         struct _ADEC_BUF_T *buf);                              /* 0x0a4 */
    int (*write_data)(unsigned char *src, int len, struct _ADEC_BUF_T *buf);    /* 0x0a8 */
    int (*finish_au)(struct _ADEC_BUF_T *buf);                                  /* 0x0ac */
    int (*insert_au)(unsigned char *src, int len, ADEC_AU_INFO_T *info,
                     struct _ADEC_BUF_T *buf);                                  /* 0x0b0 */
    int (*update_au)(ADEC_AU_INFO_T *info, struct _ADEC_BUF_T *buf);            /* 0x0b4 */
    int (*get_au_info)(int index, int *offset, ADEC_AU_INFO_T *info,
                       struct _ADEC_BUF_T *buf);                                /* 0x0b8 */
    int (*get_current_info)(int *offset, ADEC_AU_INFO_T *info,
                            struct _ADEC_BUF_T *buf);                           /* 0x0bc */
    int (*read_data)(unsigned char **dst, int *size, int *au_in_data,
                     struct _ADEC_BUF_T *buf);                                  /* 0x0c0 */
    int (*flush_data)(int size, struct _ADEC_BUF_T *buf);                       /* 0x0c4 */
    int (*read_au)(unsigned char **dst, int *size, int *au_cnt,
                   struct _ADEC_BUF_T *buf);                                    /* 0x0c8 */
    int (*event)(int event, void *param, struct _ADEC_BUF_T *buf);              /* 0x0cc */
    int (*set_repeat)(int cnt, struct _ADEC_BUF_T *buf);                        /* 0x0d0 */
    int (*request_data)(struct _ADEC_BUF_T *buf);                               /* 0x0d4 */
    int (*ref)(struct _ADEC_BUF_T *buf);                                        /* 0x0d8 */
    int (*unref)(struct _ADEC_BUF_T *buf);                                      /* 0x0dc */

    ADEC_BUF_TYPE_T buf_type;                /* 0x0e0 */
    unsigned char unknown_0e4[0x14];         /* never touched by libaa */
    pthread_mutex_t ref_count_mutex;         /* 0x0f8 */
    int ref_count;                           /* 0x110 */
    pthread_mutex_t lock;                    /* 0x114 */
    struct aimc *aimc;                       /* 0x12c */
    unsigned int coworker;                   /* 0x130 uid of the remote peer module */
} ADEC_BUF_T;

_Static_assert(sizeof(ADEC_COMMON_BUF_STATUS_T) == 0x1c, "status size");
_Static_assert(sizeof(ADEC_COM_BUF_AU_INFO_T) == 0x28, "aui size");
_Static_assert(sizeof(ADEC_COMMON_BUF_T) == 0x38, "common buf size");
#if __SIZEOF_POINTER__ == 4
_Static_assert(offsetof(ADEC_BUF_T, init) == 0x80, "vtable offset");
_Static_assert(offsetof(ADEC_BUF_T, lock) == 0x114, "lock offset");
_Static_assert(sizeof(ADEC_BUF_T) == 0x134, "ADEC_BUF_T size");
#endif

/* ------------------------------------------------------------------------- */
/* buffer manager event (BM_CMD_PROCESS_EVT_REMOTE)                          */
/* ------------------------------------------------------------------------- */
#define BM_CMD_PROCESS_EVT_REMOTE   0xf101002cu

typedef enum {
    BM_EVT_CREATE,
    BM_EVT_DESTROY,
    BM_EVT_READ,                             /* 2: reader consumed, carries read idx/offset */
    BM_EVT_WRITE,                            /* 3: writer produced, carries write idx/offset */
} BM_EVT_TYPE_T;

typedef struct _BmCmdProcessEvtRemote {      /* 0x2c bytes */
    BM_EVT_TYPE_T evt_type;
    ADEC_BUF_TYPE_T buf_type;
    unsigned int base_addr;                  /* ADEC_BUF_T::offset */
    ADEC_COMMON_BUF_STATUS_T status;
    int update_mask;
} BmCmdProcessEvtRemote;

/* ------------------------------------------------------------------------- */
/* prototypes                                                                */
/* ------------------------------------------------------------------------- */
/* buffers.c */
int CommonBuffer_GetStructSize(int index_count);
int CommonBuffer_GetStatusStructSize(void);
int CommonBuffer_Init(ADEC_BUF_T *buf);
int CommonBuffer_Create(int buffer_size, int max_au_size, int index_count, ADEC_BUF_T *buf);
int CommonBuffer_GetFreeSize(ADEC_BUF_T *buf);
int CommonBuffer_GetPassSize(ADEC_BUF_T *buf);
int CommonBuffer_GetUsedSize(ADEC_BUF_T *buf);
int CommonBuffer_GetAuCnt(ADEC_BUF_T *buf);
int CommonBuffer_GetMaxSize(ADEC_BUF_T *buf);
int CommonBuffer_GetMaxAu(ADEC_BUF_T *buf);
int CommonBuffer_GetMaxAuLength(ADEC_BUF_T *buf);
void common_written_and_reset(void);
int CommonBuffer_FinishAu(ADEC_BUF_T *buf);
int CommonBuffer_CreateNewAu(ADEC_AU_INFO_T *au, ADEC_BUF_T *buf);
int CommonBuffer_WriteData(unsigned char *src, int len, ADEC_BUF_T *buf);
int CommonBuffer_WriteDataUser(unsigned char *src, int len, ADEC_BUF_T *buf);
void *CommonBuffer_RequestMem(ADEC_AU_INFO_T *au, ADEC_BUF_T *buf, unsigned int size);
int CommonBuffer_InsertAu(unsigned char *src, int len, ADEC_AU_INFO_T *au, ADEC_BUF_T *buf);
int CommonBuffer_InsertAuUser(unsigned char *src, int len, ADEC_AU_INFO_T *au,
                              ADEC_BUF_T *buf);
int CommonBuffer_UpdateAu(ADEC_AU_INFO_T *au, ADEC_BUF_T *buf);
int CommonBuffer_GetAuInfo(int index, int *offset, ADEC_AU_INFO_T *au, ADEC_BUF_T *buf);
int CommonBuffer_GetCurrentInfo(int *offset, ADEC_AU_INFO_T *au, ADEC_BUF_T *buf);
int CommonBuffer_ReadData(unsigned char **dst, int *size, int *au_in_data, ADEC_BUF_T *buf);
int CommonBuffer_FlushData(int *size, ADEC_BUF_T *buf);
int CommonBuffer_ReadAu(unsigned char **dst, int *size, int *au_cnt, ADEC_BUF_T *buf);
int CommonBuffer_Ref(ADEC_BUF_T *buf);
int CommonBuffer_Unref(ADEC_BUF_T *buf);
void ashbuf_notify_read(struct aimc *imc, ADEC_BUF_T *buf, unsigned int coworker);
void ashbuf_notify_write(struct aimc *imc, ADEC_BUF_T *buf, unsigned int coworker);
void BufDbg_AssertBreak(void);
void BufDbg_Dump(ADEC_BUF_T *buf);

/* interbuf.c */
int InterBufReader_GetStructSize(void);
int InterBufReader_Create(ADEC_PROCESSOR_ADAP_IF_T *adap, ADEC_COMMON_BUF_T *common_buf,
                          int buffer_size, int max_access_size, int index_count, void *data,
                          unsigned int coworker, struct aimc *imc, ADEC_BUF_T *buf);
int InterBufReader_CreateFromCommonBuffer(ADEC_PROCESSOR_ADAP_IF_T *adap,
                                          ADEC_COMMON_BUF_T *common_buf, void *data,
                                          unsigned int coworker, struct aimc *imc,
                                          ADEC_BUF_T *buf);
int InterBufReader_UpdateStatus(ADEC_BUF_T *buf, ADEC_COMMON_BUF_STATUS_T *status);
int InterBufWriter_GetStructSize(void);
int InterBufWriter_Create(ADEC_PROCESSOR_ADAP_IF_T *adap, ADEC_COMMON_BUF_T *common_buf,
                          int buffer_size, int max_access_size, int index_count, void *data,
                          unsigned int coworker, struct aimc *imc, ADEC_BUF_T *buf);
int InterBufWriter_CreateFromCommonBuffer(ADEC_PROCESSOR_ADAP_IF_T *adap,
                                          ADEC_COMMON_BUF_T *common_buf, void *data,
                                          unsigned int coworker, struct aimc *imc,
                                          ADEC_BUF_T *buf);
int InterBufWriter_CreateFromCommonBufferUser(ADEC_PROCESSOR_ADAP_IF_T *adap,
                                              ADEC_COMMON_BUF_T *common_buf, void *data,
                                              unsigned int coworker, struct aimc *imc,
                                              ADEC_BUF_T *buf);
int InterBufWriter_UpdateStatus(ADEC_BUF_T *buf, ADEC_COMMON_BUF_STATUS_T *status);

/* rbtree.c */
typedef unsigned int RBKey;
int RBTree_CreateTree(void **tree, void (*terminator)(void *info));
int RBTree_InitTree(void *tree, void (*terminator)(void *info));
int RBTree_DeleteTree(void *tree);
int RBTree_CreateNode(void *tree, RBKey key, void *info);
int RBTree_Search(void *tree, RBKey key, void *info);    /* info: void ** */
int RBTree_Delete(void *tree, RBKey key);

/* modlist.c */
typedef enum _ADEC_CORE_TYPE {
    ADEC_CORE_NOT_DEF = 0,
    ADEC_CORE_ARM,
    ADEC_CORE_DSP0,
    ADEC_CORE_DSP1,
    ADEC_CORE_DSP2,
    ADEC_CORE_DSP3,
} ADEC_CORE_TYPE;

#define ADEC_MODULE_INFO_COUNT  201          /* ADEC_MODULE_NOT_DEF .. ADEC_MODULE_MAX - 1 */
#define ADEC_MOD_TYPE_COUNT     34           /* type names, last is "INVALID" */

typedef struct _ModuleInfo {                 /* 0x30 bytes */
    unsigned int module;
    char *name;
    unsigned int type;                       /* ADEC_MODULE_TYPE */
    unsigned int location;                   /* ADEC_CORE_TYPE */
    int num_in;
    int num_out;
    unsigned int out_buffer_type[4];
    struct link list_head;
} ModuleInfo;

typedef struct _ModuleMemInfo {              /* 0x28 bytes */
    unsigned int module;
    unsigned int ioType;
    int bufSize;
    int accessSize;
    int auCount;
    int def_bufSize;
    int def_accessSize;
    int def_auCount;
    struct link list_head;
} ModuleMemInfo;

extern ModuleInfo gModuleInfo[ADEC_MODULE_INFO_COUNT];

int ModuleList_SetPlatform(unsigned int core);
int ModuleList_GetModuleInfo(unsigned int module, ModuleInfo *info);
int ModuleList_SetModuleInfo(unsigned int module, ModuleInfo *info);
int ModuleList_GetModuleList(unsigned int type, unsigned int *result, int max);
const char *ModuleList_GetModuleTypeName(unsigned int type);
ModuleMemInfo *ModuleList_GetModuleMemoryInfo(unsigned int module);

/* ------------------------------------------------------------------------- */
/* buffer assert (ADEC_BUF_ASSERT): counts, logs on "aud", dumps, returns.    */
/* cond_str is the original source text of the condition, as found in .rodata */
/* ------------------------------------------------------------------------- */
#define BUF_ASSERT(cond, cond_str, buf, ret, line, ...) do { \
        if (!(cond)) { \
            BufDbg_AssertBreak(); \
            if (LOGMM_ON(aud, log_level_error)) { \
                logmm_printl(&logmm_module_aud, log_level_error, __func__, (line), \
                             "Assert:" cond_str "\n"); \
                LOGE(aud, line, __VA_ARGS__); \
            } \
            BufDbg_Dump(buf); \
            return (ret); \
        } \
    } while (0)

#endif /* LIBAA_BUFFERS_H */
