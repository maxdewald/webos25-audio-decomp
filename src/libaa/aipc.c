/*
 * libaa aipc: small RPC over AF_UNIX SOCK_SEQPACKET sockets between Linux audio
 * processes. Every process listens on "<base>.<proc_id>" (base = option
 * "aipc_socket_base", default "/build/scratch/aipc"); the controller (acont, proc 0) listens on
 * "<base>.0" and hands out proc ids via "acont_register_proc".
 *
 * Wire format (one request):
 *   u32 write_size, u32 argc, u32 flags          (12-byte header, separate write)
 *   u32 name_size, char name[name_size]          (padded to 8)
 *   argc x { u32 type, u32 size, u8 data[size] } (each padded to 8)
 * Reply (only if flags & AIPC_FLAG_NEED_RESPONSE):
 *   int error, int ret, u32 additional_size, then additional_size bytes
 *   (no additional data if flags & AIPC_FLAG_NO_ADDITIONAL).
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 * Evidence: build/decomp/libaa.so.2.0.0.c @0x1e6e8..@0x205b4, logmm "aipc" strings.
 */
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "aimc_local.h"

LOGMM_MODULE_DEFINE(aipc);

struct aipc_command {             /* 0x14 */
    struct link list;
    const char *name;
    aipc_command_fn func;
    void *arg;
};

struct aipc_additional {          /* 0x10 */
    struct link list;
    void *data;                   /* owned, freed after the reply */
    unsigned int size;
};

/* decoded request: descriptor followed by the args array (one malloc) */
struct ipc_desc {                 /* 0x18 */
    unsigned int argc;
    struct aipc_ipc_arg *args;
    unsigned int flags;
    unsigned int name_size;
    char *name;
    void *buf;
};

struct ipc_msg_header {
    unsigned int write_size;
    unsigned int argc;
    unsigned int flags;
};

struct ipc_reply {
    int error;
    int ret;
    unsigned int additional_size;
};

#define AIPC_MAX_ADDITIONAL 0x6400000   /* 100 MiB */
#define ALIGN8(x)           (((x) + 7) & ~7u)

static char *aipc_socket_base;

static const char *socket_base(void)
{
    if (!aipc_socket_base) {
        aipc_socket_base = autil_option_str("aipc_socket_base");
        if (!aipc_socket_base)
            aipc_socket_base = strdup("/build/scratch/aipc");
    }
    return aipc_socket_base;
}

/* @0x1e6e8: connect_socket(fmt, ...) */
int connect_socket(const char *fmt, ...)
{
    struct sockaddr_un addr;
    va_list ap;
    int fd;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    va_start(ap, fmt);
    vsnprintf(addr.sun_path, sizeof(addr.sun_path), fmt, ap);
    va_end(ap);

    fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (fd < 0) {
        LOGE(aipc, 0x8d, "socket() failed. %s\n", strerror(errno));
        return -1;
    }
    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        LOGE(aipc, 0x93, "connect() failed. %s\n", strerror(errno));
        close(fd);
        return -1;
    }
    LOGI(aipc, 0x98, "connect socket name, \"%s\". fd%d\n", addr.sun_path, fd);
    return fd;
}

/* @0x1e7fc: (type, value...)* 0 varargs -> malloc'ed aipc_ipc_arg array */
struct aipc_ipc_arg *get_ipc_argv(va_list ap, int *argc_out)
{
    struct aipc_ipc_arg *args;
    va_list cnt;
    int argc = 0, i, type;

    va_copy(cnt, ap);
    while ((type = va_arg(cnt, int)) != ipc_argtype_end) {
        switch (type) {
        case ipc_argtype_bool:
        case ipc_argtype_int:
        case ipc_argtype_str:
            (void)va_arg(cnt, int);
            break;
        case ipc_argtype_longlong:
        case ipc_argtype_double:
            (void)va_arg(cnt, long long);
            break;
        case ipc_argtype_mem:
            (void)va_arg(cnt, void *);
            (void)va_arg(cnt, unsigned int);
            break;
        default:
            LOGE(aipc, 0x357, "unknown type. %d, %d\n", argc, type);
            va_end(cnt);
            return NULL;
        }
        argc++;
    }
    va_end(cnt);
    LOGD(aipc, 0x35d, "ipc argc %d\n", argc);

