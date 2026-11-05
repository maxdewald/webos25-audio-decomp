/*
 * Shared declarations for the libpf reconstruction (starfish media pipeline framework,
 * namespace mediapipeline). RECONSTRUCTION from orig/usr/lib/libpf-1.0.so.1.0.0
 *   sha256 03211e3d4a9d73bf75d16d4001d051f86762d215056d38aed118c7d4dd20b374
 * Ghidra addresses = raw ELF VA + 0x10000. Pseudocode: build/decomp/libpf-1.0.so.1.0.0.c.
 *
 * Conventions (see README.md in this directory):
 *  - One .cpp per original source file (__FILE__ strings: "airplayer.cpp", ...).
 *  - Each .cpp declares the classes it implements locally, with only the members it
 *    defines. Object layout is accessed through FIELD(); layouts are not shared.
 *  - Class hierarchy (RTTI): Player <- AbstractPlayer <- {AirPlayer, CustomPlayer,
 *    DvrPlayer, DirectMediaPlayer, SWDecPlayer <- {EsSWDecPlayer, UriSWDecPlayer},
 *    GenericPlayer <- {Adaptive,Ad,Dlna,Hls,Thumbnail,Usb}Player};
 *    Subject <- Pipeline <- {CustomPipeline, DvrPipeline, GenericPipeline <-
 *    {Adaptive,Ad,Dlna,Hls,Thumbnail,Usb}Pipeline}.
 */
#ifndef LIBPF_H
#define LIBPF_H

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <string>
#include <glib-object.h>
#include <gst/gst.h>

#define FIELD(T, obj, off) (*(T *)((char *)(obj) + (off)))
/* Virtual call through the vtable slot at byte offset `off`. */
#define VCALL(R, obj, off, ...) \
    ((*(R (**)(...))(*(char **)(obj) + (off)))((obj), ##__VA_ARGS__))

/* PmLogLib (libPmLogLib.so.3). The context's first int is its current level. */
extern "C" {
typedef struct PmLogContextStruct { int level; } *PmLogContext;
int PmLogGetContextLevel(PmLogContext ctx, int *level);
int PmLogGetContext(const char *name, PmLogContext *ctx);
int PmLogString_(PmLogContext ctx, int level, const char *msgid, const char *kv,
                 const char *msg);
}

/* Logger object: static `pflog` (Pipeline::pflog, *_pflog) or a member (AirPlayer +0x64).
 * The name buffer follows the context. */
struct PfLog {
    PmLogContext ctx;
    char name[32];
};

#define PF_CODE_POINT(cp, name, file, func, line)                                      \
    snprintf((cp), 0x100, "[%s][%ld:%ld]<%s:%s(%d)>", (name), syscall(SYS_getpid),      \
             syscall(SYS_gettid), (file), (func), (line))

/* Debug: only formatted when the context level is exactly 7. Message is
 * "{\"CODE_POINT\":\"...\"} <text>" in one 0x200 buffer. */
#define PF_LOG_DEBUG(log, file, func, line, ...)                                       \
    do {                                                                               \
        int lvl_;                                                                      \
        if (PmLogGetContextLevel((log).ctx, &lvl_) == 0 && lvl_ == 7) {                \
            char cp_[0x100], msg_[0x200];                                              \
            PF_CODE_POINT(cp_, (log).name, file, func, line);                          \
            int n_ = snprintf(msg_, 0x200, "{\"%s\":\"%s\"}", "CODE_POINT", cp_);      \
            msg_[n_] = ' ';                                                            \
            snprintf(msg_ + n_ + 1, 0x1ff - n_, __VA_ARGS__);                          \
            if ((log).ctx == NULL || (log).ctx->level > 6)                             \
                PmLogString_((log).ctx, 7, NULL, NULL, msg_);                          \
        }                                                                              \
    } while (0)

/* Info/warning/error: always formatted; kv = "{\"CODE_POINT\":\"...\"}". */
#define PF_LOG_LEVEL(log, lvl, msgid, file, func, line, ...)                         \
    do {                                                                               \
        char cp_[0x200], kv_[0x200];                                                   \
        PF_CODE_POINT(cp_, (log).name, file, func, line);                              \
        snprintf(kv_, 0x200, "{\"%s\":\"%s\"}", "CODE_POINT", cp_);                    \
        snprintf(cp_, 0x200, __VA_ARGS__);                                             \
        if ((log).ctx == NULL || (log).ctx->level > (lvl) - 1)                         \
            PmLogString_((log).ctx, (lvl), (msgid), kv_, cp_);                       \
    } while (0)

#define PF_LOG_INFO(log, file, func, line, ...) \
    PF_LOG_LEVEL(log, 6, "SFPIPELINE", file, func, line, __VA_ARGS__)
#define PF_LOG_WARNING(log, file, func, line, ...) \
    PF_LOG_LEVEL(log, 4, "SFPIPELINE", file, func, line, __VA_ARGS__)
#define PF_LOG_ERROR(log, file, func, line, ...) \
    PF_LOG_LEVEL(log, 3, "SFPIPELINE", file, func, line, __VA_ARGS__)

/* Custom event type used for "acquired-resource" (GST_EVENT_CUSTOM_DOWNSTREAM_OOB...). */
#define PF_EVENT_ACQUIRED_RESOURCE ((GstEventType)0x10e01)

#endif
