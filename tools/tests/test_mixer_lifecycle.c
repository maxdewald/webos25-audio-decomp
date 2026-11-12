/* audiod mixDigitalSoundOutput holder lifetime: holders change before UMI
 * readiness is known, and only an explicit mix:false from the same caller ID
 * removes one (no Luna disconnect cleanup exists for this method). */
#include <assert.h>
#include <stdbool.h>
#include <string.h>

// @match src/audiod/mixer.c (?m)^#define MAX_HOLDERS .*
// @match src/audiod/mixer.c (?m)^static char  ?mix_holders\[[^;]*;
// @match src/audiod/mixer.c (?m)^static int  ?mix_holder_count;
// @match src/audiod/mixer.c (?m)^extern bool (?:umi_ready|send_set_mixer)\([^;]*;
// @declare src/audiod/mixer.c mixDigitalSoundOutput
// @extract src/audiod/mixer.c find_holder forward_set_mixer arbitrate mixDigitalSoundOutput

static bool ready = true;
static int sends;
static const char *last_sent;

bool umi_ready(void) { return ready; }
bool send_set_mixer(const char *request_json)
{
    sends++;
    last_sent = request_json;
    return true;
}

static void reset(void)
{
    mix_holder_count = 0;
    ready = true;
    sends = 0;
    last_sent = NULL;
}

static bool request(bool mix, const char *caller)
{
    return mixDigitalSoundOutput(mix, caller, NULL, "com.test.service", mix ? "on" : "off");
}

/* @0x1a5480 pushes before @0x1ec608 reaches AudioMixer::setSettings' ready test:
 * the client sees an error reply, yet its holder stays and pins mixing. */
static void test_unready_acquire_still_records_holder(void)
{
    reset();
    ready = false;
    assert(!request(true, "A"));
    assert(sends == 0);
    ready = true;
    assert(request(false, "B"));
    assert(sends == 0);
    assert(request(false, "A"));
    assert(sends == 1 && strcmp(last_sent, "off") == 0);
}

/* The last holder is erased even when forwarding mix:false then fails. */
static void test_unready_release_still_erases_holder(void)
{
    reset();
    assert(request(true, "A"));
    ready = false;
    assert(!request(false, "A"));
    ready = true;
    sends = 0;
    assert(request(false, "C"));
    assert(sends == 1 && strcmp(last_sent, "off") == 0);
}

/* A departed client's holder persists; cleanup by any other ID neither forwards
 * nor creates a holder. Only the same ID's mix:false releases it. */
static void test_foreign_release_never_acquires(void)
{
    reset();
    assert(request(true, "A"));
    assert(request(true, "A"));
    assert(mix_holder_count == 1);
    sends = 0;
    assert(request(false, "cleanup"));
    assert(request(false, "cleanup"));
    assert(sends == 0 && mix_holder_count == 1);
    assert(request(false, "A"));
    assert(sends == 1 && mix_holder_count == 0);
}

/* Empty callerId falls back to the application ID, then the sender service.
 * A different identity for the release leaves the original holder stranded. */
static void test_identity_fallback(void)
{
    reset();
    assert(mixDigitalSoundOutput(true, "", "com.app", "com.service", "on"));
    sends = 0;
    assert(mixDigitalSoundOutput(false, "", NULL, "com.app", "off"));
    assert(sends == 1 && mix_holder_count == 0);

    reset();
    assert(mixDigitalSoundOutput(true, NULL, "com.app", "com.service", "on"));
    sends = 0;
    assert(mixDigitalSoundOutput(false, "", NULL, "com.service", "off"));
    assert(sends == 0 && mix_holder_count == 1);
}

int main(void)
{
    test_unready_acquire_still_records_holder();
    test_unready_release_still_erases_holder();
    test_foreign_release_never_acquires();
    test_identity_fallback();
    return 0;
}