    args = malloc(argc * sizeof(*args));
    if (!args) {
        LOGE(aipc, 0x361, "no mem\n");
        return NULL;
    }
    for (i = 0; i < argc; i++) {
        type = va_arg(ap, int);
        args[i].type = type;
        switch (type) {
        case ipc_argtype_bool:
        case ipc_argtype_int:
            args[i].i = va_arg(ap, int);
            break;
        case ipc_argtype_str:
            args[i].s = va_arg(ap, char *);
            break;
        case ipc_argtype_longlong:
            args[i].ll = va_arg(ap, long long);
            break;
        case ipc_argtype_double:
            args[i].d = va_arg(ap, double);
            break;
        case ipc_argtype_mem:
            args[i].m = va_arg(ap, void *);
            args[i].size = va_arg(ap, unsigned int);
            break;
        default:
            LOGE(aipc, 0x381, "unknown type. %d, %d\n", i, type);
            free(args);
            return NULL;
        }
    }
    *argc_out = argc;
    return args;
}

/* @0x1e9a8: connect to "<base>.<proc_id>" and remember the server fd */
int get_target_fd(struct aipc *ipc, int proc_id)
{
    struct aipc_server *s;
    int fd;

    fd = connect_socket("%s.%d", aipc_socket_base, proc_id);
    if (fd < 0) {
        LOGE(aipc, 0x2dd, "no connection to %d\n", proc_id);
        return -1;
    }
    LOGI(aipc, 0x2e0, "new connection to procid %d, fd %d\n", proc_id, fd);
    s = realloc(ipc->servers, (ipc->nservers + 1) * sizeof(*s));
    if (!s) {
        LOGE(aipc, 0x2e5, "no mem. %d\n", ipc->nservers);
        close(fd);
        return -1;
    }
    ipc->servers = s;
    s[ipc->nservers].fd = fd;
    s[ipc->nservers].proc_id = proc_id;
    s[ipc->nservers].refs = 1;
    ipc->nservers++;
    return fd;
}

/* @0x1eab0: create_socket(fmt, ...) - listening socket */
int create_socket(const char *fmt, ...)
{
    struct sockaddr_un addr;
    va_list ap;
    int fd;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    va_start(ap, fmt);
    vsnprintf(addr.sun_path, sizeof(addr.sun_path), fmt, ap);
    va_end(ap);
    LOGI(aipc, 99, "create socket name, \"%s\"\n", addr.sun_path);

    fd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (fd < 0) {
        LOGE(aipc, 0x68, "socket() failed. %s\n", strerror(errno));
        return -1;
    }
    unlink(addr.sun_path);
    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        LOGE(aipc, 0x6f, "bind() failed. %s\n", strerror(errno));
    } else if (listen(fd, 20) < 0) {
        LOGE(aipc, 0x76, "connect() failed. %s\n", strerror(errno));
    } else {
        return fd;
    }
    close(fd);
    return -1;
}

/* @0x1ec0c: _create */
struct aipc *aipc__create(int proc_id)
{
    struct aipc *ipc;

    ipc = calloc(1, sizeof(*ipc));
    if (!ipc) {
        LOGF(aipc, log_level_error, "_create", 0x27a, "no mem\n");
        return NULL;
    }
    pthread_mutex_init(&ipc->call_lock, NULL);
    LINK_INIT(&ipc->command_list);
    LINK_INIT(&ipc->additional_list);
    ipc->listen_fd = create_socket("%s.%d", socket_base(), proc_id);
    ipc->proc_id = proc_id;
    LOGF(aipc, log_level_info, "_create", 0x286, "create aipc at %s.%d fd%d\n",
         aipc_socket_base, proc_id, ipc->listen_fd);
    return ipc;
}

