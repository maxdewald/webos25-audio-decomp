/*
 * RECONSTRUCTION — hand-cleaned from the dsp2 firmware disassembly
 * (audio_a0_dsp2.main_60000000.bin, base 0x60000000), NOT original source.
 * Every branch is annotated with the instruction address(es) that support it.
 *   [OBS]  = read directly from the disassembly (*.asm in this folder)
 *   [INF]  = inferred (usually across an opaque 8-byte FLIX bundle)
 *   [SRC]  = matches paluma-5 GPL module.c / module_connector.c (2024 snapshot)
 *
 * Struct offsets recovered from the firmware (all [OBS] via l32i/s32i offsets):
 *   aud_port:   +0x00 number, +0x04 module, +0x08 flag (bit0 OUTPUT, bit1 DISABLED,
 *               bit2 HAVE_BEEP), +0x0c stream_lock, +0x28 async_down_lock,
 *               +0x44 async_up_lock, +0x60 module_node, +0x90 down_peer_node,
 *               +0xc0 down_peer_root, +0xec up_peer, +0xf0 pending_objs,
 *               +0xf4 pending_obj_alloced, +0xf8 pending_obj_num, +0x12c throwing,
 *               +0x140 current_first (word 0x50), +0x144 passed, +0x148 last_clock,
 *               +0x150 flag_work.  sizeof = 0x158 (memset at 0x60073be8 / 0x60073d12).
 *   aud_module: +0x00 name, +0x0c impl, +0x10 input_port_root, +0x3c output_port_root,
 *               +0x90 thread_priority, +0x96 block_module, +0x97 do_chain_profile,
 *               +0xa8 first_event_out (word 0x2a).
 *   impl:       +0x04 name, +0x10 chain(), +0x14 event(), +0x24 deleting_port().
 *   aud_object: +0x00 refcount, +0x04 class tag ('evnt'=0x65766e74 @lit 0x6005cdf0,
 *               'data'=0x64617461 @lit 0x6005cfb4).
 *   aud_event:  +0x0c type (1 FIRST_AU, 2 LAST_AU), +0x10 flags (bit0 DOWNWARD, bit1 SYNC).
 *
 * Helper identities: 0x60069988 aud_object_get (refcount++, returns arg in a2 — window
 * ABI, callee never writes a2), 0x600699a4 aud_object_put, 0x600a6a40/0x600a6a50
 * aud_mutex_lock/unlock, 0x6006c5e0 aud_link_add_tail, 0x6006c718 aud_link_foreach_node,
 * 0x60075024 aud_module_event, 0x6007adfc logmm_printl.
 * Symbol-table mislabels noticed: 0x600725cc "foreach_push_event" is really source's
 * foreach_down_peer_push_event (its log uses that name, lit 0x6005d9c8); the
 * callx8 target 0x600f2c50 labelled "aud_port_push_event" is an XOS yield
 * (= source aud_thread_yield()).
 */

/* ------------------------------------------------------------------ */
/* aud_event_new_first @0x60069e20  [OBS, SRC match]                   */
/* ------------------------------------------------------------------ */
struct aud_event *aud_event_new_first(void)
{
    struct aud_event *e = aud_event_new(0, 0);         /* 0x60069e23..27: a10=0,a11=0 */
    if (!e) { error("no ..."); return NULL; }          /* 0x60069e2c, 0x60069e60 */
    e->type   = 1;                                      /* 0x60069e3e s32i a11(=1),+0xc */
    e->flags |= 1 | 2;                                  /* 0x60069e42..48 DOWNWARD|SYNC */
    e->caller = __builtin_return_address(0);            /* 0x60069e4a..5c +0x1c */
    return e;                                           /* NO key-values: a "dummy" */
}

