#!/bin/zsh
# Export instruction listings for every analyzed range from the private project copy.
R=${0:A:h:h:h}
O=$R/evidence/dsp2-connect
args=()
for spec in \
  6006f600:6006f6c7:6006f600_aud_connector_connect \
  60073b70:60073c97:60073b70_aud_module_new_input \
  60073c98:60073dd7:60073c98_aud_module_new_output \
  60073dd8:60073e0f:60073dd8_aud_module_new_input_port_output_port \
  60074154:600741af:60074154_aud_module_connect \
  60072078:600721fb:60072078_add_pending_obj_locked \
  60072694:600727a3:60072694_foreach_down_peer_push_data \
  600727a4:6007285f:600727a4_aud_port_push_event \
  600746d4:6007496b:600746d4_aud_module_chain_locked \
  60069e20:60069ecb:60069e20_aud_event_new_first \
  60043254:60043320:60043254_strings
do
  a=(${(s.:.)spec})
  args+=(-postScript ListInsns.java $a[1] $a[2] $O/$a[3].asm)
done
JAVA_HOME=/opt/homebrew/opt/openjdk@21 /opt/homebrew/opt/ghidra/libexec/support/analyzeHeadless \
  $O/proj audio_a0_dsp2.main_60000000.bin -process audio_a0_dsp2.main_60000000.bin \
  -noanalysis -readOnly -scriptPath $R/tools "${args[@]}" 2>&1 | grep -E 'LIST_EXPORTED|ERROR|Exception' | grep -v patternconstraints