/* ipc_copy_arg (inlined): serialise one argument at p; returns its data size or -1 */
static int ipc_copy_arg(unsigned int *p, const struct aipc_ipc_arg *a)
{
    unsigned int size;

    switch (a->type) {
    case ipc_argtype_bool:
    case ipc_argtype_int:    size = 4; break;
    case ipc_argtype_longlong:
    case ipc_argtype_double: size = 8; break;
    case ipc_argtype_str:    size = strlen(a->s) + 1; break;
    case ipc_argtype_mem:    size = a->size; break;
    default:
        LOGF(aipc, log_level_error, "ipc_copy_arg", 0x18a, "unknown arg type, %d\n", a->type);
        return -1;
    }
    p[0] = a->type;
    p[1] = size;
    LOGF(aipc, log_level_trace, "ipc_copy_arg", 0x196, "type %d, size %d\n", a->type, size);
    switch (a->type) {
    case ipc_argtype_bool:
    case ipc_argtype_int:
        p[2] = a->i;
        break;
    case ipc_argtype_longlong:
    case ipc_argtype_double:
        memcpy(&p[2], &a->ll, 8);
        break;
    default:
        memcpy(&p[2], a->m, size);
        break;
    }
    return size;
}

/* @0x1ecf8: _ipc_send - serialise and write one request */
int ipc_send(int fd, unsigned int flags, const char *name, int argc,
             const struct aipc_ipc_arg *args)
{
    struct ipc_msg_header hdr;
    unsigned int args_size = 0, write_size, name_len;
    unsigned char *buf, *p;
    ssize_t n;
    int i;

    LOGD(aipc, 0x1c3, "send ipc. %s\n", name);
    for (i = 0; i < argc; i++) {
        switch (args[i].type) {
        case ipc_argtype_bool:
        case ipc_argtype_int:
            args_size += ALIGN8(4 + 8);
            break;
        case ipc_argtype_longlong:
        case ipc_argtype_double:
            args_size += 0x10;
            break;
        case ipc_argtype_str:
            args_size += (strlen(args[i].s) + 0x10) & ~7u;
            break;
        case ipc_argtype_mem:
            if (args[i].size) {
                args_size += ALIGN8(args[i].size + 8);
                break;
            }
            /* zero-sized memory arguments are rejected */
            /* fall through */
        default:
            if (args[i].type < ipc_argtype_bool || args[i].type > ipc_argtype_mem)
                LOGF(aipc, log_level_error, "ipc_copy_arg", 0x18a, "unknown arg type, %d\n",
                     args[i].type);
            LOGE(aipc, 0x1ce, "unknown arg at %s\n", name);
            return -1;
        }
    }
    name_len = strlen(name);
    write_size = ((name_len + 0xc) & ~7u) + args_size;
    LOGD(aipc, 0x1d7, "send ipc to fd%d, %s, write size %d, args %d, flags 0x%x\n",
         fd, name, write_size, argc, flags);

    buf = calloc(1, write_size);
    if (!buf) {
        LOGE(aipc, 0x1dd, "no mem for %s. write size %d, args size %d, args count %d\n",
             name, write_size, args_size, argc);
        return -1;
    }
    *(unsigned int *)buf = name_len + 1;
    LOGT(aipc, 0x1e4, "name size %d\n", name_len + 1);
    memcpy(buf + 4, name, name_len + 1);
    p = buf + ALIGN8(4 + name_len + 1);
    for (i = 0; i < argc; i++) {
        int size = ipc_copy_arg((unsigned int *)p, &args[i]);
        p += 8;
        if (size > 0)
            p += size;
        p = (unsigned char *)ALIGN8((uintptr_t)p);
    }

    if (write_size != (unsigned int)(p - buf)) {
        LOGE(aipc, 0x1f9, "write size != (p - write_buf). %d != (%p - %p)\n", write_size, p, buf);
        goto fail;
    }
    hdr.write_size = write_size;
    hdr.argc = argc;
    hdr.flags = flags;
    n = write(fd, &hdr, sizeof(hdr));
    if (n != sizeof(hdr)) {
        LOGE(aipc, 0x205, "write(header) failed. %d. %s\n", (int)n, strerror(errno));
        goto fail;
    }
    n = write(fd, buf, write_size);
    if ((unsigned int)n != write_size) {
        LOGE(aipc, 0x20d, "write(desc) failed. %d. %s\n", (int)n, strerror(errno));
        goto fail;
    }
    free(buf);
    return 0;

fail:
    free(buf);
    return -1;
}