/* ------------------------------------------------------------------ */
/* aud_connector_connect @0x6006f600  [OBS, SRC match]                 */
/* ------------------------------------------------------------------ */
int aud_connector_connect(struct aud_module *inst_out, int port_out,
                          struct aud_module *inst_in, int port_in)
{
    if (aud_module_get_input_port(inst_in, port_in)) {          /* 0x6006f609, 0x6006f615 */
        error("already used input module port (%s.%d)\n");      /* 0x6006f639 */
        return -0x18;                                           /* 0x6006f650 */
    }
    struct aud_port *out = aud_module_new_output_port(inst_out, port_out); /* 0x6006f627 */
    if (!out) { error("no mem\n"); return -0x14; }              /* 0x6006f62d, 0x6006f67d */
    struct aud_port *in = aud_module_new_input_port(inst_in, port_in);     /* 0x6006f65a */
    if (!in)  { error("no mem\n"); return -0x14; }              /* 0x6006f660, 0x6006f669 */
    aud_module_connect(out, in);                                /* 0x6006f697 */
    info("module connected (%s.%d -> %s.%d)\n");                /* 0x6006f6a5, str 0x60041b90 */
    return 0;
    /* No access to out->current_first (+0x140) anywhere in 0x6006f600..0x6006f6c7. */
}

/* aud_module_new_output_port @0x60073df4 -> aud_module_new_output @0x60073c98 [OBS, SRC match]
 * An EXISTING output port with the same number is returned unchanged
 * (loop 0x60073cb2..0x60073cc1, beq 0x60073cb9 -> 0x60073cea; log at 0x60073d9b
 * "module(%s) have already output port %d\n" is reached via FLIX 0x60073cf5 [INF]).
 * => a re-route keeps the upstream output port and therefore its current_first.
 * Only a NEW port is alloc'd (0x60073cca) and zeroed 0x158 bytes (0x60073d12).
 * aud_module_new_input_port @0x60073dd8 -> aud_module_new_input @0x60073b70 likewise
 * allocs + zeroes 0x158 bytes (0x60073ba0, 0x60073be8), so a new input port starts
 * with current_first(+0x140) == NULL.
 */

/* ------------------------------------------------------------------ */
/* aud_module_connect @0x60074154  [OBS, SRC match]                    */
/* ------------------------------------------------------------------ */
int aud_module_connect(struct aud_port *out_port, struct aud_port *in_port)
{
    info("module connect %s.%d -> %s.%d\n");          /* bbsi 0x60074160 -> 0x60074186, str 0x60043894 */
    aud_mutex_lock(&in_port->async_up_lock);           /* 0x60074163..68  (+0x44) */
    in_port->up_peer = out_port;                       /* 0x6007416d s32i a2,a3,0xec */
    aud_mutex_unlock(&in_port->async_up_lock);         /* 0x60074170 */
    aud_link_add_tail(&out_port->down_peer_root,       /* 0x60074173..7f (+0xc0) */
                      &in_port->down_peer_node);       /*                (+0x90) */
    return 0;                                          /* 0x60074182 */
    /* No FLIX in this function. No event is created, pushed or replayed here. */
}

