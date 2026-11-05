/*
 * dsp2 property diagnostics and the integer GET_PROP distinction.
 * Reconstruction: audio_a0_dsp2.bin, sha256
 * c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d;
 * main image sha256 d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd.
 * Evidence: build/scratch/kprop/{audreg_print_property,get_prop,prop_str}.lst,
 * ipcm_writedebug.od, call_function.od; tools/xtobjdump.sh for branch slots.
 *
 * audreg_print_property emits magic start/end via logm_message @6007af14 ->
 * IPC_WriteDebugV @6006f014 -> AUD_IPCM_WriteDebug @6006ee18. It formats ALL
 * properties (default, class, runtime descriptors), invoking string getters.
 * No generic module mutex is taken by aud_module_prop_str @60075d24.
 * A particular custom getter can still lock or block. Formatting one property
 * via integer GET_PROP avoids this all-property traversal and the log capture.
 *
 * The debug writer does not guarantee delivery: ring full @6006ef30/6006ef53
 * drops the message, sets *(u8 *)0x601282b5=1, returns 7; the next successful
 * message gets flag bit1 (DROPPED). Neither marker send here checks its result.
 * A missing marker leaves the kernel's proc read waiting indefinitely.
 *
 * Command scheduling: aimc_start @60079efc registers the IRQ callback at
 * 60079c2c (misnamed aimc_ipc_header_get in the recovered symbol table).
 * It copies incoming headers to a list and @60079c98..60079c9e queues scheduler
 * job 0x60101298 on queue 1. That job's function is ipc_header_process @60079974,
 * which calls IMC_ProcessHeader @6006da08 -> aimc_dispatch_command @6007a450.
 * sched_init @6007c46c creates queue 1's thread at priority 10; sched_process
 * @6007c3f8 invokes one job at a time. Command getters and the proc dump both
 * need this queue to run; an asynchronous ARM request cannot bypass a stuck DSP.
 * Queue 0 is run by sched_mai in the main thread. These are static facts, not
 * evidence that scheduler queue 1 is blocked during the user's playback.
 *
 * GET_PROP @60076444 returns aud_module_get_prop @60075b74 as the IMC result:
 * gets-only descriptors return -1 (60075c3e..60075c64); geti @60075c18 works;
 * int offset load @60075cab and bool byte load @60075cd6 work. No response text
 * parsing is involved. Delay queued-length is gets-only, unlike fader's
 * queued-length which supplies both gets and geti. Gain es is not a descriptor.
 */
#include <stddef.h>
#include <stdint.h>

extern unsigned long strtoul(const char *, char **, int);
extern void *ModuleRegistry_SearchInstByUid(unsigned uid);
extern int logm_message(const char *fmt, ...);
extern char *aud_module_prop_str(void *module, const char *format);
extern void aud_free(void *);

/* @0x600708d4: argv[1] is the kernel's magic; argv[2] is a numeric uid. */
int audreg_print_property(int argc, char **argv)
{
    void *module;
    char *text;
    if (argc < 3)
        return -1;
    module = ModuleRegistry_SearchInstByUid((unsigned)strtoul(argv[2], NULL, 0));
    logm_message("%s: start\n", argv[1]);
    if (module) {
        text = aud_module_prop_str(module, "%s=%s\n");
        logm_message("%s", text);
        aud_free(text); /* actual free target 600e9550; duplicate symbol alias */
    } else {
        logm_message("no instance for uid %s\n", argv[2]);
    }
    logm_message("%s: end\n", argv[1]);
    return 0;
}
