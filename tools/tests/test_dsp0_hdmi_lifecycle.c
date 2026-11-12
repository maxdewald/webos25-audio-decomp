/* Real recovered HDMI setter/start bodies; MMIO is backed by host memory. */
// @cflags -fno-pie -no-pie
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern volatile uint32_t *dsp0_hdmi_registers;
#define DSP0_HDMI_REGISTERS dsp0_hdmi_registers
#include "../../src/dsp0/hdmi.c"

/* Weak declarations let the first RED run fail as a missing-behavior assertion. */
extern int hdmi_set_mode(dsp0_hdmi *, const void *, const char *) __attribute__((weak));
extern int hdmi_set_data_drop_mode(dsp0_hdmi *, const void *, const char *) __attribute__((weak));
extern int hdmi_set_start(dsp0_hdmi *, const void *, const char *) __attribute__((weak));
extern int _hdmi_start(dsp0_hdmi *, int) __attribute__((weak));
extern int _hdmi_InitBC(dsp0_hdmi *) __attribute__((weak));

static volatile uint32_t registers[0x400 / 4], bc[4], bc_control, bc_reset;
static uint32_t descriptor[6];
volatile uint32_t *dsp0_hdmi_registers = registers;
static unsigned parser_resets, irq_enables, irq_disables, irq_registrations;
static uint32_t last_irq, last_owner;
static void (*last_handler)(void *);
static void *last_handler_arg;
uint32_t AddrTrans_GetAddr(uint32_t address) { return address + 0x100000; }
void HdmiParser_Init_Dtsx_SycnType(void) { parser_resets++; }
int xos_interrupt_enable(uint32_t irq) { last_irq = irq; irq_enables++; return -9; }
int xos_interrupt_disable(uint32_t irq) { last_irq = irq; irq_disables++; return -8; }
void hdmi_handle_irq(void *arg) { (void)arg; }
int irq_register(uint32_t irq, uint32_t owner, void (*handler)(void *), void *arg)
{
    last_irq = irq; last_owner = owner; last_handler = handler;
    last_handler_arg = arg; irq_registrations++; return -7;
}
static uint32_t address32(const volatile void *p)
{
    assert((uintptr_t)p <= UINT32_MAX);
    return (uint32_t)(uintptr_t)p;
}
static void fixture(dsp0_hdmi *m, unsigned channel, unsigned port)
{
    memset(m, 0, sizeof(*m));
    memset((void *)registers, 0, sizeof(registers));
    descriptor[0] = channel; descriptor[1] = 9;
    descriptor[2] = address32(bc); descriptor[3] = address32(&bc_control);
    descriptor[4] = address32(&bc_reset);
    bc[0] = 0x100000; bc[1] = bc[0] + 0x8000;
    bc[2] = bc[0] + 0x200; bc[3] = bc[0] + 0x20;
    bc_control = 17; bc_reset = 18;
    uint32_t desc = address32(descriptor);
    memcpy((uint8_t *)m + 0xf0, &desc, 4);
    m->hdmi_port = port; m->sampling_freq = 48000; m->data_drop_mode = 2;
    parser_resets = irq_enables = irq_disables = irq_registrations = 0;
}
uint32_t _Stoul(const char *s, char **end, int base)
{
    return (uint32_t)strtoul(s, end, base);
}
static uint32_t word(const dsp0_hdmi *m, size_t off)
{
    uint32_t v;
    memcpy(&v, (const uint8_t *)m + off, 4);
    return v;
}
static void set_word(dsp0_hdmi *m, size_t off, uint32_t v)
{
    memcpy((uint8_t *)m + off, &v, 4);
}
static void set_byte(dsp0_hdmi *m, size_t off, uint8_t v)
{
    ((uint8_t *)m)[off] = v;
}
static uint8_t byte(const dsp0_hdmi *m, size_t off)
{
    return ((const uint8_t *)m)[off];
}
static void test_mode_sequencing(void)
{
    int (*mode_setter)(dsp0_hdmi *, const void *, const char *) = hdmi_set_mode;
    assert(mode_setter && "mode setter not reconstructed");
    dsp0_hdmi m = {0};
    set_word(&m, 0x160, 19);
    set_word(&m, 0x1ac, 23);
    set_byte(&m, 0x155, 1); set_byte(&m, 0x156, 1);
    set_word(&m, 0x1e0, 1); /* Mid-stream writes do not restart or invalidate FIRST. */
    assert(hdmi_set_mode(&m, NULL, "7") == 0);
    assert(word(&m, 0x1a0) == 7 && word(&m, 0x160) == 19);
    assert(byte(&m, 0x194) == 1 && word(&m, 0x1ac) == 308800);
    assert(hdmi_set_mode(&m, NULL, "0") == 0);
    assert(word(&m, 0x160) == 1 && byte(&m, 0x194) == 0);
    assert(word(&m, 0x1ac) == 308800); /* Deliberately stale. */
    assert(hdmi_set_mode(&m, NULL, "6") == 0);
    assert(word(&m, 0x160) == 1 && word(&m, 0x1ac) == 77200);
    assert(hdmi_set_mode(&m, NULL, "4") == 0);
    assert(word(&m, 0x160) == 4 && byte(&m, 0x194) == 1);
    assert(hdmi_set_mode(&m, NULL, "8") == 0);
    assert(word(&m, 0x160) == 4 && byte(&m, 0x194) == 0);
    assert(word(&m, 0x1ac) == 308800);
    assert(hdmi_set_mode(&m, NULL, "0x3") == 0);
    assert(word(&m, 0x1a0) == 3 && byte(&m, 0x194) == 0);
    assert(hdmi_set_mode(&m, NULL, "99suffix") == 0);
    assert(word(&m, 0x1a0) == 99 && word(&m, 0x160) == 4);
    assert(byte(&m, 0x155) == 1 && byte(&m, 0x156) == 1);
    assert(word(&m, 0x1e0) == 1);
}
static void test_drop_invalidates_only_on_change(void)
{
    int (*setter)(dsp0_hdmi *, const void *, const char *) = hdmi_set_data_drop_mode;
    assert(setter && "drop setter not reconstructed");
    dsp0_hdmi m = {0};
    m.data_drop_mode = 2;
    set_byte(&m, 0x155, 1); set_byte(&m, 0x156, 1);
    set_word(&m, 0x1b4, 4096);
    assert(setter(&m, NULL, "2") == 0);
    assert(byte(&m, 0x155) == 1 && byte(&m, 0x156) == 1);
    assert(byte(&m, 0x1dc) == 0);
    assert(setter(&m, NULL, "1") == 0);
    assert(m.data_drop_mode == 1 && byte(&m, 0x1dc) == 1);
    assert(byte(&m, 0x155) == 0 && byte(&m, 0x156) == 0);
    assert(word(&m, 0x1b4) == 4096); /* Deferred until output callback. */
    set_byte(&m, 0x155, 1); set_byte(&m, 0x156, 1);
    set_byte(&m, 0x1dc, 0);
    assert(setter(&m, NULL, "1") == 0);
    assert(byte(&m, 0x155) == 1 && byte(&m, 0x156) == 1);
    assert(byte(&m, 0x1dc) == 0);
    assert(setter(&m, NULL, "0xffffffff") == 0); /* No enum bounds check. */
    assert(m.data_drop_mode == -1 && byte(&m, 0x1dc) == 1);
    assert(byte(&m, 0x155) == 0 && byte(&m, 0x156) == 0);
}
static void test_bc_initialization(void)
{
    int (*initbc)(dsp0_hdmi *) = _hdmi_InitBC;
    assert(initbc && "BC initialization not reconstructed");
    dsp0_hdmi m;
    fixture(&m, 0, 0);
    set_word(&m, 0x100, 31); set_word(&m, 0x104, 32);
    set_word(&m, 0x108, 33);
    bc[1] = bc[0];
    assert(initbc(&m) == -1);
    assert(word(&m, 0xf8) == bc[0] && word(&m, 0xf4) == bc[0] + 0x100000);
    assert(word(&m, 0xfc) == 0 && bc[3] == bc[0] + 0x20);
    assert(word(&m, 0x100) == 31 && word(&m, 0x104) == 32 && word(&m, 0x108) == 33);
    bc[1] = bc[0] + 0x10000;
    assert(initbc(&m) == 0);
    assert(bc[3] == bc[2] && word(&m, 0xf8) == bc[0]);
    assert(word(&m, 0xfc) == 0x10000 && word(&m, 0x100) == 0x200);
    assert(word(&m, 0x104) == 0x200 && word(&m, 0x108) == 0x200);
    bc[2] = bc[1];
    assert(initbc(&m) == 0);
    assert(word(&m, 0x100) == 0 && word(&m, 0x108) == 0);
    assert(word(&m, 0x104) == 0x10000 && bc[3] == bc[1]);
}
static const size_t reset_words[] = {
    0x1d0, 0x1cc, 0x1c8, 0x1c4, 0x1c0, 0x1bc, 0x1b8,
    0x19c, 0x198, 0x1a8, 0x1a4, 0x1b0, 0x174, 0x188,
    0x184, 0x180, 0x18c, 0x204, 0x1f8, 0x1f4
};
static void test_initial_sizing_and_port_selection(void)
{
    int (*start)(dsp0_hdmi *, int) = _hdmi_start;
    assert(start && "start helper not reconstructed");
    static const struct {
        uint32_t mode, channels, rate;
        int codec, drop;
        uint8_t hbr;
        uint32_t size;
    } cases[] = {
        {0, 2, 48000, 0, 0, 0, 1024},
        {0, 2, 48001, 0, 0, 0, 4096},
        {0, 8, 48000, 0, 0, 0, 4096},
        {0, 8, 96000, 0, 2, 0, 1024},
        {4, 8, 48000, 0, 0, 1, 1024},
        {4, 2, 48000, 0, 2, 1, 4096},
        {4, 2, 48000, 0, 0, 0, 1024},
        {7, 2, 48000, 4, 2, 1, 4096},
        {8, 2, 48000, 4, 2, 0, 1024},
        {4, 8, 48000, 2, 2, 0, 4096},
        {4, 8, 48000, 4, 2, 1, 1024},
        {4, 8, 48000, 6, 2, 1, 1024},
        {0, 2, 48000, 2, 2, 0, 4096},
        {1, 8, 96000, 0, 0, 0, 1024},
        {99, 8, 96000, 0, 0, 0, 1024}
    };
    static const uint32_t selectors[] = {0, 1, 6, 7};
    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); ++i) {
        dsp0_hdmi m;
        unsigned channel = i % 2, port = i % 4;
        fixture(&m, channel, port);
        m.mode = cases[i].mode; m.channel_num = cases[i].channels;
        m.sampling_freq = cases[i].rate; m.codec = cases[i].codec;
        m.data_drop_mode = cases[i].drop; m.hbr = cases[i].hbr;
        set_word(&m, 0x1e0, 0xa5a5); /* Helper does not own the start property. */
        for (size_t j = 0; j < sizeof(reset_words) / sizeof(reset_words[0]); ++j)
            set_word(&m, reset_words[j], 0xfeedbeef);
        set_byte(&m, 0x155, 1); set_byte(&m, 0x156, 1);
        set_byte(&m, 0x1f0, 0xa5); set_byte(&m, 0x208, 0xa5);
        set_byte(&m, 0x1e4, 0); set_byte(&m, 0x1dc, 0x5a);
        set_word(&m, 0x16c, 0xcafe); set_word(&m, 0x178, 0xcafe);
        set_word(&m, 0x1ac, 0xcafe); set_word(&m, 0x1e8, 0xcafe);
        set_word(&m, 0x1fc, 0xcafe);
        registers[0x140 / 4] = 0xdeadbeef;
        uint32_t expected_mux = channel ?
            ((0xdeadbeef & ~0x70U) | (selectors[port] << 4)) :
            ((0xdeadbeef & ~7U) | selectors[port]);
        assert(start(&m, 7) == 0); /* Any nonzero enable value takes start. */
        assert(word(&m, 0x1b4) == cases[i].size);
        for (size_t j = 0; j < sizeof(reset_words) / sizeof(reset_words[0]); ++j)
            assert(word(&m, reset_words[j]) == 0);
        assert(byte(&m, 0x155) == 0 && byte(&m, 0x156) == 0);
        assert(byte(&m, 0x1f0) == 0 && byte(&m, 0x208) == 0);
        assert(byte(&m, 0x1e4) == 1 && byte(&m, 0x1dc) == 0x5a);
        assert(word(&m, 0x190) == 1 && word(&m, 0x1e0) == 0xa5a5);
        assert(word(&m, 0x16c) == 0xcafe && word(&m, 0x178) == 0xcafe);
        assert(word(&m, 0x1ac) == 0xcafe && word(&m, 0x1e8) == 0xcafe);
        assert(word(&m, 0x1fc) == 0xcafe);
        assert(registers[0x140 / 4] == expected_mux);
        assert(registers[(0x300 + port * 8) / 4] == 1);
        assert(registers[(0x304 + port * 8) / 4] == 4);
        set_byte(&m, 0x155, 1); set_byte(&m, 0x156, 1);
        dsp0_hdmi snapshot = m;
        assert(start(&m, 0) == 0);
        assert(memcmp(&snapshot, &m, sizeof(m)) == 0); /* Stop does not reset FIRST. */
        assert(registers[0x140 / 4] == expected_mux && bc_control == 0);
        assert(registers[(0x300 + port * 8) / 4] == 1 && bc_reset == 3);
    }
}
static void test_start_stop_and_failed_start(void)
{
    int (*setter)(dsp0_hdmi *, const void *, const char *) = hdmi_set_start;
    int (*start)(dsp0_hdmi *, int) = _hdmi_start;
    int (*initbc)(dsp0_hdmi *) = _hdmi_InitBC;
    assert(setter && start && initbc && "start/stop not reconstructed");
    dsp0_hdmi m;
    fixture(&m, 1, 2);
    assert(setter(&m, NULL, "0") == -1);
    assert(parser_resets == 0 && irq_registrations == 0);
    set_word(&m, 0, 123);
    set_byte(&m, 0x155, 1); set_byte(&m, 0x156, 1);
    set_word(&m, 0x174, 41); set_word(&m, 0x198, 42);
    set_word(&m, 0x1c0, 43); set_word(&m, 0x204, 44);
    m.irq_count = 99;
    registers[(0x200 / 4) + 1] = 0x80;
    registers[0x140 / 4] = 0xffffffff;
    assert(setter(&m, NULL, "3") == 0);
    assert(word(&m, 0x1e0) == 3); /* Stored raw, helper receives boolean. */
    assert(parser_resets == 1 && irq_enables == 1 && irq_registrations == 1);
    assert(last_owner == 123 && last_handler == hdmi_handle_irq && last_handler_arg == &m);
    assert(last_irq == 9 && bc_control == 1 && bc_reset == 3);
    assert(registers[(0x200 / 4) + 1] == 0x88);
    assert(registers[0x140 / 4] == ((0xffffffffU & ~0x70U) | 0x60));
    assert(registers[0x314 / 4] == 4 && registers[0x310 / 4] == 1);
    assert(bc[3] == bc[2]); /* Old unread BC data is discarded, not emitted. */
    assert(word(&m, 0xf4) == bc[0] + 0x100000 && word(&m, 0xfc) == 0x8000);
    assert(word(&m, 0x100) == 0x200 && word(&m, 0x104) == 0x200);
    assert(byte(&m, 0x155) == 0 && byte(&m, 0x156) == 0);
    /* Start recomputes +0x1b4 itself; only the drop setter marks +0x1dc. */
    assert(byte(&m, 0x1dc) == 0 && byte(&m, 0x1e4) == 1);
    assert(word(&m, 0x174) == 0 && word(&m, 0x198) == 0);
    assert(word(&m, 0x1c0) == 0 && word(&m, 0x204) == 0 && m.irq_count == 0);
    assert(word(&m, 0x190) == 1 && word(&m, 0x1b4) == 1024);
    set_byte(&m, 0x155, 1); set_byte(&m, 0x156, 1);
    assert(setter(&m, NULL, "1") == -1);
    assert(word(&m, 0x1e0) == 3 && parser_resets == 1);
    assert(byte(&m, 0x155) == 1 && byte(&m, 0x156) == 1);
    assert(setter(&m, NULL, "0") == 0);
    assert(irq_disables == 1 && irq_registrations == 2 && last_owner == 0);
    assert(last_handler == NULL && last_handler_arg == NULL);
    assert(bc_control == 0 && registers[(0x200 / 4) + 1] == 0x80);
    assert(bc_reset == 3 && registers[0x310 / 4] == 1); /* Not undone on stop. */
    assert(byte(&m, 0x155) == 1 && byte(&m, 0x156) == 1);
    assert(setter(&m, NULL, "0") == -1 && irq_disables == 1);

    fixture(&m, 0, 0);
    bc[1] = bc[0]; /* _hdmi_InitBC fails, after start's software resets. */
    set_byte(&m, 0x155, 1); set_byte(&m, 0x156, 1);
    assert(start(&m, 1) == -1);
    assert(word(&m, 0x1e0) == 0);
    assert(byte(&m, 0x155) == 0 && byte(&m, 0x156) == 0);
    assert(parser_resets == 1 && irq_enables == 0 && irq_registrations == 0);
    assert(bc_control == 17 && bc_reset == 18); /* No rollback / no HW enable. */
    assert(setter(&m, NULL, "2") == 0); /* Deliberately swallows helper -1. */
    assert(word(&m, 0x1e0) == 2 && irq_enables == 0);
    assert(setter(&m, NULL, "1") == -1); /* Retry requires stop first. */
    assert(setter(&m, NULL, "0") == 0 && bc_control == 0);
    bc[1] = bc[0] + 0x10000;
    assert(setter(&m, NULL, "1") == 0 && irq_enables == 1);
    assert(registers[0x140 / 4] == 0 && registers[0x304 / 4] == 4);
    assert(setter(&m, NULL, "0") == 0);

    fixture(&m, 0, 0);
    bc[1] = bc[0] + 0x10001;
    assert(initbc(&m) == -1); /* Bounds are unsigned nonzero <=64KiB. */
    bc[1] = bc[0] - 1;
    assert(initbc(&m) == -1);
    bc[1] = bc[0] + 0x1000; bc[2] = bc[1] + 4;
    assert(initbc(&m) == 0);
    assert(word(&m, 0x100) == 0 && word(&m, 0x108) == 0);
    assert(word(&m, 0x104) == 0x1004); /* Shipped stale-read-offset quirk. */
}
int main(void)
{
    test_mode_sequencing();
    test_drop_invalidates_only_on_change();
    test_bc_initialization();
    test_initial_sizing_and_port_selection();
    test_start_stop_and_failed_start();
    puts("DSP0 HDMI lifecycle: mode sequencing/stale fields and no implicit resync");
    return 0;
}