/* ------------------------------------------------------------------ */
/* add_pending_obj_locked @0x60072078  (OUTPUT side)  [OBS/INF, SRC match] */
/* ------------------------------------------------------------------ */
static int add_pending_obj_locked(struct aud_port *port, struct aud_object *obj)
{
    struct aud_module *module = port->module;                       /* 0x60072084 */
    trace("pending object %08x to %s:out%d\n");                     /* 0x600720e6, str 0x60043230 */

    if (obj->class == 'evnt') {                                     /* 0x6007208d..94 */
        struct aud_event *e = (void *)obj;
        if (e->type == 1) {                                         /* 0x60072109 -> 0x60072174 */
            if (port->current_first) aud_object_put(port->current_first);  /* 0x60072176 */
            port->current_first = aud_object_get(e);                /* 0x6007217b..7e */
        }
        if (e->type == 2) {                                         /* 0x6007210c */
            if (port->current_first) {                              /* 0x6007210f */
                aud_object_put(port->current_first);                /* 0x60072115 */
                port->current_first = NULL;                         /* 0x60072118..1a */
                /* falls into the data-check at 0x6007211d; FLIX 0x60072122 [INF]
                 * holds the "obj->class == 'data'" test (a11=obj->class,a12='data'
                 * are loaded just before it) and exits to 0x6007209d for events. */
            } else
                warn("last without first.. at output%d of %s\n");  /* 0x6007218c..aa, str 0x60043254 */
        }
    }
    if (!port->current_first && obj->class == 'data') {           /* 0x6007209a / 0x6007211d..22 [FLIX INF] */
        if (module->first_event_out) {                              /* 0x6007212a l32i a4,0xa8; beqz 0x6007212f */
            info("data without first at output%d of %s. using previous first event\n");
                                                                    /* 0x600721c2..d6, str 0x6004327c;
                                                                       entered via FLIX 0x60072132 [INF] */
            port->current_first = aud_object_get(module->first_event_out); /* 0x6007213a..3d */
        } else {
            info("data without first at output%d of %s. make dummy first event\n");
                                                                    /* bbsi 0x600721b3 -> 0x600721df, str 0x600432c0 */
            port->current_first = aud_event_new_first();            /* 0x600721b6..b9 */
        }
        /* NOTE: only port->current_first is set. The synthesized/previous event is
         * NOT queued into pending_objs and NOT sent to peers here. */
    }
    /* grow + append obj to pending_objs: 0x6007209d..0x600720d8 (FLIX 0x600720b0
     * sits in the realloc path, [INF] log-level test for "increase memory %d") */
    return 0;
}

/* ------------------------------------------------------------------ */
/* aud_port_push_event @0x600727a4  [OBS, SRC match]                   */
/* ------------------------------------------------------------------ */
int aud_port_push_event(struct aud_port *port, struct aud_event *event)
{
    if (!port)  { debug("no port.\n"); if (event) aud_object_put(event); return -1; } /* 0x600727b0..c4 */
    if (!event) { debug("no event\n"); return -1; }                                   /* 0x600727c6..dd */
    if (event->flags & 2 /*SYNC*/)                                                    /* 0x600727e2 */
        return add_pending_obj(port, event);                                          /* 0x60072853..57 */
    if (port->flag & 1 /*OUTPUT*/) {                                                  /* 0x600727e7 */
        aud_mutex_lock(&port->async_down_lock);                                       /* 0x600727ea */
        aud_link_foreach_node(&port->down_peer_root, event,
                              foreach_down_peer_push_event /*0x600725cc*/);           /* 0x600727f2..fd */
        aud_mutex_unlock(&port->async_down_lock);
    } else {
        aud_mutex_lock(&port->async_up_lock);                                         /* 0x60072808 */
        if (port->up_peer)                                                            /* 0x60072810..13 */
            aud_module_event(port->up_peer->module, port->up_peer,
                             aud_object_get(event));                                  /* 0x6007283d..4a */
        aud_mutex_unlock(&port->async_up_lock);
    }
    aud_object_put(event);                                                            /* 0x6007281d */
    return 0;
    /* No FLIX. No current_first access. No replay logic. */
}

/* ------------------------------------------------------------------ */
/* foreach_down_peer_push_data @0x60072694  [OBS, SRC match]           */
/* ------------------------------------------------------------------ */
static int foreach_down_peer_push_data(struct aud_data *data, struct aud_port *peer)
{
    struct aud_port *port = peer->up_peer;                     /* 0x60072697 l32i a3,0xec */
    aud_mutex_lock(&peer->stream_lock);                        /* 0x6007269a..a0 */
    aud_link_node_unlock(&peer->down_peer_node);               /* 0x600726a3..a9 (+0x98) */
    debug("%s.%s%d : write to %d bytes to %s.%s%d\n");         /* 0x600726b5.., str 0x600433f4 */
    if (do_check_beep) check_beep(data, port, peer);           /* 0x60072704..0x6007277d (inlined) */
    aud_module_chain_locked(peer, aud_object_get(data));       /* 0x6007277f..88  <-- replay lives here */
    bool disabled = peer->flag & 2;                            /* 0x6007278b..8f */
    aud_mutex_unlock(&peer->stream_lock);                      /* 0x60072792 */
    if (disabled) aud_thread_yield();                          /* 0x60072795..9a (callx8 0x600f2c50) */
    return 0;
    /* No FLIX. No current_first access in this function itself. */
}