/* @0x1f154: send a request and read the reply (+ additional data) */
int ipc_send_and_wait(int fd, int *call_ret, const char *name, int argc,
                      const struct aipc_ipc_arg *args, void **additional,
                      unsigned int *additional_size)
{
    struct ipc_reply reply;
    unsigned int flags;
    ssize_t n;
    void *data;

    LOGD(aipc, 0x224, "call %s..\n", name);
    flags = additional ? AIPC_FLAG_NEED_RESPONSE
                       : AIPC_FLAG_NEED_RESPONSE | AIPC_FLAG_NO_ADDITIONAL;
    if (ipc_send(fd, flags, name, argc, args) < 0)
        return -1;

    n = read(fd, &reply, sizeof(reply));
    if (n != sizeof(reply)) {
        LOGE(aipc, 0x22f, "read(response) failed. %d, %s\n", (int)n, strerror(errno));
        return -1;
    }
    LOGD(aipc, 0x233, "response error %d, return %d, additional_size %d\n",
         reply.error, reply.ret, reply.additional_size);

    if (reply.additional_size) {
        if (reply.additional_size > AIPC_MAX_ADDITIONAL) {
            LOGE(aipc, 0x23b, "too big additional data size. %d\n", reply.additional_size);
            data = NULL;
        } else {
            data = malloc(reply.additional_size + 1);
        }
        if (!data) {
            LOGE(aipc, 0x241, "no mem. %d\n", reply.additional_size);
            return -1;
        }
        n = read(fd, data, reply.additional_size);
        if ((unsigned int)n != reply.additional_size) {
            LOGE(aipc, 0x249, "read(additional) failed. %d(%d), %s\n", (int)n,
                 reply.additional_size, strerror(errno));
            free(data);
            return -1;
        }
        ((char *)data)[reply.additional_size] = '\0';
        if (additional && additional_size) {
            *additional = data;
            *additional_size = reply.additional_size;
        } else {
            free(data);
        }
    }
    if (call_ret)
        *call_ret = reply.ret;
    return reply.error;
}

/* @0x1f340: the controller's own endpoint (proc id 0) */
struct aipc *aipc_create0(void)
{
    struct aipc *ipc;

    LOGI(aipc, 0x290, "create0..\n");
    ipc = aipc__create(0);
    if (ipc)
        ipc->is_controller = true;
    return ipc;
}

/* @0x1f394: register with the controller and listen on our own socket */
struct aipc *aipc_create(void)
{
    struct aipc *ipc;
    int fd, proc_id;

    LOGI(aipc, 0x29f, "create..\n");
    fd = connect_socket("%s.0", socket_base());
    if (fd < 0) {
        LOGE(aipc, 0x2a4, "no connection to controller.\n");
        return NULL;
    }
    if (ipc_send_and_wait(fd, &proc_id, "acont_register_proc", 0, NULL, NULL, NULL) < 0) {
        LOGE(aipc, 0x2aa, "\"acont_register_proc\" failed\n");
        close(fd);
        return NULL;
    }
    LOGI(aipc, 0x2af, "my proc_id %d\n", proc_id);

    ipc = aipc__create(proc_id);
    if (!ipc) {
        close(fd);
        return NULL;
    }
    ipc->servers = calloc(1, sizeof(*ipc->servers));
    if (!ipc->servers) {
        LOGE(aipc, 0x2ba, "nomem\n");
        close(fd);
        free(ipc);
        return NULL;
    }
    ipc->servers[0].fd = fd;            /* servers[0] is always the controller */
    ipc->servers[0].proc_id = 0;
    ipc->servers[0].refs = 1;
    ipc->nservers = 1;
    return ipc;
}

/* @0x1f530: only frees the struct (sockets and lists leak, as in the original) */
void aipc_destroy(struct aipc *ipc)
{
    free(ipc);
}

static int find_target_fd(struct aipc *ipc, int target)
{
    int i;

    for (i = 0; i < ipc->nservers; i++)
        if (ipc->servers[i].proc_id == target)
            return ipc->servers[i].fd;
    return get_target_fd(ipc, target);
}

/* @0x1f534 */
int aipc_call_to(struct aipc *ipc, int target, int *call_ret, const char *name, int argc,
                 struct aipc_ipc_arg *args)
{
    int fd, r;

    fd = find_target_fd(ipc, target);
    if (fd < 0) {
        LOGE(aipc, 0x308, "call \"%s\" to unknown target %d\n", name, target);
        return -1;
    }
    if (ipc->last_additional)
        free(ipc->last_additional);
    ipc->last_additional = NULL;
    ipc->last_additional_size = 0;

