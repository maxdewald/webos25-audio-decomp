/*
 * libaabase: plugin directory loader (dlload_dir / dlload_dirs).
 *
 * RECONSTRUCTION from orig/usr/lib/libaabase.so.1.0.0
 *   sha256 0d150201c8fb1dbda6c6769ed5d9254c21c98098fca31e2db33eb0b861f88806
 * Evidence: decomp/libaabase.so.1.0.0.c, strings "%s/%s" @0x10e44,
 * "%s.%d: cannot load \"%s\". %s\n" @0x10e4c, "load_dl" @0x10e6c (used as __func__,
 * line 25: an inlined helper of that name).
 */
#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include "aabase.h"

static inline void load_dl(const char *path)
{
    if (!dlopen(path, RTLD_LAZY))
        fprintf(stderr, "%s.%d: cannot load \"%s\". %s\n", __func__, 25, path, dlerror());
}

/* @0x10a08 */
int dlload_dir(const char *dir)
{
    DIR *d;
    struct dirent *de;
    size_t dir_len;

    if (!dir || !(d = opendir(dir)))
        return -1;

    dir_len = strlen(dir);
    while ((de = readdir(d)) != NULL) {
        if (de->d_name[0] == '.')
            continue;
        {
            char path[dir_len + strlen(de->d_name) + 4];

            sprintf(path, "%s/%s", dir, de->d_name);
            load_dl(path);
        }
    }
    closedir(d);
    return 0;
}

/* @0x10ae0 */
int dlload_dirs(const char **dirs)
{
    for (; *dirs; dirs++)
        dlload_dir(*dirs);
    return 0;
}
