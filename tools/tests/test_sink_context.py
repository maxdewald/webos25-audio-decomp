#!/usr/bin/env python3
"""Usage: python3 tools/tests/test_sink_context.py

Audit recovered sink/context association and lifecycle/API boundaries against the
saved binary. Static contract tests, NOT a runtime hook or concurrency proof.
"""
import hashlib
from pathlib import Path
import re
import shutil
import subprocess
import unittest

ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / 'src/libgstlgaudiosink'
BINARY = ROOT / 'orig/usr/lib/gstreamer-1.0/libgstlgaudiosink.so'


def function(text, name):
    text = re.sub(r'/\*.*?\*/', '', text, flags=re.S)
    definition = re.search(r'^.*\b' + re.escape(name) + r'\s*\([^;{}]*\)\s*\n\{', text, re.M)
    if definition is None:
        raise AssertionError(f'missing definition: {name}')
    start = text.index('{', definition.start())
    depth, end = 1, start + 1
    while depth:
        depth += (text[end] == '{') - (text[end] == '}')
        end += 1
    return text[start:end]


def ordered(test, text, parts):
    cursor = 0
    for part in parts:
        position = text.find(part, cursor)
        test.assertGreaterEqual(position, cursor, part)
        cursor = position + len(part)


def tool(name):
    found = shutil.which(name)
    if found:
        return found
    candidate = Path('/opt/homebrew/opt/llvm/bin') / name
    return str(candidate) if candidate.is_file() else None


class ContextContracts(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.props = (SOURCE / 'render_props.c').read_text()
        cls.element = (SOURCE / 'render_element.c').read_text()
        cls.lgao = (SOURCE / 'lgao.c').read_text()

    def test_actual_instance_association_not_global_lookup(self):
        state = function(self.props, 'gst_lg_audio_render_change_state')
        ordered(self, state, ['GST_STATE_CHANGE_NULL_TO_READY:',
                             'pAudioRen->ao = lgao_open ();',
                             'GST_ELEMENT_CLASS (parent_class)->change_state'])
        chain = function(self.element, 'gst_lg_audio_render_chain')
        self.assertIn('lgao_feed (pAudioRen->ao,', chain)
        self.assertNotIn('ModuleRegistry_Search', chain)
        stop = function(self.element, 'gst_lg_audio_render_stop')
        ordered(self, stop, ['gst_task_join', 'lgao_disconnect_renderer',
                            'lgao_close (pAudioRen->ao)', 'pAudioRen->ao = NULL'])

    def test_lifecycle_has_no_universal_submission_lease(self):
        for text, name in [(self.element, 'gst_lg_audio_render_stop'),
                           (self.lgao, 'lgao_disconnect_renderer'),
                           (self.lgao, 'lgao_set_drop_renderer'),
                           (self.lgao, 'lgao_close')]:
            body = function(text, name)
            self.assertNotRegex(body, r'(?:pthread_mutex_lock|g_mutex_lock|GST_PAD_STREAM_LOCK)\s*\(')
        disconnect = function(self.lgao, 'lgao_disconnect_renderer')
        ordered(self, disconnect, ['aimc_module_destroy(ao->imc, ao->uid_bypass_appsrc)',
                                   'ao->uid_bypass_appsrc = 0', 'ao->bypass_appsrc = NULL'])
        drop = function(self.lgao, 'lgao_set_drop_renderer')
        ordered(self, drop, ['ao->drop_renderer = is_drop', 'lgao_disconnect_renderer(ao)',
                            '_lgao_connect_renderer(ao)', 'lgao_push_first_event(ao)'])

    def test_callbacks_and_properties_are_not_context_capabilities(self):
        class_init = function(self.props, 'gst_lg_audio_render_class_init')
        self.assertNotIn('g_param_spec_pointer', class_init)
        signals = re.findall(r'g_signal_new\s*\((.*?)\);', class_init, re.S)
        self.assertEqual(len(signals), 3)
        for signal in signals:
            self.assertRegex(signal, r'G_TYPE_NONE,\s*0\s*$')
        callback_task = function(self.element, 'gst_lg_audio_render_callback_task')
        self.assertIn('g_signal_emit_by_name (GST_ELEMENT (pAudioRen), "es-bypass-done")', callback_task)
        connect = function(self.lgao, '_lgao_connect_renderer')
        self.assertIn('module_appsink_set_callback(ao->appsink, &lgao_appsink_cb, ao)', connect)
        callback = function(self.lgao, 'lgao_appsink_handle_data')
        self.assertIn('aud_object_put(&data->object)', callback)
        code = re.sub(r'/\*.*?\*/', '', self.lgao, flags=re.S)
        self.assertEqual(len(re.findall(r'ao->pFnCallbackFn', code)), 1)
        self.assertNotIn('lgao_set_callback', code)

    def test_rebuilds_are_not_only_pad_submission(self):
        caps = function(self.element, 'gst_lg_audio_render_set_caps')
        self.assertIn('lgao_disconnect_renderer (pAudioRen->ao)', caps)
        event = function(self.element, 'gst_lg_audio_render_event')
        self.assertIn('lgao_disconnect_renderer (pAudioRen->ao)', event)
        chain = function(self.element, 'gst_lg_audio_render_chain')
        self.assertIn('lgao_set_drop_renderer (pAudioRen->ao, 0)', chain)
        self.assertIn('lgao_set_drop_renderer (pAudioRen->ao, 1)', chain)

    def test_saved_binary_identity_and_interception_surface(self):
        if not BINARY.is_file():
            self.skipTest('original sink binary not available')
        self.assertEqual(hashlib.sha256(BINARY.read_bytes()).hexdigest(),
                         '90dba3ef32704ce4096bf1e6045d2879d0c2192d507bc04d1e8e98437f420a43')
        nm, readelf = tool('llvm-nm'), tool('llvm-readelf')
        if not nm or not readelf:
            self.skipTest('LLVM ELF tools unavailable')
        exports = subprocess.check_output([nm, '-D', '--defined-only', str(BINARY)], text=True, timeout=15)
        names = {line.split()[-1] for line in exports.splitlines() if line.split()}
        self.assertIn('gst_lg_audio_render_get_type', names)
        self.assertIn('lgao_appsink_cb', names)
        self.assertFalse(any(re.search(r'(?:get_ao|get_context|get_appsrc|set_callback)', n) for n in names))
        relocs = subprocess.check_output([readelf, '-r', str(BINARY)], text=True, timeout=15)
        for name in ('lgao_feed', 'lgao_open', 'lgao_close', 'lgao_connect_renderer',
                     'lgao_disconnect_renderer', 'lgao_set_drop_renderer'):
            self.assertRegex(relocs, r'R_ARM_JUMP_SLOT[^\n]*\b' + name + r'\b')
        dynamic = subprocess.check_output([readelf, '-d', str(BINARY)], text=True, timeout=15)
        self.assertIn('(BIND_NOW)', dynamic)
        notes = subprocess.check_output([readelf, '-n', str(BINARY)], text=True, timeout=15)
        self.assertIn('a7977a3e730e6d0a6c70eb3ddd2b9ae04ee45d7f', notes)


if __name__ == '__main__':
    unittest.main()