    pthread_mutex_lock(&ipc->call_lock);
    r = ipc_send_and_wait(fd, call_ret, name, argc, args, &ipc->last_additional,
                          &ipc->last_additional_size);
    pthread_mutex_unlock(&ipc->call_lock);
    return r;
}

/* @0x1f600: hand over (and forget) the additional data of the last call */
int aipc_get_last_additional_data(struct aipc *ipc, void **data)
{
    if (!ipc->last_additional) {
        *data = NULL;
        return 0;
    }
    *data = ipc->last_additional;
    ipc->last_additional = NULL;
    return ipc->last_additional_size;
}

static int count_args(const struct aipc_ipc_arg *args)
{
    int n = 0;

    while (args[n].type != ipc_argtype_end)
        n++;
    return n;
}

/* @0x1f61c: args terminated by type 0 */
int aipc_calla(struct aipc *ipc, int target, int *call_ret, const char *name,
               struct aipc_ipc_arg *args)
{
    return aipc_call_to(ipc, target, call_ret, name, count_args(args), args);
}

/* @0x1f640 */
int aipc_callv(struct aipc *ipc, int target, int *call_ret, const char *name, va_list ap)
{
    struct aipc_ipc_arg *args;
    int argc, r;

    LOGD(aipc, 0x392, "call %s()\n", name);
    args = get_ipc_argv(ap, &argc);
    if (!args) {
        LOGE(aipc, 0x396, "wrong argv for %s()\n", name);
        return -1;
    }
    r = aipc_call_to(ipc, target, call_ret, name, argc, args);
    free(args);
    return r;
}

/* @0x1f6f8: synchronous call, returns the remote error and stores its return value */
int aipc_call(struct aipc *ipc, int target, int *call_ret, const char *name, ...)
{
    va_list ap;
    int r;

    va_start(ap, name);
    r = aipc_callv(ipc, target, call_ret, name, ap);
    va_end(ap);
    return r;
}

/* @0x1f718: one-way request, no reply is read */
int aipc_send_to(struct aipc *ipc, int target, const char *name, int argc,
                 struct aipc_ipc_arg *args)
{
    int fd, r;

    fd = find_target_fd(ipc, target);
    if (fd < 0) {
        LOGE(aipc, 0x3b5, "call \"%s\" to unknown target %d\n", name, target);
        return -1;
    }
    pthread_mutex_lock(&ipc->call_lock);
    r = ipc_send(fd, 0, name, argc, args);
    pthread_mutex_unlock(&ipc->call_lock);
    return r;
}

/* @0x1f7cc */
int aipc_senda(struct aipc *ipc, int target, const char *name, struct aipc_ipc_arg *args)
{
    return aipc_send_to(ipc, target, name, count_args(args), args);
}

/* @0x1f7f0 */
int aipc_sendv(struct aipc *ipc, int target, const char *name, va_list ap)
{
    struct aipc_ipc_arg *args;
    int argc, r;

    LOGD(aipc, 0x3cf, "send %s()\n", name);
    args = get_ipc_argv(ap, &argc);
    if (!args) {
        LOGE(aipc, 0x3d3, "wrong argv for %s()\n", name);
        return -1;
    }
    r = aipc_send_to(ipc, target, name, argc, args);
    free(args);
    return r;
}

/* @0x1f8a4 */
int aipc_send(struct aipc *ipc, int target, const char *name, ...)
{
    va_list ap;
    int r;

    va_start(ap, name);
    r = aipc_sendv(ipc, target, name, ap);
    va_end(ap);
    return r;
}

/* @0x1f8c0 */
int aipc_register_cb(struct aipc *ipc, int (*cb)(void *priv, int cfd), void *priv)
{
    ipc->cb_client_disconnect = cb;
    ipc->cb_priv = priv;
    return 0;
}

/* @0x1f8c8 */
int aipc_get_proc_id(struct aipc *ipc)
{
    return ipc->proc_id;
}

/* @0x1f8cc: fds == NULL: return the count (and freeze the server count);
 * otherwise fill [listen fd][servers (HUP only)][clients] and return the count.
 */
