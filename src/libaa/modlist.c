/*
 * Static (legacy, compile-time) module list: per module id its name, type, the core it
 * runs on, and its memory definition. The tables themselves are data exported by the
 * binary (gModuleInfo) or initialised data (module memory info); only the accessors are
 * reconstructed here.
 *
 * RECONSTRUCTION from orig/usr/lib/libaa.so.2.0.0
 *   sha256 301d682fa52c1f072f1b7b0d8863a545df69b235d5454ae0f23b89de83eebdd2
 *
 * Evidence: Ghidra build/decomp/libaa.so.2.0.0.c @0x28624..@0x28874; gModuleInfo = .data
 * 0x3e390 (9648 bytes = 201 x 0x30, symbol table); platform word .data 0x3c424 (= 1);
 * module memory info .data 0x3c428 (201 x 0x28); type-name table .data.rel.ro 0x3b734.
 */
#include <string.h>

#include "buffers.h"

/* Contents of the 201-entry memory-definition table (initialised .data at 0x3c428) are
 * deliberately not reproduced (no tables copied from orig/); only its shape is. */
static ModuleMemInfo gModuleMemInfo[ADEC_MODULE_INFO_COUNT];

static unsigned int module_list_platform = ADEC_CORE_ARM;

/* ADEC_MODULE_TYPE names; holes are types that were removed from the enum */
static const char *const module_type_name[ADEC_MOD_TYPE_COUNT] = {
    "NOT_DEF", "SOURCE", "REFORM", "SINK", "DEC", "ENC", "RENDERER", "PCMCVT", "MIX", "SE",
    "DAP", "DTSVIRTUALX", "DDPENC", "LIPSYNC", "INPUTCTRL", "OUTCTRL", "ESOUTCTRL", "BYPASS",
    "MAN", "DBG_MOR", "SOLA", "CTRLSRC", "DTOCTRL", NULL, NULL, "EXTBUF", "RTS", "GLUE",
    "GAIN", "FADER", NULL, "DELAY", NULL, "INVALID",
};

/* @0x28624 */
int ModuleList_SetPlatform(unsigned int core)
{
    module_list_platform = core;
    return 0;
}

/* @0x28634 */
unsigned int ModuleList_GetPlatform(void)
{
    return module_list_platform;
}

/* @0x28640: processor of a module uid. Dynamic uids carry it in bits 24..31; legacy
 * module ids (proc 0, id <= 200) are looked up in the static table. */
unsigned int ModuleList_FindModule(unsigned int muid)
{
    unsigned int proc = muid >> 24;

    if (proc)
        return proc;
    if (muid > ADEC_MODULE_INFO_COUNT - 1)
        return proc;
    return gModuleInfo[muid].location;
}

/* @0x28668 */
int ModuleList_GetModuleInfo(unsigned int module, ModuleInfo *info)
{
    if (!info) {
        LOGE(aud, 200, "assert failed. (%s)\n", "_info != NULL");
        return ADEC_ERR_NULL;
    }
    if (module - 1 >= ADEC_MODULE_INFO_COUNT - 1)
        return ADEC_ERR_RANGE;
    *info = gModuleInfo[module];
    return 0;
}

/* @0x286f8 */
int ModuleList_SetModuleInfo(unsigned int module, ModuleInfo *info)
{
    if (!info) {
        LOGE(aud, 0xd6, "assert failed. (%s)\n", "_info != NULL");
        return ADEC_ERR_NULL;
    }
    if (module - 1 >= ADEC_MODULE_INFO_COUNT - 1)
        return ADEC_ERR_RANGE;
    gModuleInfo[module] = *info;
    return 0;
}

/* @0x28788: ids of all modules of a type, at most max; returns how many */
int ModuleList_GetModuleList(unsigned int type, unsigned int *result, int max)
{
    int i, n = 0;

    if (!result) {
        LOGE(aud, 0xe6, "assert failed. (%s)\n", "_result_array != NULL");
        return ADEC_ERR_NULL;
    }
    for (i = 0; i < ADEC_MODULE_INFO_COUNT && n < max; i++)
        if (gModuleInfo[i].type == type)
            result[n++] = i;
    return n;
}

/* @0x28808 */
const char *ModuleList_GetModuleName(unsigned int module)
{
    if (module < ADEC_MODULE_INFO_COUNT)
        return gModuleInfo[module].name;
    return "";
}

/* @0x28834 */
const char *ModuleList_GetModuleTypeName(unsigned int type)
{
    if (type >= ADEC_MOD_TYPE_COUNT)
        return "INVALID";
    if (!module_type_name[type])
        return "UNKNOWN";
    return module_type_name[type];
}

/* @0x2885c */
ModuleMemInfo *ModuleList_GetModuleMemoryInfo(unsigned int module)
{
    if (module - 1 >= ADEC_MODULE_INFO_COUNT - 1)
        return NULL;
    return &gModuleMemInfo[module];
}
