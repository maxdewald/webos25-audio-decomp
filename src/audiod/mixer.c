/*
 * audiod: luna://com.webos.service.audio/tv/mixDigitalSoundOutput
 * RECONSTRUCTION from usr/sbin/audiod
 *   sha256 170d468085255d381245693260840771b00d39c718473ec024566fb1df1ca3af
 * Addresses: Ghidra (= raw ELF VA + 0x10000). Evidence: build/scratch/evidence/audiod-mixer/,
 * build/scratch/mixer-lifecycle/findings.md
 *
 * mix is NOT derived from codec, output mode, source, game, BT or system sounds
 * in audiod. It is a caller-supplied boolean, arbitrated per caller ID.
 *
 * Proven ownership: TVManager owns the +0x2c4/+0x2c8/+0x2cc vector of copied
 * strings, not LSMessage objects. Explicit equal-ID mix:false erases a holder
 * @0x1a58a6. ~TVManager @0x1b1db2..0x1b1e68 destroys strings and storage.
 * The reply-only handler @0x1d186c does not subscribe the request.
 * [GAP] No holder-specific client-exit/disconnect callback or restart replay was
 * recovered in audiod. Holders persist until exact-ID release or TVManager destruction.
 */
#include <stdbool.h>
#include <string.h>

/* [MODEL] Original is an unbounded std::vector<std::string>; the cap and the
 * 63-byte copy are host-model limits, not shipped behavior. */
#define MAX_HOLDERS 32

/* TVManager +0x2c4/+0x2c8: vector of caller IDs currently requesting mixing. */
static char mix_holders[MAX_HOLDERS][64];
static int  mix_holder_count;

/* AudioMixer::setSettings @0xc6ec4: UMI forwarding object (+4) and its ready
 * byte (+0x34) must be valid, else it returns 0 without sending. */
extern bool umi_ready(void);

/* umiaudiomixer::setSettings @0xd15d8: builds {"category":"setMixer","value":
 * <original request>} and sends luna://com.webos.service.audiooutput/quality/
 * setSettings via LSCallOneReply (@0xd18c0). Success means the call was queued;
 * the libumi result (and any ALSA/hardware write) is never observed by audiod. */
extern bool send_set_mixer(const char *request_json);

static int find_holder(const char *id)
{
    for (int i = 0; i < mix_holder_count; i++)
        if (strcmp(mix_holders[i], id) == 0)
            return i;
    return -1;
}

/* @0x1ec608 TVManager::mixDigitalSoundOutput: JValue::hasKey("mix") (not modeled;
 * schema-validated request), then AudioMixer::setSettings. Readiness is checked
 * HERE, after the holder vector has already changed. */
static bool forward_set_mixer(const char *request_json)
{
    if (!umi_ready())
        return false;
    return send_set_mixer(request_json);
}

/* @0x1a5480: caller arbitration. The vector is mutated before forwarding, so a
 * not-ready UMI makes the client see failure while mix:true still records the
 * holder and mix:false still removes it. mix:false never adds a holder. */
static bool arbitrate(bool mix, const char *caller_id, const char *request_json)
{
    int holder_index = find_holder(caller_id);

    if (mix) {
        if (holder_index < 0 && mix_holder_count < MAX_HOLDERS)   /* push @0x1df544 */
            strncpy(mix_holders[mix_holder_count++], caller_id, 63);
        return forward_set_mixer(request_json);   /* even when already held */
    }

    if (holder_index >= 0) {                      /* _M_erase @0x1a58a6 */
        memmove(mix_holders[holder_index], mix_holders[holder_index + 1],
                (size_t)(mix_holder_count - holder_index - 1) * sizeof(mix_holders[0]));
        mix_holder_count--;
    }
    /* @0x1a58d0: other holders remain -> swallow the false, reply success. */
    if (mix_holder_count > 0)
        return true;
    return forward_set_mixer(request_json);       /* @0x1a5910 */
}

/* @0x1d186c: registered callback; schema allows only mix:boolean, callerId:string.
 * Reply-only (no subscription). Empty callerId -> LSMessageGetApplicationID,
 * else LSMessageGetSenderServiceName; requests from one client that mix these
 * identities create separate holders. */
bool mixDigitalSoundOutput(bool mix, const char *caller_id,
                           const char *app_id, const char *service_name,
                           const char *request_json)
{
    if (!caller_id || !*caller_id)
        caller_id = app_id ? app_id : service_name;   /* LSMessageGetApplicationID, then sender */
    return arbitrate(mix, caller_id, request_json);
}