int aipc_get_pollfd(struct aipc *ipc, struct pollfd *fds)
{
    int n, i, k;

    if (!fds) {
        ipc->npoll_servers = ipc->nservers;
        return ipc->nclients + 1 + ipc->nservers;
    }
    n = ipc->nclients + 1 + ipc->npoll_servers;
    memset(fds, 0, n * sizeof(*fds));
    fds[0].fd = ipc->listen_fd;
    fds[0].events = POLLIN;
    k = 1;
    for (i = 0; i < ipc->npoll_servers; i++, k++) {
        fds[k].fd = ipc->servers[i].fd;
        fds[k].events = 0;
    }
    for (i = 0; i < ipc->nclients; i++, k++) {
        fds[k].fd = ipc->client_fds[i];
        fds[k].events = POLLIN;
    }
    return n;
}

/* @0x1f94c: queue extra reply data from inside a command handler (takes ownership) */
int aipc_append_additional(struct aipc *ipc, void *data, unsigned int size)
{
    struct aipc_additional *a;

    a = calloc(1, sizeof(*a));
    if (!a) {
        LOGE(aipc, 0x426, "nomem\n");
        return -1;
    }
    a->data = data;
    a->size = size;
    link_add_tail(&ipc->additional_list, &a->list);
    return 0;
}

static void free_additional(struct aipc *ipc)
{
    while (!link_empty(&ipc->additional_list)) {
        struct aipc_additional *a =
            link_container(struct aipc_additional, list, ipc->additional_list.next);
        link_del(&a->list);
        free(a->data);
        free(a);
    }
}

/* ipc_read (inlined): read and decode one request from client fd */
static struct ipc_desc *ipc_read(int fd)
{
    struct ipc_msg_header hdr;
    struct ipc_desc *d;
    unsigned char *buf, *p;
    unsigned int i;
    ssize_t n;

    LOGF(aipc, log_level_trace, "ipc_read", 0xf6, "read header.. from fd%d\n", fd);
    n = read(fd, &hdr, sizeof(hdr));
    if (n != sizeof(hdr)) {
        LOGF(aipc, log_level_error, "ipc_read", 0xfa, "read size differ. %zd != %zd\n",
             n, (ssize_t)sizeof(hdr));
        return NULL;
    }
    LOGF(aipc, log_level_debug, "ipc_read", 0xfe, "header: desc_size %d, arg_count %d\n",
         hdr.write_size, hdr.argc);
    buf = malloc(hdr.write_size);
    if (!buf) {
        LOGF(aipc, log_level_error, "ipc_read", 0x102,
             "no mem for read_buffer. desc_size %d, arg_count %d\n", hdr.write_size, hdr.argc);
        return NULL;
    }
    n = read(fd, buf, hdr.write_size);
    if ((unsigned int)n != hdr.write_size) {
        LOGF(aipc, log_level_error, "ipc_read", 0x109, "no desc. %zd, %d. %s\n", n,
             hdr.write_size, strerror(errno));
        free(buf);
        return NULL;
    }
    d = malloc((hdr.argc + 1) * sizeof(struct aipc_ipc_arg));
    if (!d) {
        LOGF(aipc, log_level_error, "ipc_read", 0x110,
             "no mem for desc. desc_size %d, arg_count %d\n", hdr.write_size, hdr.argc);
        free(buf);
        return NULL;
    }
    d->flags = hdr.flags;
    d->name_size = *(unsigned int *)buf;
    LOGF(aipc, log_level_trace, "ipc_read", 0x119, "name size %d\n", d->name_size);
    if (d->name_size < 2) {
        LOGF(aipc, log_level_error, "ipc_read", 0x11c, "no name.\n");
        goto bad;
    }
    d->name = (char *)buf + 4;
    d->name[d->name_size - 1] = '\0';
    d->argc = hdr.argc;
    d->args = (struct aipc_ipc_arg *)(d + 1);

