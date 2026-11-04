/*
 * audiod: luna://com.webos.service.audio/tv/mixDigitalSoundOutput
 * RECONSTRUCTION from usr/sbin/audiod
 *   sha256 170d468085255d381245693260840771b00d39c718473ec024566fb1df1ca3af
 * Addresses: Ghidra (= raw ELF VA + 0x10000). Evidence: ../../evidence/audiod-mixer/
 *
 * mix is NOT derived from codec, output mode, source, game, BT or system sounds
 * in audiod. It is a caller-supplied boolean, arbitrated per caller ID.
 */
#include <stdbool.h>
#include <string.h>

#define MAX_HOLDERS 32

/* TVManager +0x2c4/+0x2c8: vector of caller IDs currently requesting mixing. */
static char mix_holders[MAX_HOLDERS][64];
static int  mix_holder_count;

/* UMI forwarding object (+4) and its ready byte (+0x34) must be valid. */
extern bool umi_ready(void);

/* @0x1ec608: builds {"category":"setMixer","value":<original request>} and sends
 * luna://com.webos.service.audiooutput/quality/setSettings via LSCallOneReply
 * (@0xd18c0). Does not wait for the downstream effect. */
extern bool send_set_mixer(const char *request_json);

static int find_holder(const char *id)
{
    for (int i = 0; i < mix_holder_count; i++)
        if (strcmp(mix_holders[i], id) == 0)
            return i;
    return -1;
}

/* @0x1a5480: caller arbitration. */
static bool arbitrate(bool mix, const char *caller_id, const char *request_json)
{
    int idx = find_holder(caller_id);

    if (!umi_ready())
        return false;

    if (mix) {
        if (idx < 0 && mix_holder_count < MAX_HOLDERS)
            strncpy(mix_holders[mix_holder_count++], caller_id, 63);
        return send_set_mixer(request_json);
    }

    if (idx >= 0) {
        memmove(mix_holders[idx], mix_holders[idx + 1],
                (size_t)(mix_holder_count - idx - 1) * sizeof(mix_holders[0]));
        mix_holder_count--;
    }
    /* @0x1a58d0: other holders remain -> swallow the false, reply success. */
    if (mix_holder_count > 0)
        return true;
    return send_set_mixer(request_json);          /* @0x1a5910 */
}

/* @0x1d186c: registered callback; schema allows only mix:boolean, callerId:string. */
bool mixDigitalSoundOutput(bool mix, const char *caller_id,
                           const char *app_id, const char *service_name,
                           const char *request_json)
{
    if (!caller_id || !*caller_id)
        caller_id = app_id ? app_id : service_name;   /* LSMessageGetApplicationID, then sender */
    return arbitrate(mix, caller_id, request_json);
}