/* ------------------------------------------------------------------ */
/* aud_module_chain_locked @0x600746d4  (INPUT side, the real replay) */
/* ------------------------------------------------------------------ */
static int aud_module_chain_locked(struct aud_port *port, struct aud_data *data)
{
    struct aud_module *module = port->module;                  /* 0x600746d7 */
    /* block_module wait: 0x600746dc..0x60074716 */
    if (port->flag & 2 /*DISABLED*/) {                         /* bbci 0x60074722 */
        debug("port disconnecting.\n"); aud_object_put(data); return -1;  /* 0x60074725..2f */
    }
    if (port->throwing) {                                      /* 0x60074734..39 (+0x12c) */
        debug("data index %6d, Throwing...%s\n");              /* via FLIX 0x6007473c [INF] -> 0x60074888 */
        aud_object_put(data); return -1;                       /* 0x60074744..4d */
    }
    if (module->thread_priority) adjust_thread_priority(module); /* 0x600747bb..c2 */

    if (!port->current_first) {                                /* 0x600747c5 l32i a2,0x140; beqz 0x600747ca */
        struct aud_event *e = NULL;
        info("chain without first at input%d of %s. get first event from up_peer..\n");
                                                               /* 0x60074938..53, str 0x60043950;
                                                                  entered via FLIX 0x600748aa [INF] */
        if (port->up_peer && port->up_peer->current_first)     /* 0x600748b2 l32i a2,0xec; beqz 0x600748b7;
                                                                  0x600748b9 l32i a9,0x140; beqz 0x600748bc */
            e = aud_object_get(port->up_peer->current_first);  /* 0x600748bf; bnez 0x600748c2 */
        if (!e) {
            warn("no first at up_peer. make new\n");           /* 0x60074956..63, str 0x60043998;
                                                                  entered via FLIX 0x600748c9 [INF] */
            e = aud_event_new_first();                         /* 0x600748d1 */
            if (!e) { error("NULL e.\n"); return -1; }         /* 0x600748d4, 0x600748e2..f7, str 0x600439b8 */
        }
        port->current_first = e;                               /* 0x6007490d s32i a10,a2,0x140 */
        module->impl->event(module, port, aud_object_get(e));  /* 0x60074910..20: impl(+0xc)->event(+0x14) */
    }
    /* 0x600747cd..: last_clock, passed, flag_work|=CHAIN, impl->chain() ...
     * Opaque/mis-decoded bytes at 0x60074818 / FLIX 0x60074819 / 0x60074849 / 0x6007485c
     * sit in this post-replay section (profiling + chain call); they do not affect
     * the replay decision above. */
    return module->impl->chain(port, data);                    /* 0x600748fd..05 / 0x6007483b */
}

/* ------------------------------------------------------------------ */
/* aud_port_sync_locked @0x60072c08 (excerpt) [OBS, SRC match]         */
/* ------------------------------------------------------------------ */
/* data objs -> aud_link_foreach_node(&port->down_peer_root, data,
 *                 foreach_down_peer_push_data)            0x60072d32..45
 * event objs -> foreach_down_peer_push_event (0x600725cc)  0x60072d50..58
 * all pending objs put, pending_obj_num = 0                0x60072cce..ce5
 * => a FIRST event that was already synced is gone from the queue; only the
 *    reference in port->current_first (+0x140) survives for late peers.
 * legacy path reads current_first "acmod"                  0x60073002..12
 */

/* aud_module_disconnect @0x60074048 (excerpt) [OBS, SRC match]
 * Clears in_port->up_peer (0x60074048 range, s32i +0xec) and deletes the INPUT port;
 * the OUTPUT port (and its current_first) is NOT deleted here.
 * impl->deleting_port only called when out_port word +0xe8 == 1 (0x60074061..96);
 * source says peer_count, but the value may be produced by FLIX 0x60074059 [INF].
 * deleting_port is module-specific and was not analysed.
 */
