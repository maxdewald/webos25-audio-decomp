/*
 * dsp2 "bypass" module: the last module of the renderer ES path
 * (... delay -> gain -> bypass -> output).
 * RECONSTRUCTION from lib/firmware/audio_a0_dsp2.bin
 *   sha256 c32c0f65f8a4238075530417dfdda012c64317a1a9270a4e7bf80d5ce7db134d
 *   main segment build/fw/audio_a0_dsp2.main_60000000.bin
 *   sha256 d1735e7ad803d5da3f4dfd778a5936d3f962a91e1d26f896ec1dba8423b7d3cd
 * Linked at 0x60000000; fully decoded, no FLIX in these functions.
 * Class descriptor @0x60101d88 {name "bypass", size 0xf0 (core only), chain
 * 0x60084ad0, event 0x60084ae4, cmd 0x60084af8, ctor 0x60084b10, dtor 0x60084b18,
 * props none}. Registered by bypass_init via the ModuleRegistry entry @0x6005eff0.
 *
 * Observed: no data-type test at all, so PCM, ES and metadata are forwarded
 * unchanged. The only possible drops are the core ones in aud_module_push_data
 * (drop-output-data / drop-output-meta / no output port, see module_core.c).
 * Idle snapshot (evidence/runtime/idle-state.txt, bypass_04160001): drop-output-data
 * false, block-module false; only in0 is connected (from gain_041b0001).
 */
struct aud_module;
struct aud_port { int pad; struct aud_module *module; };
struct aud_data;
struct aud_event;

extern int aud_module_push_data(struct aud_module *, int out_port, struct aud_data *); /* @0x600724d8 */
extern int aud_module_default_event(struct aud_module *, struct aud_port *, struct aud_event *); /* @0x60076608 */
extern int aud_module_default_command(struct aud_module *, unsigned, void *, unsigned);      /* @0x60076444 */
extern int ModuleRegistry_Register(const void *class_desc);
extern const char bypass_class[];                    /* @0x60101d88 */

/* @0x60084ad0 bypass_chain: forward everything to out0 */
int bypass_chain(struct aud_port *port, struct aud_data *data)
{
    aud_module_push_data(port->module, 0, data);
    return 0;
}

/* @0x60084ae4 bypass_event */
int bypass_event(struct aud_module *m, struct aud_port *port, struct aud_event *e)
{
    return aud_module_default_event(m, port, e);
}

/* @0x60084af8 bypass_cmd */
int bypass_cmd(struct aud_module *m, unsigned cmd, void *param, unsigned len)
{
    return aud_module_default_command(m, cmd, param, len);
}

/* @0x60084b10 bypass_constructor */
int bypass_constructor(struct aud_module *m)
{
    (void)m;
    return 0;
}

/* @0x60084b18 bypass_destructor */
void bypass_destructor(struct aud_module *m)
{
    (void)m;
}

/* @0x60084b20 bypass_init */
void bypass_init(void)
{
    ModuleRegistry_Register(bypass_class);
}
