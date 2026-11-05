/*
 * dsp1 "fader" module, registered as "fader", "queue" and "input_queue". The kernel's
 * output_ARC chain starts with the "queue" instance (queue -> outctrlbin -> output_arc).
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp1.bin
 *   sha256 8f4e6d88a21b8486f3e15a866a989281f5f9cea564b5767cee24ca01277ae2f4
 * The dsp1 code at 0x6010d690..0x60110290 is the same source as dsp2's fader
 * (0x60084b90..0x60087770, reconstructed in ../dsp2/fader_queue.c). Of 43 functions, 9 are
 * byte-identical and 30 have the same length; the differences are only call forms
 * (call8 vs l32r+callx8) and relocations (tmp/a17/cmpfn.py, insdiff.py). The other 4
 * are 8 bytes longer on dsp1 because strncmp/logmm are reached through l32r+callx8:
 *   _fader_push_data 0x6010d81c, underflow 0x6010e118, set_block_queue 0x6010ee18,
 *   _fader_add_pe_and_signal 0x6010f6e0.
 * Read ../dsp2/fader_queue.c for the logic. dsp1 -> dsp2 address map:
 *   6010d690 debug_fader_input_queue_data  60084b90 | 6010e5e8 fadein_bypass      60085ad8
 *   6010d6e8 _fader_write_metadata         60084be8 | 6010e610 _fader_process_data_fadein 60085b00
 *   6010d81c _fader_push_data              60084d1c | 6010e7ec add_zero           60085cdc
 *   6010d8e0 pending_events_add            60084dd8 | 6010e860 _fader_process_data_wait 60085d50
 *   6010d9e8 pending_events_update         60084ee0 | 6010ed10 process_data_stop  60086200
 *   6010da24 pending_events_check          60084f1c | 6010ed28 fader_output_work  60086218
 *   6010db80 pending_events_empty          60085078 | 6010edb8 queue_module_unblock 600862a8
 *   6010dbf4 _fader_process_data_running   600850ec | 6010ee18 set_block_queue    60086308
 *   6010df2c fadeout_32bit_2ch             60085424 | 6010ef1c fader_start        60086404
 *   6010df6c fadeout_32bit_4ch             60085464 | 6010efac fader_stop         60086494
 *   6010dfcc fadeout_32bit_8ch             600854c4 | 6010f02c fader_bypass_work  60086514
 *   6010e06c fadeout_32bit_10ch            60085564 | 6010f1fc fader_bypass_start 600866e4
 *   6010e0f0 fadeout_bypass                600855e8 | 6010f290 fader_chain        60086778
 *   6010e118 _fader_process_data_fadeout   60085610 | 6010f6e0 _fader_add_pe_and_signal 60086bc8
 *   6010e418 fadein_32bit_2ch              60085908 | 6010f82c fader_event        60086d0c
 *   6010e458 fadein_32bit_4ch              60085948 | 6010fda8 fader_cmd          60087288
 *   6010e4bc fadein_32bit_8ch              600859ac | 6010fe80 fader_disconnect_port 60087360
 *   6010e560 fadein_32bit_10ch             60085a50 | 6010ff30 fader_deleting_port 60087410
 *   6010ff5c fader_constructor 6008743c | 60110060 fader_destructor 60087540
 *   60110170/6011018c/601101b0 fader_prop_* 60087650/6008766c/60087690
 *   601101f0 send_underflow_event 600876d0 | 60110260 fader_init 60087740
 * (functions.tsv names 0x6010df6c "fader_cmd", 0x6010e0f0 "fader_event_first",
 *  0x6010e118 "underflow"; the names above follow ../dsp2/fader_queue.c.)
 * ES behaviour (from the dsp2 reconstruction, same code): fadein/fadeout_bypass copy ES
 * unscaled. fader_chain drops data on a disabled port or while flushing, and otherwise
 * applies back-pressure. The wait state queues zero front padding. The fadeout tail adds
 * zeros. The "queue" variant does not raise underflow_input. Nothing filters ES by
 * codec or rate.
 */

extern char fader_class[];          /* @0x6027f080 (descriptor table - 0x60) */
extern char fader_desc_fader[];     /* @0x6027f0e0 */
extern char fader_desc_queue[];     /* @0x6027f0f8 */
extern char fader_desc_input_queue[]; /* @0x6027f110 */
extern void ModuleRegistry_RegisterNamef(void *cls, void *desc, const char *fmt, ...);

/* @0x60110260 */
void fader_init(void)
{
    ModuleRegistry_RegisterNamef(fader_class, fader_desc_fader, "fader");
    ModuleRegistry_RegisterNamef(fader_class, fader_desc_queue, "queue");
    ModuleRegistry_RegisterNamef(fader_class, fader_desc_input_queue, "input_queue");
}