    p = buf + 4 + d->name_size;
    for (i = 0; i < hdr.argc; i++) {
        struct aipc_ipc_arg *a = &d->args[i];
        unsigned int *w = (unsigned int *)ALIGN8((uintptr_t)p);

        a->type = w[0];
        a->size = w[1];
        LOGF(aipc, log_level_trace, "ipc_read", 0x135, "args[%d] type %d, size %d\n",
             i, a->type, a->size);
        switch (a->type) {
        case ipc_argtype_bool:
        case ipc_argtype_int:
            a->i = w[2];
            break;
        case ipc_argtype_longlong:
        case ipc_argtype_double:
            memcpy(&a->ll, &w[2], 8);
            break;
        case ipc_argtype_str:
            if (a->size == 0) {
                LOGF(aipc, log_level_error, "ipc_read", 0x14b, "no argument string at %s\n",
                     d->name);
                goto bad;
            }
            a->s = (char *)&w[2];
            a->s[a->size - 1] = '\0';
            break;
        case ipc_argtype_mem:
            a->m = &w[2];
            break;
        default:
            LOGF(aipc, log_level_error, "ipc_read", 0x159, "unknown argument type. %d\n",
                 a->type);
            goto bad;
        }
        p = (unsigned char *)&w[2] + a->size;
    }
    d->buf = buf;
    return d;

bad:
    free(buf);
    free(d);
    return NULL;
}

/* write_additional (inlined): send the queued additional buffers back to the caller */
static int write_additional(struct aipc *ipc, int fd, unsigned int total, int count)
{
    struct link *l;
    unsigned char *all;
    unsigned int off = 0;
    ssize_t n;

    LOGF(aipc, log_level_debug, "write_additional", 0x455, "write total %d(%d) to fd%d\n",
         total, count, fd);
    if (count == 1) {
        struct aipc_additional *a =
            link_container(struct aipc_additional, list, ipc->additional_list.next);
        n = write(fd, a->data, a->size);
        if (n != (ssize_t)a->size) {
            LOGF(aipc, log_level_error, "write_additional", 0x461, "write() error. %d, %d, %s\n",
                 (int)n, a->size, strerror(errno));
            free_additional(ipc);
            return -1;
        }
        free_additional(ipc);
        return 0;
    }
    all = malloc(total);
    if (!all) {
        LOGF(aipc, log_level_error, "write_additional", 0x46c, "no mem. %d, %d\n", total, count);
        free_additional(ipc);
        return -1;
    }
    for_each_link(l, &ipc->additional_list) {
        struct aipc_additional *a = link_container(struct aipc_additional, list, l);
        memcpy(all + off, a->data, a->size);
        off += a->size;
        if (off >= total)
            break;
    }
    n = write(fd, all, total);
    free(all);
    if ((unsigned int)n != total) {
        LOGF(aipc, log_level_error, "write_additional", 0x480, "write() error. %d, %d, %s\n",
             (int)n, total, strerror(errno));
        free_additional(ipc);
        return -1;
    }
    free_additional(ipc);
    return 0;
}

/* ipc_process (inlined): run the registered command and send the reply.
 * cfd points at the client's slot in ipc->client_fds. */
static void ipc_process(struct aipc *ipc, int *cfd)
{
    struct ipc_reply reply = { 0, 0, 0 };
    struct ipc_desc *d;
    struct aipc_command *cmd = NULL;
    struct link *l;
    int count = 0;

    LOGF(aipc, log_level_trace, "ipc_process", 0x495, "read ipc..\n");
    d = ipc_read(*cfd);
    if (!d)
        return;
    LOGF(aipc, log_level_debug, "ipc_process", 0x49a, "got ipc %s(), %d args\n", d->name,
         d->argc);

    for_each_link(l, &ipc->command_list) {
        struct aipc_command *c = link_container(struct aipc_command, list, l);
        if (!strcmp(c->name, d->name)) {
            cmd = c;
            break;
        }
    }

    if (!cmd) {
        LOGF(aipc, log_level_error, "ipc_process", 0x4a8,
             "no command register for %s(), from client fd %d\n", d->name, *cfd);
        reply.error = -1;
    } else {
        free_additional(ipc);
        reply.ret = cmd->func(cmd->arg, *cfd, d->name, d->argc, d->args);
        if (!(d->flags & AIPC_FLAG_NO_ADDITIONAL)) {
            for_each_link(l, &ipc->additional_list) {
                count++;
                reply.additional_size +=
                    link_container(struct aipc_additional, list, l)->size;
            }
        }
        LOGF(aipc, log_level_debug, "ipc_process", 0x4b4,
             "return value of %s(), %d, additional %d\n", d->name, reply.ret,
             reply.additional_size);
    }

    if (d->flags & AIPC_FLAG_NEED_RESPONSE) {
        if (write(*cfd, &reply, sizeof(reply)) != sizeof(reply))
            LOGF(aipc, log_level_error, "ipc_process", 0x4be, "write(response) failed. %s\n",
                 strerror(errno));
        else if (count && write_additional(ipc, *cfd, reply.additional_size, count) < 0)
            LOGF(aipc, log_level_error, "ipc_process", 0x4c7, "write(additional) failed. %d\n",
                 reply.additional_size);
    }
    free(d->buf);
    free(d);
}

