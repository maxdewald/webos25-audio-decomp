// SPDX-License-Identifier: GPL-2.0-only
/*
 * Source-assisted reconstruction of Image-5.4.268-329.decompressed, kernel
 * 5.4.268-329.ptl4tv.5, SHA256
 * 0606410b32b19fd10f77bb3bdd9b5c618380c717076638418169b7e7bbff80db.
 * Offline instruction evidence: build/scratch/seek-chain/image-evidence.json.
 * Address-tagged bodies were checked against the shipped AArch64 instructions.
 * Kernel allocation, locking, logging and workqueue primitives remain external.
 */
/*
 * SIC LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 * Copyright(c) 2020 by LG Electronics Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include "log.h"
#include "renderer.h"
#include "alsa/alsa_support_sndout.h"

logm_define (notifier, log_level_noti);
#define debug(fmt,args...)  logm_debug(notifier,fmt,##args)
#define noti(fmt,args...)   logm_noti(notifier,fmt,##args)

#define PLAY_SPEED_INDEX_MAX    2
unsigned int play_speed[PLAY_SPEED_INDEX_MAX] = { 1000, 1000 };

/* Image direct callers: sndout_connect+0x140, optic_output_type+0x124,
 * arc_output_type+0x130, wowcast_output_type_put+0xe4, earc_output_type+0x120,
 * earc_onoff+0xe8, arc_onoff+0xf8, spdif_output_type+0xd8 (all sndout_*).
 * This is NOT a reconnect/event queue in this Image: renderer_change_output
 * @0xffffffc010cad3f0 is just RET. Reconnection occurs separately inside the
 * control handlers, before notification; even unchanged valid writes notify.
 * Digest-bound aligned direct-call evidence: test_output_routing.py. Indirect
 * callers are not ruled out. */
/* @0xffffffc010cc32a0 */
void notifier_output_changed (void)
{
    renderer_change_output ();

    debug ("output changed\n");
}

/* The sole aligned direct Image caller is decoder_decoded_info_cb+0xf8
 * @0xffffffc010ca62d8. Raw callback +0x10 checks payload length 0x2c;
 * +0x54..+0x78 / +0x164..+0x17c compare converted codec, sampling rate,
 * channels, bits/sample and acmod against prior decoded info. Only a change
 * notifies, after storing the new info; unrelated fields do not reconnect.
 * Unlike output_changed, this really invokes the locked soundout reconnect
 * dispatcher; it is not an eARC-IRQ or log-text heuristic. */
/* @0xffffffc010cc3310 */
void notifier_input_changed (void)
{
    sndout_connect_output ();

    debug ("input changed\n");
}

/* A changed index 0/1 reconnects synchronously; invalid/unchanged requests only
 * log. 1000 is normal speed. sndout_connect_output -> _connect_output_all ->
 * _connect_output/_old_connect_output -> _get_output_format forces PCM and
 * bypass=false for non-1000 speed -> _set_output disconnects/reconnects ports
 * unless its cached-parameters guard returns early. Legacy set_type=NONE
 * goes through _old_arc_connect_output instead, without that speed override.
 * output_set_format is logging-only in this Image: it neither sets hardware nor
 * stores bypass. The later DSP output_status event drives _set_format instead.
 * The only aligned direct caller is aren_set_speed+0x11c
 * @0xffffffc010cbe75c (AREN_SET_SPEED 0x40045209; suppressed in VSP mode).
 * This setter does not itself send FIRST, flush, or a clock-update command. */
/* @0xffffffc010cc3380 */
void notifier_set_play_speed (int index, unsigned int speed)
{
    if (index < PLAY_SPEED_INDEX_MAX && index >= 0)
    {
        if (play_speed[index] != speed)
        {
            play_speed[index] = speed;
            sndout_connect_output ();
        }
    }

    noti ("set play speed: index %d, speed %d\n", index, speed);
}

/* @0xffffffc010cc3430 */
unsigned int notifier_get_play_speed (int index)
{
    unsigned int speed = 1000;

    if (index < PLAY_SPEED_INDEX_MAX && index >= 0)
        speed = play_speed[index];

    return speed;
}