/* @0x1f9c4: handle poll() results for the set built by aipc_get_pollfd */
int aipc_handle_pollfd(struct aipc *ipc, struct pollfd *fds)
{
    int nclients = ipc->nclients;
    int i, j;

    i = 1;
    for (j = 0; j < ipc->npoll_servers; j++, i++)
        LOGT(aipc, 0x4d9, "servers fds[%d].fd %d, events 0x%x, revents 0x%x\n",
             i, fds[i].fd, fds[i].events, fds[i].revents);
    i = 1 + ipc->npoll_servers;
    for (j = 0; j < nclients; j++, i++)
        LOGT(aipc, 0x4e0, "clients fds[%d].fd %d, events 0x%x, revents 0x%x\n",
             i, fds[i].fd, fds[i].events, fds[i].revents);

    /* outgoing connections that hung up */
    i = 1;
    for (j = 0; j < ipc->npoll_servers; j++, i++) {
        if (!(fds[i].revents & POLLHUP))
            continue;
        if (j == 0) {
            LOGE(aipc, 0x4ed, "lost controller connection. stop.\n");
            exit(1);
        }
        LOGI(aipc, 0x4f1, "disconnect to server, %d\n", ipc->servers[j].proc_id);
        close(ipc->servers[j].fd);
        ipc->nservers--;
        if (ipc->nservers - j > 0)
            memmove(&ipc->servers[j], &ipc->servers[j + 1],
                    (ipc->nservers - j) * sizeof(ipc->servers[0]));
    }

    /* new client */
    if (fds[0].revents & POLLIN) {
        int cfd;

        LOGI(aipc, 0x503, "get listening client. accept from fd%d.\n", ipc->listen_fd);
        cfd = accept(ipc->listen_fd, NULL, NULL);
        if (cfd < 0) {
            LOGE(aipc, 0x507, "accept() failed. %s\n", strerror(errno));
        } else {
            int *c = realloc(ipc->client_fds, (ipc->nclients + 1) * sizeof(int));

            if (!c) {
                LOGE(aipc, 0x510, "no mem for cinfo. %d\n", ipc->nclients);
                close(cfd);
            } else {
                c[ipc->nclients] = cfd;
                ipc->nclients++;
                ipc->client_fds = c;
                LOGI(aipc, 0x518, "client added. %d, fd %d\n", ipc->nclients, cfd);
            }
        }
    }

    /* requests (only from clients known when poll() was set up) */
    i = 1 + ipc->npoll_servers;
    for (j = 0; j < nclients; j++, i++) {
        if ((fds[i].revents & POLLIN) && !(fds[i].revents & POLLHUP)) {
            LOGT(aipc, 0x523, "read client %d\n", j);
            ipc_process(ipc, &ipc->client_fds[j]);
        }
    }

    /* hung-up clients, last to first */
    i = ipc->npoll_servers + nclients;
    for (j = nclients - 1; j >= 0; j--, i--) {
        if (!(fds[i].revents & POLLHUP))
            continue;
        LOGI(aipc, 0x530, "client %d, fd %d disconnected.\n", j, ipc->client_fds[j]);
        if (ipc->cb_client_disconnect)
            ipc->cb_client_disconnect(ipc->cb_priv, ipc->client_fds[j]);
        close(ipc->client_fds[j]);
        ipc->nclients--;
        if (ipc->nclients - j > 0)
            memmove(&ipc->client_fds[j], &ipc->client_fds[j + 1],
                    (ipc->nclients - j) * sizeof(int));
    }
    return 0;
}

/* @0x205b4 */
int aipc_register_command(struct aipc *ipc, const char *name, aipc_command_fn func, void *arg)
{
    struct aipc_command *c;

    c = calloc(1, sizeof(*c));
    if (!c) {
        LOGE(aipc, 0x54c, "nomem\n");
        return -1;
    }
    c->name = name;
    c->func = func;
    c->arg = arg;
    link_add_tail(&ipc->command_list, &c->list);
    return 0;
}
