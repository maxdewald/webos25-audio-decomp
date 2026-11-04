/* Ghidra pseudocode, not original source. FLIX operations remain opaque;
 * __blk_ routines are synthetic exports, not proven independent functions. */

/* @600a0ed8 _lipsync_print_metadata */

undefined4 _lipsync_print_metadata(undefined4 *param_1)

{
  uint *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  
  puVar3 = PTR_DAT_600a0634;
  puVar2 = PTR_s__lipsync_print_metadata_600a0630;
  puVar1 = SUB_600a062c;
  if ((*SUB_600a062c & 4) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_print_metadata_600a0630,299,
                 PTR_s__________________________________60045b38_0x18_600a0640);
  }
  if ((*SUB_600a062c & 4) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_print_metadata_600a0630,300,
                 PTR_s______s_MetaData_600a0638,*param_1);
  }
  msf_print_metadata(param_1 + 0x45);
  if ((*puVar1 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(puVar3,2,puVar2,0x12f,PTR_s__________________________________60045b38_0x18_600a0640
                );
  }
  return 0;
}


/* @600a0f06 _lipsync_print_metadata__blk_600a0f06 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_print_metadata__blk_600a0f06
          (int param_1,uint *param_2,undefined4 param_3,undefined4 param_4,uint param_5)

{
  if ((*param_2 & param_5) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(param_3,2,param_4,0x12d,_DAT_600a063c);
  }
  msf_print_metadata(param_1 + 0x114);
  if ((*param_2 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(param_3,2,param_4,0x12f,
                 PTR_s__________________________________60045b38_0x18_600a0640);
  }
  return 0;
}


/* @600a0f43 _lipsync_print_metadata__blk_600a0f43 */

undefined4
_lipsync_print_metadata__blk_600a0f43
          (undefined4 *param_1,uint *param_2,undefined4 param_3,undefined4 param_4,uint param_5)

{
  if ((*param_2 & param_5) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(param_3,2,param_4,300,PTR_s______s_MetaData_600a0638,*param_1);
  }
  msf_print_metadata(param_1 + 0x45);
  if ((*param_2 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(param_3,2,param_4,0x12f,
                 PTR_s__________________________________60045b38_0x18_600a0640);
  }
  return 0;
}


/* @600a0f5d _lipsync_print_metadata__blk_600a0f5d */

undefined4 _lipsync_print_metadata__blk_600a0f5d(void)

{
  return 0;
}


/* @600a0f64 _lipsync_print_status */

undefined4 _lipsync_print_status(undefined4 *param_1)

{
  uint uVar1;
  
  logm_message(PTR_s__________________________________600a0644);
  logm_message(PTR_s_________________________INPUT_CO_600a0648);
  logm_message(PTR_s______s__d__Info_600a064c,*param_1,param_1[1]);
  logm_message(PTR_s___Processing_byte____d_600a0650,param_1[0x78] * param_1[0x76] * param_1[0x9d]);
  logm_message(PTR_s___samples_per_ch____d_600a0654,param_1[0x9d]);
  logm_message(PTR_s___inbyte_outbyte____u__u_600a0658,param_1[0x4e],param_1[0x4f]);
  logm_message(PTR_s___discard_count____d_600a065c,param_1[0x79]);
  logm_message(PTR_s____________Lipsync_info__________600a0660);
  logm_message(PTR_s___lipsync_on_1__off_0_____d_600a0664,param_1[0x7a]);
  logm_message(PTR_s___clock_type___0x_x_600a0668,param_1[0x80]);
  logm_message(PTR_s___data_type__0_pcm__1_es_____d_600a066c,param_1[0x75]);
  logm_message(PTR_s___lower_bound____d_600a0670,param_1[0x7c]);
  logm_message(PTR_s___upper_bound____d_600a0674,param_1[0x7b]);
  logm_message(PTR_s___additional_bound____d_600a0678,param_1[0x8d]);
  logm_message(PTR_s___freerun_lower_bound____d_600a067c,param_1[0x7f]);
  logm_message(PTR_s___freerun_upper_bound____d_600a0680,param_1[0x7e]);
  logm_message(PTR_s___const_offset____d_600a0684,param_1[0x7d]);
  logm_message(PTR_s___Fs____d_600a0688,param_1[0x77]);
  logm_message(PTR_s___Channel____d_600a068c,param_1[0x76]);
  logm_message(PTR_s___based_CT__onlyEMP_____d_600a0690,param_1[0x81]);
  logm_message(PTR_s___based_ST__onlyEMP_____d_600a0694,param_1[0x82]);
  logm_message(PTR_s___speed_mode_on_1__off_0_____d_600a0698,param_1[0x5f]);
  logm_message(PTR_s___speed____d_600a069c,param_1[0x60]);
  logm_message(PTR_s___bype_per_sample____d_600a06a0,param_1[0x78]);
  if (param_1[0x77] == 0) {
    uVar1 = 0;
  }
  else {
    uVar1 = (uint)(param_1[0x9d] * param_1[0x62]) / (uint)param_1[0x77];
  }
  logm_message(PTR_s___idle_cnt____d_loop___d_msec__600a06a4,param_1[0x62],uVar1);
  logm_message(PTR_s___overflow_protection____d_600a06a8,param_1[0x86]);
  logm_message(PTR_s____________Media_info____________600a06ac);
  logm_message(PTR_s_skip___d_600a06b0,param_1[0x68]);
  logm_message(PTR_s_repeat___d_600a06b4,param_1[0x69]);
  logm_message(PTR_s_discontinuity___d_600a06b8,param_1[0x6a]);
  logm_message(PTR_s_freerun___d_600a06bc,param_1[0x6b]);
  logm_message(PTR_s__________________________________600a0644);
  return 0;
}


/* @600a10a4 _lipsync_flush */

undefined4 _lipsync_flush(int param_1)

{
  if ((*SUB_600a062c & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_flush_600a06c0,0x181,PTR_s_do_flush____600a06c4);
  }
  aud_data_adapter_flush(param_1 + 0xf0,*(int *)(param_1 + 0x100) - *(int *)(param_1 + 0x104));
  msf_flush_metadata(param_1 + 0x114);
  *(undefined4 *)(param_1 + 0x228) = 0;
  *(undefined4 *)(param_1 + 0x224) = 0;
  *(undefined1 *)(param_1 + 0x220) = 0;
  *(undefined4 *)(param_1 + 0x110) = 0;
  return 0;
}


/* @600a10fa _lipsync_flush__blk_600a10fa */

undefined4 _lipsync_flush__blk_600a10fa(int param_1)

{
  aud_data_adapter_flush(param_1 + 0xf0,*(int *)(param_1 + 0x100) - *(int *)(param_1 + 0x104));
  msf_flush_metadata(param_1 + 0x114);
  *(undefined4 *)(param_1 + 0x228) = 0;
  *(undefined4 *)(param_1 + 0x224) = 0;
  *(undefined1 *)(param_1 + 0x220) = 0;
  *(undefined4 *)(param_1 + 0x110) = 0;
  return 0;
}


/* @600a1100 _lipsync_cmd */

undefined4 _lipsync_cmd(int param_1,undefined4 param_2,int *param_3)

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  
  puVar1 = SUB_600a062c;
  flix(0x20b0283,0xd001a41);
  aud_mutex_lock(param_1 + 0x23c);
  aud_cond_signal(param_1 + 600);
  aud_mutex_unlock(param_1 + 0x23c);
  flix(0x50b0c83,0xd000841);
  flix(0x20b0e93,0xd000e41);
  if (param_3 == (int *)0x0) {
    __assert_failed(PTR_s__lipsync_cmd_600a06c8,0x1f9,PTR_s_param____NULL_600a0774,0,DAT_600a06f0);
  }
  flix(0xb0b05ac,0xd004a41);
  if ((*puVar1 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1fb,PTR_s_clockbase____d_600a06d4,
                 *param_3);
  }
  iVar2 = *param_3;
  iVar3 = param_3[1];
  if (iVar2 == *(int *)(param_1 + 0x204)) {
    flix(0x10b08b8,0xd004a41);
  }
  *(undefined1 *)(param_1 + 0x231) = 1;
  *(int *)(param_1 + 0x204) = iVar2;
  *(int *)(param_1 + 0x20c) = iVar2;
  *(int *)(param_1 + 0x208) = iVar3;
  *(int *)(param_1 + 0x210) = iVar3;
  return 0;
}


/* @600a117d _lipsync_cmd__blk_600a117d */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a117d
          (int param_1,undefined4 param_2,int *param_3,undefined4 param_4,int param_5,uint *param_6)

{
  int iVar1;
  int iVar2;
  
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1fc,_LAB_600a06d8,param_3[1]);
  }
  iVar1 = *param_3;
  iVar2 = param_3[1];
  if (iVar1 == *(int *)(param_1 + 0x204)) {
    flix(0x10b08b8,0xd004a41);
  }
  *(undefined1 *)(param_1 + param_5 + 0xc5) = 1;
  *(int *)(param_1 + 0x204) = iVar1;
  *(int *)(param_1 + 0x20c) = iVar1;
  *(int *)(param_1 + 0x208) = iVar2;
  *(int *)(param_1 + 0x210) = iVar2;
  return 0;
}


/* @600a11c6 _lipsync_cmd__blk_600a11c6 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a11c6
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  flix(0x50b0ce3,0xd000b41);
  flix(0x10b0cf3,0xd006641);
  flix(0xa0b0c83,0xd002041);
  flix(0x70b0e93,0xd003a41);
  flix(0xf0b004a,0xd407e41);
  flix(0xf0b05ac,0xd007341);
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1cf,
                 lgsrcSinctable_makeTable__blk_600a06eb,*param_3);
  }
  *(undefined4 *)(param_1 + 0x1e8) = *param_3;
  return 0;
}


/* @600a1231 _lipsync_cmd__blk_600a1231 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a1231
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,int param_5,
          uint *param_6)

{
  undefined4 uVar1;
  longlong lVar2;
  undefined8 uVar3;
  
  flix(0xa0b0ce3,0xd000941);
  flix(0x30b0ef3,0xd002741);
  if (param_3 == (undefined4 *)0x0) {
    __assert_failed(PTR_s__lipsync_cmd_600a06c8,0x289,PTR_s_param____NULL_600a0774);
  }
  flix(0x90b0589,0xd006d41);
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x28b,
                 PTR_s_Escape_idle_time____u_msec_600a06f8,*param_3);
  }
  if (*(int *)(param_1 + 0x1dc) == 0) {
    lVar2 = (ulonglong)DAT_600a073c << 0x20;
  }
  else {
    lVar2 = __floatunsidf(*(undefined4 *)(param_1 + param_5 + 0x70));
  }
  uVar3 = __floatunsidf(*param_3);
  uVar3 = __muldf3((int)uVar3,(int)((ulonglong)uVar3 >> 0x20),(int)lVar2,
                   (int)((ulonglong)lVar2 >> 0x20));
  __muldf3((int)uVar3,(int)((ulonglong)uVar3 >> 0x20),0,DAT_600a0740);
  uVar1 = __fixdfsi();
  *(undefined4 *)(param_1 + 0x188) = uVar1;
  return 0;
}


/* @600a1288 _lipsync_cmd__blk_600a1288 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 _lipsync_cmd__blk_600a1288(undefined4 param_1)

{
  undefined4 uVar1;
  
  flix(0x50b0cb3,0xd001041);
  flix(0x90b0cc3,0xd003d41);
  flix(0x80b0cd3,0xd003841);
  flix(0xc0b0ee3,0xd002d41);
  uVar1 = _lipsync_print_status(param_1);
  return uVar1;
}


/* @600a12ba _lipsync_cmd__blk_600a12ba */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_cmd__blk_600a12ba(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl();
}


/* @600a12cc _lipsync_cmd__blk_600a12cc */

undefined4
_lipsync_cmd__blk_600a12cc
          (int param_1,undefined4 param_2,int *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  int iVar1;
  int iVar2;
  
  aud_mutex_lock(param_1 + 0x23c);
  aud_cond_signal(param_1 + 600);
  aud_mutex_unlock(param_1 + 0x23c);
  flix(0x50b0c83,0xd000841);
  flix(0x20b0e93,0xd000e41);
  if (param_3 == (int *)0x0) {
    __assert_failed(PTR_s__lipsync_cmd_600a06c8,0x1f9,PTR_s_param____NULL_600a0774,0,DAT_600a06f0);
  }
  flix(0xb0b05ac,0xd004a41);
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1fb,PTR_s_clockbase____d_600a06d4,
                 *param_3);
  }
  iVar1 = *param_3;
  iVar2 = param_3[1];
  if (iVar1 == *(int *)(param_1 + 0x204)) {
    flix(0x10b08b8,0xd004a41);
  }
  *(undefined1 *)(param_1 + 0x231) = 1;
  *(int *)(param_1 + 0x204) = iVar1;
  *(int *)(param_1 + 0x20c) = iVar1;
  *(int *)(param_1 + 0x208) = iVar2;
  *(int *)(param_1 + 0x210) = iVar2;
  return 0;
}


/* @600a12cf _lipsync_cmd__blk_600a12cf */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a12cf
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  flix(0xa0b0cf3,0xd002241);
  flix(0x30b0e83,0xd003d41);
  if (param_3 == (undefined4 *)0x0) {
    __assert_failed(PTR_s__lipsync_cmd_600a06c8,0x23e,PTR_s_param____NULL_600a0774);
  }
  flix(0x50b059a,0xd007041);
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x240,PTR_s_Onoff____d_600a0710,
                 *param_3);
  }
  *(undefined4 *)(param_1 + 0x214) = *param_3;
  flix(0x50b01b0,0xd406341);
  return 0;
}


/* @600a1315 _lipsync_cmd__blk_600a1315 */

undefined4
_lipsync_cmd__blk_600a1315
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x241,PTR_s_upperbound____d_600a0714
                 ,param_3[1]);
  }
  *(undefined4 *)(param_1 + 0x214) = *param_3;
  flix(0x50b01b0,0xd406341);
  return 0;
}


/* @600a1332 _lipsync_cmd__blk_600a1332 */

undefined4
_lipsync_cmd__blk_600a1332
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x242,PTR_s_lowerbound____d_600a0718
                 ,param_3[2]);
  }
  *(undefined4 *)(param_1 + 0x214) = *param_3;
  flix(0x50b01b0,0xd406341);
  return 0;
}


/* @600a134c _lipsync_cmd__blk_600a134c */

undefined4
_lipsync_cmd__blk_600a134c
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x243,
                 PTR_s_prebyteper1sec____d_600a071c,param_3[3]);
  }
  *(undefined4 *)(param_1 + 0x214) = *param_3;
  flix(0x50b01b0,0xd406341);
  return 0;
}


/* @600a1366 _lipsync_cmd__blk_600a1366 */

undefined4
_lipsync_cmd__blk_600a1366
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x244,
                 PTR_s_posbyteper1sec____d_600a0720,param_3[4]);
  }
  *(undefined4 *)(param_1 + 0x214) = *param_3;
  flix(0x50b01b0,0xd406341);
  return 0;
}


/* @600a1391 _lipsync_cmd__blk_600a1391 */

undefined4 _lipsync_cmd__blk_600a1391(int param_1)

{
  undefined4 in_a13;
  
  flix(0x60b0cc3,0xd001c41);
  flix(0x60b0ee3,0xd001e41);
  *(undefined4 *)(param_1 + 0x154) = in_a13;
  *(undefined4 *)(param_1 + 0x18c) = in_a13;
  *(undefined4 *)(param_1 + 0x15c) = 6;
  *(char *)(param_1 + 0x269) = (char)in_a13;
  *(char *)(param_1 + 0x27d) = (char)in_a13;
  *(undefined1 *)(param_1 + 0x231) = 1;
  *(undefined4 *)(param_1 + 400) = 1;
  *(undefined4 *)(param_1 + 0x198) = in_a13;
  *(undefined4 *)(param_1 + 0x19c) = in_a13;
  *(undefined4 *)(param_1 + 0x148) = in_a13;
  *(undefined4 *)(param_1 + 0x14c) = in_a13;
  *(undefined4 *)(param_1 + 0x150) = in_a13;
  *(undefined4 *)(param_1 + 0x158) = in_a13;
  *(undefined4 *)(param_1 + 0x194) = in_a13;
  if (*(int *)(param_1 + 0x1e4) != 0) {
    *(int *)(param_1 + 0x140) = *(int *)(param_1 + 0x1e4);
    flix(0x40b02a3,0xd005b41);
  }
  *(undefined4 *)(param_1 + 0x138) = in_a13;
  *(undefined4 *)(param_1 + 0x13c) = in_a13;
  return 0;
}


/* @600a13f4 _lipsync_cmd__blk_600a13f4 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_cmd__blk_600a13f4
               (undefined4 param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,
               undefined4 param_5,uint *param_6)

{
  flix(0x70b0cb3,0xd001e41);
  flix(0x30b0ec3,0xd001841);
  flix(0x40b05de,0xd005541);
  if ((*param_6 & 8) == 0) {
                    /* WARNING: Subroutine does not return */
    __muldi3(*param_3,param_3[1],9,0);
  }
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x2d3,_LAB_600a072f_1);
}


/* @600a1437 _lipsync_cmd__blk_600a1437 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_cmd__blk_600a1437
               (undefined4 param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,
               undefined4 param_5,uint *param_6)

{
  if ((*param_6 & 8) == 0) {
                    /* WARNING: Subroutine does not return */
    __muldi3(*param_3,param_3[1],9,0);
  }
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x2d4,_LAB_600a0732_2);
}


/* @600a1462 _lipsync_cmd__blk_600a1462 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_cmd__blk_600a1462(int param_1,undefined4 param_2,int param_3)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  uVar1 = __udivdi3();
  uVar2 = *(undefined4 *)(param_3 + 0xc);
  *(undefined4 *)(param_1 + 0x224) = uVar1;
                    /* WARNING: Subroutine does not return */
  __muldi3(*(undefined4 *)(param_3 + 8),uVar2,9,0);
}


/* @600a1478 _lipsync_cmd__blk_600a1478 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 _lipsync_cmd__blk_600a1478(int param_1)

{
  undefined4 uVar1;
  int in_a6;
  
  uVar1 = __udivdi3();
  *(undefined4 *)(param_1 + 0x228) = uVar1;
  *(undefined1 *)(param_1 + in_a6 + 0xb4) = 1;
  return 0;
}


/* @600a14b5 _lipsync_cmd__blk_600a14b5 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a14b5
          (undefined4 param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,
          undefined4 param_5,uint *param_6)

{
  flix(0x80b0c83,0xd001b41);
  flix(0xe0b0e93,0xd003741);
  if (param_3 == (undefined4 *)0x0) {
    __assert_failed(PTR_s__lipsync_cmd_600a06c8,0x2ab,PTR_s_param____NULL_600a0774);
  }
  flix(0xe0b05da,0xd006841);
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x2ad,_LAB_600a074a_2,*param_3);
  }
  flix(0xa0b02d0,0xd006841);
  return 0;
}


/* @600a14f6 _lipsync_cmd__blk_600a14f6 */

undefined4 _lipsync_cmd__blk_600a14f6(void)

{
  flix(0xa0b02d0,0xd006841);
  return 0;
}


/* @600a1504 _lipsync_cmd__blk_600a1504 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a1504
          (int param_1,uint param_2,int *param_3,undefined4 param_4,int param_5,uint *param_6)

{
  undefined4 uVar1;
  
  flix(0x90b0cb3,0xd002041);
  if (_LAB_600a0753_1 < param_2) {
    uVar1 = aud_module_default_command(param_1,param_2,param_3,param_4);
    return uVar1;
  }
  if (param_3 == (int *)0x0) {
    __assert_failed(PTR_s__lipsync_cmd_600a06c8,0x22f,PTR_s_param____NULL_600a0774);
  }
  flix(0x60b05de,0xd005141);
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x231,_LAB_600a0756_2,*param_3);
  }
  if (*(int *)(param_1 + 0x1dc) == 0) {
    *(uint *)(param_1 + 0x168) = (uint)(*param_3 * 0x30) / *(uint *)(param_1 + 0x274);
    return 0;
  }
  *(uint *)(param_1 + 0x168) =
       (uint)(*param_3 * *(int *)(param_1 + param_5 + 0x70)) / *(uint *)(param_1 + 0x274);
  return 0;
}


/* @600a155e _lipsync_cmd__blk_600a155e */

undefined4 _lipsync_cmd__blk_600a155e(int param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  undefined4 in_a13;
  
  if (param_2 == DAT_6009f860) {
    *(undefined4 *)(param_1 + 0x1a0) = in_a13;
    *(undefined4 *)(param_1 + 0x1a4) = in_a13;
    *(undefined4 *)(param_1 + 0x1a8) = in_a13;
    *(undefined4 *)(param_1 + 0x1ac) = in_a13;
    *(undefined4 *)(param_1 + 0x224) = in_a13;
    *(undefined4 *)(param_1 + 0x228) = in_a13;
    *(char *)(param_1 + 0x220) = (char)in_a13;
    *(undefined4 *)(param_1 + 0x22c) = in_a13;
    *(undefined4 *)(param_1 + 0x234) = in_a13;
    return in_a13;
  }
  uVar1 = aud_module_default_command(param_1,param_2,param_3,param_4);
  return uVar1;
}


/* @600a159c _lipsync_cmd__blk_600a159c */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a159c
          (int param_1,uint param_2,undefined4 *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  undefined4 uVar1;
  
  flix(0x50b0ce3,0xd001b41);
  if (lgsrcSinctable_makeTable__blk_600a075e < param_2) {
    uVar1 = aud_module_default_command
                      (param_1,param_2,param_3,param_4,lgsrcSinctable_makeTable__blk_600a075a);
    return uVar1;
  }
  if (param_3 == (undefined4 *)0x0) {
    __assert_failed(PTR_s__lipsync_cmd_600a06c8,0x1ee,PTR_s_param____NULL_600a0774);
  }
  flix(0xe0b0589,0xd004d41);
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1f0,
                 lgsrcSinctable_makeTable__blk_600a0762,*param_3);
  }
  *(undefined4 *)(param_1 + 0x200) = *param_3;
  return 0;
}


/* @600a15e2 _lipsync_cmd__blk_600a15e2 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a15e2(undefined4 *param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uVar1;
  
  if (param_2 == lgsrcSinctable_makeTable__blk_600a0766) {
    flix(0x10b00c3,0xd3ff141);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1bf,
                 lgsrcSinctable_makeTable__blk_600a0766,*param_1);
  }
  uVar1 = aud_module_default_command(param_1,param_2,param_3,param_4);
  return uVar1;
}


/* @600a1608 _lipsync_cmd__blk_600a1608 */

undefined4 _lipsync_cmd__blk_600a1608(void)

{
  return 0;
}


/* @600a160c _lipsync_cmd__blk_600a160c */

void _lipsync_cmd__blk_600a160c(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1fa,s_LIPSYNC_CMD_SET_BASE_60051430)
  ;
}


/* @600a161e _lipsync_cmd__blk_600a161e */

undefined4
_lipsync_cmd__blk_600a161e
          (int param_1,undefined4 param_2,int *param_3,undefined4 param_4,int param_5,uint *param_6)

{
  int iVar1;
  int iVar2;
  
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1fb,PTR_s_clockbase____d_600a06d4,
                 *param_3);
  }
  iVar1 = *param_3;
  iVar2 = param_3[1];
  if (iVar1 == *(int *)(param_1 + 0x204)) {
    flix(0x10b08b8,0xd004a41);
  }
  *(undefined1 *)(param_1 + param_5 + 0xc5) = 1;
  *(int *)(param_1 + 0x204) = iVar1;
  *(int *)(param_1 + 0x20c) = iVar1;
  *(int *)(param_1 + 0x208) = iVar2;
  *(int *)(param_1 + 0x210) = iVar2;
  return 0;
}


/* @600a162a _lipsync_cmd__blk_600a162a */

undefined4 _lipsync_cmd__blk_600a162a(int param_1)

{
  flix(0x80b0b3d,0xd3ff541);
  flix(0x10b02e3,0xd003e41);
  *(undefined1 *)(param_1 + 0x268) = 1;
  return 0;
}


/* @600a164e _lipsync_cmd__blk_600a164e */

undefined4 _lipsync_cmd__blk_600a164e(int param_1)

{
  undefined4 unaff_a10;
  undefined4 unaff_a11;
  
  *(undefined4 *)(param_1 + 0x204) = unaff_a10;
  *(undefined4 *)(param_1 + 0x20c) = unaff_a10;
  *(undefined4 *)(param_1 + 0x208) = unaff_a11;
  *(undefined4 *)(param_1 + 0x210) = unaff_a11;
  return 0;
}


/* @600a1670 _lipsync_cmd__blk_600a1670 */

void _lipsync_cmd__blk_600a1670(void)

{
  return;
}


/* @600a1674 _lipsync_cmd__blk_600a1674 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_cmd__blk_600a1674(void)

{
  flix(0x50b0c83,0xd002041);
  flix(0xb0e93,0xd3ff041);
  flix(0xc0b02d3,0xd005141);
  flix(0x40b00d0,0xd3fe641);
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_cmd_600a06c8,0x2be,_LAB_600a0780);
}


/* @600a16b2 _lipsync_cmd__blk_600a16b2 */

undefined4 _lipsync_cmd__blk_600a16b2(void)

{
  return 0;
}


/* @600a16b8 _lipsync_cmd__blk_600a16b8 */

undefined4
_lipsync_cmd__blk_600a16b8
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,int param_5,
          uint *param_6)

{
  flix(0x30b0ca3,0xd002141);
  flix(0xf0b0eb3,0xd3feb41);
  if (param_3 == (undefined4 *)0x0) {
    __assert_failed(PTR_s__lipsync_cmd_600a06c8,0x273,PTR_s_param____NULL_600a0774);
  }
  flix(0xd0b05ce,0xd004e41);
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x275,
                 PTR_s_Type_0_pcm__1_es_____d_600a078c,*param_3);
  }
  *(undefined4 *)(param_1 + 0x1d4) = *param_3;
  flix(0xe0b01f0,0xd405041);
  *(undefined4 *)(param_1 + param_5 + 0x74) = 4;
  *(undefined4 *)(param_1 + param_5 + 0x18) = 1;
  return 0;
}


/* @600a16f8 _lipsync_cmd__blk_600a16f8 */

undefined4
_lipsync_cmd__blk_600a16f8
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,int param_5)

{
  *(undefined4 *)(param_1 + 0x1d4) = *param_3;
  flix(0xe0b01f0,0xd405041);
  *(undefined4 *)(param_1 + param_5 + 0x74) = 4;
  *(undefined4 *)(param_1 + param_5 + 0x18) = 1;
  return 0;
}


/* @600a1716 _lipsync_cmd__blk_600a1716 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a1716
          (int param_1,undefined4 param_2,uint *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  flix(0xc0b0b39,0xd3fe641);
  if (param_3 == (uint *)0x0) {
    __assert_failed(PTR_s__lipsync_cmd_600a06c8,0x20b,PTR_s_param____NULL_600a0774);
  }
  flix(0x10b05ac,0xd003c41);
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x20d,_DAT_600a0794,*param_3);
  }
  *(uint *)(param_1 + 0x1dc) = *param_3 / 1000;
  return 0;
}


/* @600a175a _lipsync_cmd__blk_600a175a */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a175a
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,int param_5,
          uint *param_6)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 in_a13;
  undefined4 uVar3;
  undefined4 uVar4;
  
  flix(0x40b0b3f,0xd3fe241);
  if (param_3 == (undefined4 *)0x0) {
    __assert_failed(PTR_s__lipsync_cmd_600a06c8,0x1d8,PTR_s_param____NULL_600a0774);
    in_a13 = 0;
  }
  flix(0xf0b0589,0xd003c41);
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1da,PTR_s_lbound____d_600a079c,
                 *param_3);
  }
  uVar1 = param_3[2];
  uVar2 = param_3[4];
  uVar3 = param_3[3];
  uVar4 = param_3[1];
  *(undefined4 *)(param_1 + 0x1f0) = *param_3;
  *(undefined4 *)(param_1 + 0x1ec) = uVar4;
  *(undefined4 *)(param_1 + 0x1fc) = uVar3;
  *(undefined4 *)(param_1 + 0x1f8) = uVar2;
  flix(0xa0b08ba,0xd002641);
  *(undefined1 *)(param_1 + param_5 + 0xc4) = 1;
  *(undefined4 *)(param_1 + 500) = uVar1;
  *(undefined4 *)(param_1 + 0x234) = in_a13;
  return 0;
}


/* @600a1795 _lipsync_cmd__blk_600a1795 */

undefined4
_lipsync_cmd__blk_600a1795
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,int param_5,
          uint *param_6)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1db,PTR_s_ubound____d_600a07a0,
                 param_3[1]);
  }
  uVar1 = param_3[2];
  uVar2 = param_3[4];
  uVar3 = param_3[3];
  uVar4 = param_3[1];
  *(undefined4 *)(param_1 + 0x1f0) = *param_3;
  *(undefined4 *)(param_1 + 0x1ec) = uVar4;
  *(undefined4 *)(param_1 + 0x1fc) = uVar3;
  *(undefined4 *)(param_1 + 0x1f8) = uVar2;
  flix(0xa0b08ba,0xd002641);
  *(undefined1 *)(param_1 + param_5 + 0xc4) = 1;
  *(undefined4 *)(param_1 + 500) = uVar1;
  *(undefined4 *)(param_1 + 0x234) = 0;
  return 0;
}


/* @600a17b4 _lipsync_cmd__blk_600a17b4 */

undefined4
_lipsync_cmd__blk_600a17b4
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,int param_5,
          uint *param_6)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1dc,PTR_s_offset____d_600a07a4,
                 param_3[2]);
  }
  uVar1 = param_3[2];
  uVar2 = param_3[4];
  uVar3 = param_3[3];
  uVar4 = param_3[1];
  *(undefined4 *)(param_1 + 0x1f0) = *param_3;
  *(undefined4 *)(param_1 + 0x1ec) = uVar4;
  *(undefined4 *)(param_1 + 0x1fc) = uVar3;
  *(undefined4 *)(param_1 + 0x1f8) = uVar2;
  flix(0xa0b08ba,0xd002641);
  *(undefined1 *)(param_1 + param_5 + 0xc4) = 1;
  *(undefined4 *)(param_1 + 500) = uVar1;
  *(undefined4 *)(param_1 + 0x234) = 0;
  return 0;
}


/* @600a17d0 _lipsync_cmd__blk_600a17d0 */

undefined4
_lipsync_cmd__blk_600a17d0
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,int param_5,
          uint *param_6)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1dd,
                 PTR_s_freerunlbound____d_600a07a8,param_3[3]);
  }
  uVar1 = param_3[2];
  uVar2 = param_3[4];
  uVar3 = param_3[3];
  uVar4 = param_3[1];
  *(undefined4 *)(param_1 + 0x1f0) = *param_3;
  *(undefined4 *)(param_1 + 0x1ec) = uVar4;
  *(undefined4 *)(param_1 + 0x1fc) = uVar3;
  *(undefined4 *)(param_1 + 0x1f8) = uVar2;
  flix(0xa0b08ba,0xd002641);
  *(undefined1 *)(param_1 + param_5 + 0xc4) = 1;
  *(undefined4 *)(param_1 + 500) = uVar1;
  *(undefined4 *)(param_1 + 0x234) = 0;
  return 0;
}


/* @600a17ec _lipsync_cmd__blk_600a17ec */

undefined4
_lipsync_cmd__blk_600a17ec
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,int param_5,
          uint *param_6)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1de,
                 PTR_s_freerunubound____d_600a07ac,param_3[4]);
  }
  uVar1 = param_3[2];
  uVar2 = param_3[4];
  uVar3 = param_3[3];
  uVar4 = param_3[1];
  *(undefined4 *)(param_1 + 0x1f0) = *param_3;
  *(undefined4 *)(param_1 + 0x1ec) = uVar4;
  *(undefined4 *)(param_1 + 0x1fc) = uVar3;
  *(undefined4 *)(param_1 + 0x1f8) = uVar2;
  flix(0xa0b08ba,0xd002641);
  *(undefined1 *)(param_1 + param_5 + 0xc4) = 1;
  *(undefined4 *)(param_1 + 500) = uVar1;
  *(undefined4 *)(param_1 + 0x234) = 0;
  return 0;
}


/* @600a1808 _lipsync_cmd__blk_600a1808 */

undefined4
_lipsync_cmd__blk_600a1808
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,int param_5)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  uVar1 = param_3[2];
  uVar2 = param_3[4];
  uVar3 = param_3[3];
  uVar4 = param_3[1];
  *(undefined4 *)(param_1 + 0x1f0) = *param_3;
  *(undefined4 *)(param_1 + 0x1ec) = uVar4;
  *(undefined4 *)(param_1 + 0x1fc) = uVar3;
  *(undefined4 *)(param_1 + 0x1f8) = uVar2;
  flix(0xa0b08ba,0xd002641);
  *(undefined1 *)(param_1 + param_5 + 0xc4) = 1;
  *(undefined4 *)(param_1 + 500) = uVar1;
  *(undefined4 *)(param_1 + 0x234) = 0;
  return 0;
}


/* @600a183c _lipsync_cmd__blk_600a183c */

undefined4 _lipsync_cmd__blk_600a183c(undefined4 param_1)

{
  undefined4 uVar1;
  
  uVar1 = _lipsync_print_metadata(param_1);
  return uVar1;
}


/* @600a1845 _lipsync_cmd__blk_600a1845 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a1845
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  flix(0xd0b0b3c,0xd3fd341);
  if (param_3 == (undefined4 *)0x0) {
    __assert_failed(PTR_s__lipsync_cmd_600a06c8,0x2b5,PTR_s_param____NULL_600a0774);
  }
  flix(0x40b05de,0xd003d41);
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x2b7,_DAT_600a07b4,*param_3);
  }
  *(undefined4 *)(param_1 + 0x144) = *param_3;
  return 0;
}


/* @600a1883 _lipsync_cmd__blk_600a1883 */

undefined4
_lipsync_cmd__blk_600a1883
          (int param_1,undefined4 param_2,int *param_3,undefined4 param_4,int param_5,uint *param_6)

{
  int iVar1;
  int iVar2;
  
  flix(0xf0b0b38,0xd3fcf41);
  if (param_3 == (int *)0x0) {
    __assert_failed(PTR_s__lipsync_cmd_600a06c8,0x216,PTR_s_param____NULL_600a0774);
  }
  flix(0x40b059a,0xd003b41);
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x220,PTR_s_Channels____d_600a07bc,
                 *param_3);
  }
  iVar1 = *param_3;
  iVar2 = iVar1 << 0xb;
  *(int *)(param_1 + 0x160) = iVar2;
  *(int *)(param_1 + 0x164) = iVar2;
  if (*(int *)(param_1 + 0x1d8) == 0) {
    *(int *)(param_1 + param_5 + 0x6c) = iVar1;
    return 0;
  }
  return 0;
}


/* @600a18d2 _lipsync_cmd__blk_600a18d2 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a18d2
          (int param_1,undefined4 param_2,int *param_3,undefined4 param_4,int param_5,uint *param_6)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  
  flix(0xe0b0b3e,0xd3fca41);
  if (param_3 == (int *)0x0) {
    __assert_failed(PTR_s__lipsync_cmd_600a06c8,0x260,PTR_s_param____NULL_600a0774);
  }
  flix(0x10b05f8,0xd003841);
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x262,PTR_s_Rate____d__d_600a07c4,
                 *param_3);
  }
  iVar2 = param_3[1];
  if (iVar2 == 0) {
    flix(0xa0b02a0,0xd003a41);
    *(uint *)(param_1 + 0x17c) = (uint)(*param_3 != 0);
    return 0;
  }
  iVar3 = *param_3;
  uVar4 = __floatunsidf(iVar3);
  uVar5 = __floatunsidf(iVar2);
  __divdf3((int)uVar4,(int)((ulonglong)uVar4 >> 0x20),(int)uVar5,(int)((ulonglong)uVar5 >> 0x20));
  uVar1 = __fixdfsi();
  *(undefined4 *)(param_1 + param_5 + 0x14) = uVar1;
  *(uint *)(param_1 + 0x17c) = (uint)(iVar3 != iVar2);
  return 0;
}


/* @600a192d _lipsync_cmd__blk_600a192d */

void _lipsync_cmd__blk_600a192d(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x28a,
               PTR_s_LIPSYNC_CMD_SET_ESCAPEIDLETIME_600a07c8);
}


/* @600a193f _lipsync_cmd__blk_600a193f */

undefined4
_lipsync_cmd__blk_600a193f
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,int param_5,
          uint *param_6)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x28b,
                 PTR_s_Escape_idle_time____u_msec_600a06f8,*param_3);
  }
  if (*(int *)(param_1 + 0x1dc) == 0) {
    uVar2 = 0;
    uVar1 = DAT_600a073c;
  }
  else {
    uVar3 = __floatunsidf(*(undefined4 *)(param_1 + param_5 + 0x70));
    uVar2 = (undefined4)uVar3;
    uVar1 = (int)((ulonglong)uVar3 >> 0x20);
  }
  uVar3 = __floatunsidf(*param_3);
  uVar3 = __muldf3((int)uVar3,(int)((ulonglong)uVar3 >> 0x20),uVar2,uVar1);
  __muldf3((int)uVar3,(int)((ulonglong)uVar3 >> 0x20),0,DAT_600a0740);
  uVar1 = __fixdfsi();
  *(undefined4 *)(param_1 + 0x188) = uVar1;
  return 0;
}


/* @600a194b _lipsync_cmd__blk_600a194b */

void _lipsync_cmd__blk_600a194b(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1ce,
               PTR_s_LIPSYNC_CMD_SET_ONOFF_600a07cc);
}


/* @600a195d _lipsync_cmd__blk_600a195d */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a195d
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1cf,
                 lgsrcSinctable_makeTable__blk_600a06eb,*param_3);
  }
  *(undefined4 *)(param_1 + 0x1e8) = *param_3;
  return 0;
}


/* @600a196a _lipsync_cmd__blk_600a196a */

void _lipsync_cmd__blk_600a196a(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x2d2,
               PTR_s_ADEC_CMD_SET_GAP_INFO_600a07d0);
}


/* @600a197c _lipsync_cmd__blk_600a197c */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_cmd__blk_600a197c
               (undefined4 param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,
               undefined4 param_5,uint *param_6)

{
  if ((*param_6 & 8) == 0) {
                    /* WARNING: Subroutine does not return */
    __muldi3(*param_3,param_3[1],9,0);
  }
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x2d3,_LAB_600a072f_1);
}


/* @600a199a _lipsync_cmd__blk_600a199a */

void _lipsync_cmd__blk_600a199a(undefined4 *param_1)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_start_600a07d4,0x176,
               PTR_s__s__set_discard_count____d_600a07d8,*param_1);
}


/* @600a19b2 _lipsync_cmd__blk_600a19b2 */

undefined4 _lipsync_cmd__blk_600a19b2(int param_1)

{
  *(undefined4 *)(param_1 + 0x138) = 0;
  *(undefined4 *)(param_1 + 0x13c) = 0;
  return 0;
}


/* @600a19be _lipsync_cmd__blk_600a19be */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a19be
          (int param_1,undefined4 param_2,int param_3,undefined4 param_4,int param_5)

{
  int iVar1;
  uint uVar2;
  int *piVar3;
  int iVar4;
  uint uVar5;
  undefined4 uStack00000000;
  
  iVar1 = _DAT_600a07dc;
  iVar4 = *(int *)(param_3 + 8);
  piVar3 = (int *)(param_1 + param_5);
  *piVar3 = *(int *)(param_3 + 4) * _DAT_600a07dc;
  uVar5 = *(uint *)(param_3 + 0xc);
  piVar3[1] = iVar4 * iVar1;
  uVar2 = lgsrcSinctable_makeTable__blk_600a07dd;
  if ((uVar5 == 0) || (*(uint *)(param_3 + 0x10) == 0)) {
    flix(0x60b00d0,0xd000e41);
    uStack00000000 = *(undefined4 *)(param_3 + 0x10);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_cmd_600a06c8,0x251,
                 PTR_s_invalid_param__pre__u__post__u_600a07f8);
  }
  piVar3[3] = lgsrcSinctable_makeTable__blk_600a07dd / *(uint *)(param_3 + 0x10);
  piVar3[2] = uVar2 / uVar5;
  return 0;
}


/* @600a19ec _lipsync_cmd__blk_600a19ec */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a19ec
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  __assert_failed(PTR_s__lipsync_cmd_600a06c8,0x1cd,PTR_s_param____NULL_600a0774);
  flix(0xf0b05ac,0xd007341);
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1cf,
                 lgsrcSinctable_makeTable__blk_600a06eb,*param_3);
  }
  *(undefined4 *)(param_1 + 0x1e8) = *param_3;
  return 0;
}


/* @600a19fe _lipsync_cmd__blk_600a19fe */

void _lipsync_cmd__blk_600a19fe(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x23f,
               PTR_s_LIPSYNC_CMD_SET_NODELAY_600a07e4);
}


/* @600a1a10 _lipsync_cmd__blk_600a1a10 */

undefined4
_lipsync_cmd__blk_600a1a10
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x240,PTR_s_Onoff____d_600a0710,
                 *param_3);
  }
  *(undefined4 *)(param_1 + 0x214) = *param_3;
  flix(0x50b01b0,0xd406341);
  return 0;
}


/* @600a1a1c _lipsync_cmd__blk_600a1a1c */

void _lipsync_cmd__blk_600a1a1c(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1b2,PTR_s_mark_flushing_600a07e8);
}


/* @600a1a2e _lipsync_cmd__blk_600a1a2e */

undefined4 _lipsync_cmd__blk_600a1a2e(int param_1)

{
  int in_a6;
  
  *(undefined1 *)(param_1 + in_a6 + 0xfc) = 1;
  return 0;
}


/* @600a1a3b _lipsync_cmd__blk_600a1a3b */

void _lipsync_cmd__blk_600a1a3b(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x230,
               PTR_s_LIPSYNC_CMD_SET_DEBUGPRINT_600a07ec);
}


/* @600a1a4d _lipsync_cmd__blk_600a1a4d */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a1a4d
          (int param_1,undefined4 param_2,int *param_3,undefined4 param_4,int param_5,uint *param_6)

{
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x231,_LAB_600a0756_2,*param_3);
  }
  if (*(int *)(param_1 + 0x1dc) == 0) {
    *(uint *)(param_1 + 0x168) = (uint)(*param_3 * 0x30) / *(uint *)(param_1 + 0x274);
    return 0;
  }
  *(uint *)(param_1 + 0x168) =
       (uint)(*param_3 * *(int *)(param_1 + param_5 + 0x70)) / *(uint *)(param_1 + 0x274);
  return 0;
}


/* @600a1a91 _lipsync_cmd__blk_600a1a91 */

undefined4 _lipsync_cmd__blk_600a1a91(int param_1)

{
  undefined4 unaff_a10;
  undefined4 in_a13;
  
  *(undefined4 *)(param_1 + 500) = unaff_a10;
  *(undefined4 *)(param_1 + 0x234) = in_a13;
  return 0;
}


/* @600a1a9b _lipsync_cmd__blk_600a1a9b */

void _lipsync_cmd__blk_600a1a9b(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1ef,
               PTR_s_LIPSYNC_CMD_SET_CLOCKTYPE_600a07f0);
}


/* @600a1aad _lipsync_cmd__blk_600a1aad */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a1aad
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1f0,
                 lgsrcSinctable_makeTable__blk_600a0762,*param_3);
  }
  *(undefined4 *)(param_1 + 0x200) = *param_3;
  return 0;
}


/* @600a1af2 _lipsync_cmd__blk_600a1af2 */

void _lipsync_cmd__blk_600a1af2(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x20c,
               PTR_s_LIPSYNC_CMD_SET_FS_600a07f4);
}


/* @600a1b04 _lipsync_cmd__blk_600a1b04 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a1b04
          (int param_1,undefined4 param_2,uint *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x20d,_DAT_600a0794,*param_3);
  }
  *(uint *)(param_1 + 0x1dc) = *param_3 / 1000;
  return 0;
}


/* @600a1b32 _lipsync_cmd__blk_600a1b32 */

undefined4 _lipsync_cmd__blk_600a1b32(void)

{
  return 2;
}


/* @600a1b48 _lipsync_cmd__blk_600a1b48 */

void _lipsync_cmd__blk_600a1b48(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1d9,
               PTR_s_LIPSYNC_CMD_SET_BOUND_600a07fc);
}


/* @600a1b5a _lipsync_cmd__blk_600a1b5a */

undefined4
_lipsync_cmd__blk_600a1b5a
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,int param_5,
          uint *param_6)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x1da,PTR_s_lbound____d_600a079c,
                 *param_3);
  }
  uVar1 = param_3[2];
  uVar2 = param_3[4];
  uVar3 = param_3[3];
  uVar4 = param_3[1];
  *(undefined4 *)(param_1 + 0x1f0) = *param_3;
  *(undefined4 *)(param_1 + 0x1ec) = uVar4;
  *(undefined4 *)(param_1 + 0x1fc) = uVar3;
  *(undefined4 *)(param_1 + 0x1f8) = uVar2;
  flix(0xa0b08ba,0xd002641);
  *(undefined1 *)(param_1 + param_5 + 0xc4) = 1;
  *(undefined4 *)(param_1 + 500) = uVar1;
  *(undefined4 *)(param_1 + 0x234) = 0;
  return 0;
}


/* @600a1b68 _lipsync_cmd__blk_600a1b68 */

void _lipsync_cmd__blk_600a1b68(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x2ac,
               PTR_s_INPUTCTRL_CMD_SET_BYPASS_600a0800);
}


/* @600a1b7a _lipsync_cmd__blk_600a1b7a */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a1b7a
          (undefined4 param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,
          undefined4 param_5,uint *param_6)

{
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x2ad,_LAB_600a074a_2,*param_3);
  }
  flix(0xa0b02d0,0xd006841);
  return 0;
}


/* @600a1b86 _lipsync_cmd__blk_600a1b86 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_cmd__blk_600a1b86(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_cmd_600a06c8,0x2ae,_LAB_600a0780);
}


/* @600a1b98 _lipsync_cmd__blk_600a1b98 */

undefined4 _lipsync_cmd__blk_600a1b98(void)

{
  return 0;
}


/* @600a1bb0 _lipsync_cmd__blk_600a1bb0 */

void _lipsync_cmd__blk_600a1bb0(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x2bd,
               PTR_s_INPUTCTRL_CMD_GET_SYSTEMDELAY_600a0804);
}


/* @600a1bc1 _lipsync_cmd__blk_600a1bc1 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_cmd__blk_600a1bc1(void)

{
  flix(0x40b00d0,0xd3fe641);
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_cmd_600a06c8,0x2be,_LAB_600a0780);
}


/* @600a1bc9 _lipsync_cmd__blk_600a1bc9 */

void _lipsync_cmd__blk_600a1bc9(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x274,
               PTR_s_LIPSYNC_CMD_SET_DATATYPE_600a0808);
}


/* @600a1bdb _lipsync_cmd__blk_600a1bdb */

undefined4
_lipsync_cmd__blk_600a1bdb
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,int param_5,
          uint *param_6)

{
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x275,
                 PTR_s_Type_0_pcm__1_es_____d_600a078c,*param_3);
  }
  *(undefined4 *)(param_1 + 0x1d4) = *param_3;
  flix(0xe0b01f0,0xd405041);
  *(undefined4 *)(param_1 + param_5 + 0x74) = 4;
  *(undefined4 *)(param_1 + param_5 + 0x18) = 1;
  return 0;
}


/* @600a1bfd _lipsync_cmd__blk_600a1bfd */

undefined4 _lipsync_cmd__blk_600a1bfd(void)

{
  return 2;
}


/* @600a1c13 _lipsync_cmd__blk_600a1c13 */

undefined4 _lipsync_cmd__blk_600a1c13(void)

{
  int unaff_a9;
  undefined4 in_a13;
  
  *(undefined4 *)(unaff_a9 + 0x18) = in_a13;
  *(undefined4 *)(unaff_a9 + 0x74) = 2;
  return 0;
}


/* @600a1c32 _lipsync_cmd__blk_600a1c32 */

void _lipsync_cmd__blk_600a1c32(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x2b6,
               PTR_s_INPUTCTRL_CMD_SET_AUTOTEST_600a080c);
}


/* @600a1c44 _lipsync_cmd__blk_600a1c44 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_cmd__blk_600a1c44
          (int param_1,undefined4 param_2,undefined4 *param_3,undefined4 param_4,undefined4 param_5,
          uint *param_6)

{
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x2b7,_DAT_600a07b4,*param_3);
  }
  *(undefined4 *)(param_1 + 0x144) = *param_3;
  return 0;
}


/* @600a1c50 _lipsync_cmd__blk_600a1c50 */

void _lipsync_cmd__blk_600a1c50(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x21f,
               PTR_s_LIPSYNC_CMD_SET_CHANNEL_600a0810);
}


/* @600a1c62 _lipsync_cmd__blk_600a1c62 */

undefined4
_lipsync_cmd__blk_600a1c62
          (int param_1,undefined4 param_2,int *param_3,undefined4 param_4,int param_5,uint *param_6)

{
  int iVar1;
  int iVar2;
  
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x220,PTR_s_Channels____d_600a07bc,
                 *param_3);
  }
  iVar1 = *param_3;
  iVar2 = iVar1 << 0xb;
  *(int *)(param_1 + 0x160) = iVar2;
  *(int *)(param_1 + 0x164) = iVar2;
  if (*(int *)(param_1 + 0x1d8) == 0) {
    *(int *)(param_1 + param_5 + 0x6c) = iVar1;
    return 0;
  }
  return 0;
}


/* @600a1c6e _lipsync_cmd__blk_600a1c6e */

void _lipsync_cmd__blk_600a1c6e(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x261,
               PTR_s_LIPSYNC_CMD_SET_RATE_600a0814);
}


/* @600a1c80 _lipsync_cmd__blk_600a1c80 */

undefined4
_lipsync_cmd__blk_600a1c80
          (int param_1,undefined4 param_2,int *param_3,undefined4 param_4,int param_5,uint *param_6)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  undefined8 uVar4;
  undefined8 uVar5;
  
  if ((*param_6 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_cmd_600a06c8,0x262,PTR_s_Rate____d__d_600a07c4,
                 *param_3);
  }
  iVar2 = param_3[1];
  if (iVar2 == 0) {
    flix(0xa0b02a0,0xd003a41);
    *(uint *)(param_1 + 0x17c) = (uint)(*param_3 != 0);
    return 0;
  }
  iVar3 = *param_3;
  uVar4 = __floatunsidf(iVar3);
  uVar5 = __floatunsidf(iVar2);
  __divdf3((int)uVar4,(int)((ulonglong)uVar4 >> 0x20),(int)uVar5,(int)((ulonglong)uVar5 >> 0x20));
  uVar1 = __fixdfsi();
  *(undefined4 *)(param_1 + param_5 + 0x14) = uVar1;
  *(uint *)(param_1 + 0x17c) = (uint)(iVar3 != iVar2);
  return 0;
}


/* @600a1cc2 _lipsync_cmd__blk_600a1cc2 */

void _lipsync_cmd__blk_600a1cc2(undefined4 param_1,undefined4 param_2)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_cmd_600a06c8,0x265,
               PTR_s_invalid_param__out_rate__u__600a0818,param_2);
}


/* @600a1cd7 _lipsync_cmd__blk_600a1cd7 */

undefined4 _lipsync_cmd__blk_600a1cd7(int param_1,undefined4 param_2,int *param_3)

{
  *(uint *)(param_1 + 0x17c) = (uint)(*param_3 != param_3[1]);
  return 0;
}


/* @600a1cec _lipsync_event */

undefined4 _lipsync_event(int param_1,undefined4 *param_2,int param_3)

{
  undefined *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined *puVar4;
  undefined1 uVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  char *__s1;
  uint uVar9;
  undefined4 uVar10;
  
  puVar2 = PTR_s__lipsync_event_600a081c;
  puVar1 = PTR_DAT_600a0634;
  if ((*SUB_600a062c & 8) != 0) {
    if (param_2 == (undefined4 *)0x0) {
      uVar10 = 0xffffffff;
    }
    else {
      uVar10 = *param_2;
    }
    uVar6 = aud_event_name(param_3);
                    /* WARNING: Subroutine does not return */
    logmm_printl(puVar1,3,puVar2,0x2e9,PTR_s_port_d__event__s_60042c14_4_600a0820,uVar10,uVar6);
  }
  aud_mutex_lock(param_1 + 0x23c);
  aud_cond_signal(param_1 + 600);
  aud_mutex_unlock(param_1 + 0x23c);
  if (*(int *)(param_3 + 0xc) != 1) {
    flix(0x40b02c0,0xd401441);
    if (*(int *)(param_3 + 0xc) == 7) {
      flix(0xb0b0283,0xd001141);
      flix(0x70b02a1,0xd001041);
      *(undefined1 *)(param_1 + 0x27c) = 1;
    }
    uVar10 = aud_module_default_event(param_1,param_2,param_3);
    return uVar10;
  }
  if (*(int *)(param_1 + 0x238) != 0) {
    aud_object_put();
  }
  uVar10 = aud_event_value_get_int(param_3,PTR_s_channels_6004d44c_4_600a0824,8);
  puVar3 = PTR_s_bits_per_sample_600a0828;
  *(undefined4 *)(param_1 + 0x1d8) = uVar10;
  iVar7 = aud_event_value_get_int(param_3,puVar3,0x20);
  puVar3 = PTR_s_rate_6004feb8_0xc_600a082c;
  iVar8 = iVar7 + 7;
  if (-1 < iVar7) {
    iVar8 = iVar7;
  }
  *(int *)(param_1 + 0x1e0) = iVar8 >> 3;
  iVar8 = aud_event_value_get_int(param_3,puVar3,DAT_6009f958);
  puVar3 = PTR_s_gap_correction_600a0830;
  *(int *)(param_1 + 0x1dc) = iVar8 / 1000;
  uVar10 = aud_event_value_get_int(param_3,puVar3,0);
  puVar3 = PTR_s_seamless_mode_600a0834;
  *(undefined4 *)(param_1 + 0x22c) = uVar10;
  uVar5 = aud_event_value_get_bool(param_3,puVar3,0);
  puVar4 = PTR_DAT_600a083c;
  puVar3 = PTR_s_codec_600a0838;
  *(undefined1 *)(param_1 + 0x27d) = uVar5;
  __s1 = (char *)aud_event_value_get_str(param_3,puVar3,puVar4);
  iVar8 = strcmp(__s1,PTR_DAT_600a083c);
  if (iVar8 == 0) {
    *(undefined4 *)(param_1 + 0x1e0) = 4;
    *(undefined4 *)(param_1 + 0x184) = 1;
    *(undefined4 *)(param_1 + 0x1d4) = 0;
    *(undefined4 *)(param_1 + 0x274) = 0x100;
  }
  else {
    *(undefined4 *)(param_1 + 0x1d4) = 1;
    uVar9 = aud_event_value_get_int(param_3,PTR_s_sample_number_600a0840,0x600);
    *(uint *)(param_1 + 0x274) = uVar9;
    *(undefined4 *)(param_1 + 0x1e0) = 2;
    *(undefined4 *)(param_1 + 0x184) = 0;
    *(uint *)(param_1 + 0x168) = (uint)(*(int *)(param_1 + 0x1dc) * 2000) / uVar9;
  }
  *(int *)(param_1 + 0x238) = param_3;
  if (*(int *)(param_1 + 0x110) != 0) {
    uVar10 = aud_object_get(param_3);
    aud_module_push_event(param_1,0,uVar10);
  }
  if ((*(char *)(param_1 + 0x27d) != '\0') && ((*SUB_600a062c & 4) != 0)) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(puVar1,2,puVar2,0x318,PTR_s_first_event__set_seamless_mode_600a084c);
  }
  *(undefined1 *)(param_1 + 0x268) = 1;
  return 0;
}


/* @600a1e70 _lipsync_event__blk_600a1e70 */

undefined4 _lipsync_event__blk_600a1e70(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  _lipsync_flush(param_1);
  uVar1 = aud_module_default_event(param_1,param_2,param_3);
  return uVar1;
}


/* @600a1e78 _lipsync_event__blk_600a1e78 */

void _lipsync_event__blk_600a1e78(void)

{
  undefined4 in_a6;
  undefined4 in_a7;
  
                    /* WARNING: Subroutine does not return */
  logmm_printl(in_a6,3,in_a7,0x333,PTR_s_flush_event_600a0844);
}


/* @600a1e8a _lipsync_event__blk_600a1e8a */

undefined4
_lipsync_event__blk_600a1e8a(int param_1,undefined4 param_2,undefined4 param_3,undefined1 param_4)

{
  undefined4 uVar1;
  
  flix(0x70b02a1,0xd001041);
  *(undefined1 *)(param_1 + 0x27c) = param_4;
  uVar1 = aud_module_default_event(param_1,param_2,param_3);
  return uVar1;
}


/* @600a1e90 _lipsync_event__blk_600a1e90 */

undefined4 _lipsync_event__blk_600a1e90(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  aimc_raise_named_event
            (*(undefined4 *)(param_1 + 0x68),*(undefined4 *)(param_1 + 4),PTR_DAT_600a0848,0,0);
  if (*(int *)(param_1 + 0x110) == 0) {
    aud_object_put(param_3);
    return 0;
  }
  if (*(int *)(param_1 + 0x238) != 0) {
    aud_object_put(*(int *)(param_1 + 0x238));
    *(undefined4 *)(param_1 + 0x238) = 0;
  }
  *(undefined4 *)(param_1 + 0x224) = 0;
  *(undefined4 *)(param_1 + 0x228) = 0;
  *(undefined1 *)(param_1 + 0x220) = 0;
  *(undefined4 *)(param_1 + 0x22c) = 0;
  *(undefined4 *)(param_1 + 0x110) = 0;
  uVar1 = aud_module_default_event(param_1,param_2,param_3);
  return uVar1;
}


/* @600a1ee6 _lipsync_event__blk_600a1ee6 */

undefined4
_lipsync_event__blk_600a1ee6(undefined4 param_1,undefined1 param_2,undefined4 param_3,int param_4)

{
  *(undefined1 *)(param_4 + 0x98) = param_2;
  return 0;
}


/* @600a1efc _lipsync_constructor */

undefined4 _lipsync_constructor(int param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  flix(0xb0283,0xd000d41);
  aud_data_adapter_init(param_1 + 0xf0);
  uVar2 = DAT_600a0850;
  uVar1 = DAT_6009fbe0;
  *(undefined4 *)(param_1 + 0x160) = DAT_6009fbe0;
  *(undefined4 *)(param_1 + 0x164) = uVar1;
  *(undefined4 *)(param_1 + 0x11c) = uVar1;
  *(undefined4 *)(param_1 + 0x1f8) = uVar2;
  *(undefined4 *)(param_1 + 0x1fc) = uVar2;
  *(undefined4 *)(param_1 + 0x184) = 1;
  *(undefined4 *)(param_1 + 0x274) = 0x100;
  *(undefined4 *)(param_1 + 0x1e0) = 4;
  *(undefined4 *)(param_1 + 0x188) = 0xb;
  *(int *)(param_1 + 0x114) = param_1 + 0x114;
  *(int *)(param_1 + 0x118) = param_1 + 0x114;
  uVar1 = DAT_6009fd08;
  *(undefined4 *)(param_1 + 0x168) = 0x177;
  *(undefined4 *)(param_1 + 0x1d8) = 2;
  *(undefined4 *)(param_1 + 0x1dc) = 0x30;
  *(undefined4 *)(param_1 + 0x128) = uVar1;
  *(int *)(param_1 + 0x130) = param_1 + 0x130;
  *(int *)(param_1 + 0x134) = param_1 + 0x130;
  *(undefined4 *)(param_1 + 0x1c0) = 0xffffffff;
  *(undefined4 *)(param_1 + 0x228) = 0;
  *(undefined4 *)(param_1 + 0x224) = 0;
  *(undefined4 *)(param_1 + 0x21c) = 0;
  *(undefined4 *)(param_1 + 0x1d4) = 0;
  *(undefined4 *)(param_1 + 0x1bc) = 0;
  *(undefined4 *)(param_1 + 0x1b8) = 0;
  *(undefined4 *)(param_1 + 500) = 0x96;
  *(undefined4 *)(param_1 + 0x17c) = 0;
  *(undefined4 *)(param_1 + 0x214) = 0;
  *(undefined4 *)(param_1 + 0x10c) = 0;
  *(undefined4 *)(param_1 + 0x238) = 0;
  *(undefined4 *)(param_1 + 0x1e4) = 0;
  *(undefined4 *)(param_1 + 0x140) = 0;
  *(undefined4 *)(param_1 + 0x124) = DAT_6009fd04;
  *(undefined4 *)(param_1 + 0x180) = 1000;
  *(undefined1 *)(param_1 + 0x220) = 0;
  *(undefined4 *)(param_1 + 0x22c) = 0;
  *(undefined1 *)(param_1 + 0x232) = 0;
  *(undefined1 *)(param_1 + 0x27d) = 0;
  *(undefined1 *)(param_1 + 0x231) = 0;
  *(undefined1 *)(param_1 + 0x230) = 0;
  *(undefined4 *)(param_1 + 0x26c) = 0;
  *(undefined4 *)(param_1 + 0x110) = 0;
  aud_mutex_init(param_1 + 0x23c);
  aud_cond_init(param_1 + 600);
  return 0;
}


/* @600a1fd8 _lipsync_constructor__blk_600a1fd8 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_constructor__blk_600a1fd8(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,_DAT_600a0854,0x34a,_DAT_600a0858);
}


/* @600a1fe9 _lipsync_constructor__blk_600a1fe9 */

undefined4 _lipsync_constructor__blk_600a1fe9(int param_1)

{
  undefined4 uVar1;
  undefined4 uVar2;
  
  aud_data_adapter_init(param_1 + 0xf0);
  uVar2 = DAT_600a0850;
  uVar1 = DAT_6009fbe0;
  *(undefined4 *)(param_1 + 0x160) = DAT_6009fbe0;
  *(undefined4 *)(param_1 + 0x164) = uVar1;
  *(undefined4 *)(param_1 + 0x11c) = uVar1;
  *(undefined4 *)(param_1 + 0x1f8) = uVar2;
  *(undefined4 *)(param_1 + 0x1fc) = uVar2;
  *(undefined4 *)(param_1 + 0x184) = 1;
  *(undefined4 *)(param_1 + 0x274) = 0x100;
  *(undefined4 *)(param_1 + 0x1e0) = 4;
  *(undefined4 *)(param_1 + 0x188) = 0xb;
  *(int *)(param_1 + 0x114) = param_1 + 0x114;
  *(int *)(param_1 + 0x118) = param_1 + 0x114;
  uVar1 = DAT_6009fd08;
  *(undefined4 *)(param_1 + 0x168) = 0x177;
  *(undefined4 *)(param_1 + 0x1d8) = 2;
  *(undefined4 *)(param_1 + 0x1dc) = 0x30;
  *(undefined4 *)(param_1 + 0x128) = uVar1;
  *(int *)(param_1 + 0x130) = param_1 + 0x130;
  *(int *)(param_1 + 0x134) = param_1 + 0x130;
  *(undefined4 *)(param_1 + 0x1c0) = 0xffffffff;
  *(undefined4 *)(param_1 + 0x228) = 0;
  *(undefined4 *)(param_1 + 0x224) = 0;
  *(undefined4 *)(param_1 + 0x21c) = 0;
  *(undefined4 *)(param_1 + 0x1d4) = 0;
  *(undefined4 *)(param_1 + 0x1bc) = 0;
  *(undefined4 *)(param_1 + 0x1b8) = 0;
  *(undefined4 *)(param_1 + 500) = 0x96;
  *(undefined4 *)(param_1 + 0x17c) = 0;
  *(undefined4 *)(param_1 + 0x214) = 0;
  *(undefined4 *)(param_1 + 0x10c) = 0;
  *(undefined4 *)(param_1 + 0x238) = 0;
  *(undefined4 *)(param_1 + 0x1e4) = 0;
  *(undefined4 *)(param_1 + 0x140) = 0;
  *(undefined4 *)(param_1 + 0x124) = DAT_6009fd04;
  *(undefined4 *)(param_1 + 0x180) = 1000;
  *(undefined1 *)(param_1 + 0x220) = 0;
  *(undefined4 *)(param_1 + 0x22c) = 0;
  *(undefined1 *)(param_1 + 0x232) = 0;
  *(undefined1 *)(param_1 + 0x27d) = 0;
  *(undefined1 *)(param_1 + 0x231) = 0;
  *(undefined1 *)(param_1 + 0x230) = 0;
  *(undefined4 *)(param_1 + 0x26c) = 0;
  *(undefined4 *)(param_1 + 0x110) = 0;
  aud_mutex_init(param_1 + 0x23c);
  aud_cond_init(param_1 + 600);
  return 0;
}


/* @600a1ff0 _lipsync_destructor */

void _lipsync_destructor(int param_1)

{
  if ((*SUB_600a062c & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_destructor_600a085c,0x38d,
                 PTR_s_destroy_LIPSYNC_600a0860);
  }
  msf_flush_metadata(param_1 + 0x114);
  aud_data_adapter_cleanup(param_1 + 0xf0);
  if (*(int *)(param_1 + 0x238) != 0) {
    aud_object_put();
  }
  return;
}


/* @600a202b _lipsync_destructor__blk_600a202b */

void _lipsync_destructor__blk_600a202b(int param_1)

{
  msf_flush_metadata(param_1 + 0x114);
  aud_data_adapter_cleanup(param_1 + 0xf0);
  if (*(int *)(param_1 + 0x238) != 0) {
    aud_object_put();
  }
  return;
}


/* @600a2034 _lipsync_put_data */

undefined4 _lipsync_put_data(int param_1,undefined4 param_2)

{
  aud_data_adapter_flush(param_1 + 0xf0,param_2);
  if ((*SUB_600a062c & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_put_data_600a0864,0x3bc,
                 PTR_s_put_size__u__total__u_600a0868,param_2,
                 *(int *)(param_1 + 0x100) - *(int *)(param_1 + 0x104));
  }
  return 0;
}


/* @600a2072 _lipsync_put_data__blk_600a2072 */

undefined4 _lipsync_put_data__blk_600a2072(void)

{
  return 0;
}


/* @600a2078 _lipsync_get_pts_for_media */

/* WARNING: Control flow encountered bad instruction data */
/* WARNING: Removing unreachable block (ram,0x600a2133) */

void _lipsync_get_pts_for_media(int param_1,uint param_2)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  undefined8 uVar6;
  
  iVar3 = *(int *)(param_1 + 0x208);
  uVar4 = SUB_600a086c;
  if (*(int *)(param_1 + 0x200) == 0x11) {
    uVar4 = DAT_600a035c;
  }
  param_2 = uVar4 & param_2;
  iVar2 = *(int *)(param_1 + 0x204);
  if (*(int *)(param_1 + 0x17c) == 0) {
    uVar5 = (iVar2 + param_2) - iVar3;
    if ((*SUB_600a062c & 0x10) != 0) {
      uVar6 = ConnectionMng_Init(*(undefined4 *)(param_1 + 0x180));
      uVar6 = __muldf3((int)uVar6,(int)((ulonglong)uVar6 >> 0x20),DAT_600a0874,DAT_600a0870);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_get_pts_for_media_600a0878,0x3db,
                   PTR_s_BaseCT__9d____ts__9d___BaseST__9_600a087c,iVar2,param_2,iVar3,uVar6,uVar5,
                   uVar4 & uVar5);
    }
    return;
  }
  param_2 = param_2 - iVar3;
  iVar3 = (int)param_2 >> 0x1f;
  uVar5 = param_2;
  if (*(char *)(param_1 + 0x27d) == '\0') {
    bVar1 = false;
    if ((iVar3 < 1) && ((int)param_2 < 0)) {
      bVar1 = true;
    }
    if (bVar1) {
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
    uVar5 = param_2 & uVar4;
    iVar3 = (int)(param_2 & uVar4) >> 0x1f;
  }
                    /* WARNING: Subroutine does not return */
  __muldi3(uVar5,iVar3,1000,0);
}


/* @600a20b4 _lipsync_get_pts_for_media__blk_600a20b4 */

void _lipsync_get_pts_for_media__blk_600a20b4
               (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
               undefined4 *param_5)

{
  undefined8 uVar1;
  
  __divdi3();
  if ((*SUB_600a062c & 0x10) != 0) {
    uVar1 = ConnectionMng_Init(*param_5);
    __muldf3((int)uVar1,(int)((ulonglong)uVar1 >> 0x20),DAT_600a0874,DAT_600a0870);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_get_pts_for_media_600a0878,0x3db,
                 PTR_s_BaseCT__9d____ts__9d___BaseST__9_600a087c,param_2);
  }
  return;
}


/* @600a20fe _lipsync_get_pts_for_media__blk_600a20fe */

void _lipsync_get_pts_for_media__blk_600a20fe(void)

{
  return;
}


/* @600a211a _lipsync_get_pts_for_media__blk_600a211a */

void _lipsync_get_pts_for_media__blk_600a211a(int param_1)

{
  uint in_a7;
  uint unaff_a8;
  uint in_a12;
  int in_a13;
  
                    /* WARNING: Subroutine does not return */
  __muldi3(unaff_a8 & in_a7,*(int *)(param_1 + 0x18) + in_a13 & in_a12,1000,0);
}


/* @600a2138 lipsync_fadein */

/* WARNING: Control flow encountered bad instruction data */

void lipsync_fadein(uint param_1,undefined4 param_2,undefined4 *param_3,uint param_4)

{
  flix(0x60b0283,0xd000a41);
  flix(0xc0b0146,0xd400941);
  if ((*SUB_600a062c & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s_lipsync_fadein_600a0880,0x3f0,
                 PTR_s__d____x_<___x_length__d_600a0884,0,0,*param_3,param_4 / param_1 >> 2);
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}


/* @600a21ae lipsync_fadein__blk_600a21ae */

undefined8 lipsync_fadein__blk_600a21ae(void)

{
  undefined4 uVar1;
  int extraout_a3;
  int extraout_a4;
  int in_a6;
  int unaff_a8;
  int unaff_a10;
  int in_stack_00000018;
  int in_stack_0000001c;
  
  func_0x600bd8dc();
  func_0x600bbc1c();
  uVar1 = *(undefined4 *)(unaff_a8 + 0x20);
  *(int *)(in_a6 + unaff_a10) = (*(int *)(in_a6 + unaff_a8) / extraout_a4) * 0xa2dff9a;
  if (0 < in_stack_0000001c + -1) {
    ((int *)(in_a6 + unaff_a10))[1] = (((int *)(in_a6 + unaff_a8))[1] / extraout_a4) * 0xa2dff9a;
  }
  flix(0x80b0b94,0xd3ff841);
  return CONCAT44(extraout_a3 + in_stack_00000018,uVar1);
}


/* @600a21fc lipsync_fadein__blk_600a21fc */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void lipsync_fadein__blk_600a21fc(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uStack00000000;
  
  uStack00000000 = param_3;
                    /* WARNING: Subroutine does not return */
  logmm_printl();
}


/* @600a2212 lipsync_fadein__blk_600a2212 */

/* WARNING: Control flow encountered bad instruction data */

void lipsync_fadein__blk_600a2212(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  undefined4 uStack00000008;
  undefined4 *in_stack_00000014;
  int iStack00000018;
  int in_stack_0000001c;
  undefined4 uStack00000020;
  
  flix(0xc0b0146,0xd400941);
  uStack00000020 = 0;
  iStack00000018 = in_stack_0000001c << 2;
  if ((*SUB_600a062c & 0x20) != 0) {
    uStack00000004 = *in_stack_00000014;
    uStack00000000 = 0;
    uStack00000008 = param_3;
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s_lipsync_fadein_600a0880,0x3f0,
                 PTR_s__d____x_<___x_length__d_600a0884,0);
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}


/* @600a2218 lipsync_fadeout */

/* WARNING: Control flow encountered bad instruction data */

void lipsync_fadeout(uint param_1,undefined4 param_2,int *param_3,uint param_4)

{
  uint uVar1;
  
  uVar1 = param_4 / param_1 >> 2;
  flix(0x90b0283,0xd000941);
  flix(0xf0b0126,0xd400841);
  if ((*SUB_600a062c & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s_lipsync_fadeout_600a088c,0x40b,
                 PTR_s__d____x_<___x_6004c2f0_4_600a0890,uVar1,uVar1 * (*param_3 / (int)uVar1),
                 *param_3);
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}


/* @600a228d lipsync_fadeout__blk_600a228d */

void lipsync_fadeout__blk_600a228d
               (int param_1,undefined4 param_2,undefined4 param_3,int param_4,int param_5)

{
  int unaff_a9;
  int in_stack_00000010;
  int in_stack_00000014;
  int in_stack_0000001c;
  
  if (0 < unaff_a9) {
    *(int *)(param_5 + in_stack_00000010) =
         param_4 * (*(int *)(param_5 + in_stack_00000014) / param_1);
    if (0 < in_stack_0000001c + -1) {
      ((int *)(param_5 + in_stack_00000010))[1] =
           param_4 * (((int *)(param_5 + in_stack_00000014))[1] / param_1);
    }
  }
  flix(0xe0b0b32,0xd3ff841);
  return;
}


/* @600a22d5 lipsync_fadeout__blk_600a22d5 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void lipsync_fadeout__blk_600a22d5(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl();
}


/* @600a22e9 lipsync_fadeout__blk_600a22e9 */

/* WARNING: Control flow encountered bad instruction data */

void lipsync_fadeout__blk_600a22e9(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iStack00000000;
  int iStack00000004;
  int *in_stack_00000014;
  int iStack00000018;
  int in_stack_0000001c;
  
  flix(0xf0b0126,0xd400841);
  iStack00000018 = in_stack_0000001c << 2;
  if ((*SUB_600a062c & 0x20) != 0) {
    iStack00000004 = *in_stack_00000014;
    iStack00000000 = param_4 * (iStack00000004 / param_1);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s_lipsync_fadeout_600a088c,0x40b,
                 PTR_s__d____x_<___x_6004c2f0_4_600a0890,param_4);
  }
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}


/* @600a22ec _lipsync_write_lipsync_state */

undefined4 _lipsync_write_lipsync_state(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x15c);
  if (iVar1 == param_2) {
    return 0;
  }
  if (((((iVar1 == 2) && (param_2 == 3)) || ((iVar1 == 3 && (param_2 == 7)))) ||
      ((iVar1 == 7 && (param_2 == 3)))) && (*(uint *)(param_1 + 0x1a0) < 0xff)) {
    *(uint *)(param_1 + 0x1a0) = *(uint *)(param_1 + 0x1a0) + 1;
  }
  if (((iVar1 == 3) && (param_2 == 7)) &&
     ((*(int *)(param_1 + 0x200) == 1 || (*(int *)(param_1 + 0x200) == 2)))) {
    *(undefined4 *)(param_1 + 0x234) = 0x40;
  }
  if (((iVar1 == 2) && (param_2 == 4)) && (*(uint *)(param_1 + 0x1a4) < 0xff)) {
    *(uint *)(param_1 + 0x1a4) = *(uint *)(param_1 + 0x1a4) + 1;
  }
  else if (param_2 == 6) {
    if (*(uint *)(param_1 + 0x1a8) < 0xff) {
      *(uint *)(param_1 + 0x1a8) = *(uint *)(param_1 + 0x1a8) + 1;
    }
    *(undefined4 *)(param_1 + 0x234) = 0;
  }
  if ((param_2 == 5) && (*(uint *)(param_1 + 0x1ac) < 0xff)) {
    *(uint *)(param_1 + 0x1ac) = *(uint *)(param_1 + 0x1ac) + 1;
    return 0;
  }
  return 0;
}


/* @600a2378 _lipsync_write_metadata */

undefined4 _lipsync_write_metadata(int param_1,undefined4 param_2,int param_3)

{
  uint *puVar1;
  undefined *puVar2;
  undefined4 uVar3;
  undefined4 uVar4;
  int iVar5;
  undefined4 extraout_a3;
  undefined *puVar6;
  
  puVar2 = PTR_DAT_600a0634;
  puVar1 = SUB_600a062c;
  puVar6 = PTR_s__lipsync_write_metadata_600a0898;
  if ((((*(int *)(param_1 + 0x218) == 0) && (*(int *)(param_1 + 0x140) < 1)) && (param_3 == 0)) &&
     ((*(int *)(param_1 + 0x110) == 0 && (*(int *)(param_1 + 0x238) != 0)))) {
    uVar3 = aud_event_str(*(int *)(param_1 + 0x238),0,PTR_s__lipsync_write_metadata_600a0898);
    uVar4 = aud_object_get(*(undefined4 *)(param_1 + 0x238));
    aud_module_push_event(param_1,0,uVar4);
    flix(0x60b02c3,0xd001241);
    _BufMonitor_RemoveBufferIndex(uVar3,extraout_a3,*puVar1);
    puVar6 = PTR_s__lipsync_write_metadata_600a0898;
    *(undefined4 *)(param_1 + 0x110) = 1;
  }
  if ((*(int *)(param_1 + 0x1d4) == 1) ||
     ((*(char *)(param_1 + 0x27d) == '\0' &&
      ((*(uint *)(param_1 + 0x1d8) < 3 || (*(int *)(param_1 + 0x17c) == 1)))))) {
    if ((*puVar1 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
      logmm_printl(puVar2,5,puVar6,0x4aa,PTR_s_this_lipsync_module_is_ES_mode___600a089c,
                   *(undefined4 *)(param_1 + 0x1d4),*(undefined4 *)(param_1 + 0x1d8),
                   *(undefined4 *)(param_1 + 0x17c));
    }
    return 0;
  }
  if (*(int *)(param_1 + 0x218) != 0) {
    if (param_3 == 0) {
      msf_update_read_buf(param_1 + 0x114,param_2);
      return 0;
    }
    flix(0xc0b0140,0xd400d41);
    return 0;
  }
  flix(0xb0b01e4,0xd7ff641);
  if (param_3 == 0) {
    while( true ) {
      iVar5 = msf_get_metadata(param_1 + 0x114,param_2);
      if (iVar5 == 0) break;
      msf_add_put_metadata(param_1 + 0x130,iVar5);
      aud_meta_data_free(iVar5);
    }
LAB_600a24c3:
    msf_add_update_write_buf(param_1 + 0x130,param_2);
    if (param_3 == 0) {
      msf_update_read_buf(param_1 + 0x114,param_2);
      goto LAB_600a2450;
    }
  }
  else if (param_3 == 2) goto LAB_600a24c3;
  flix(0x10b0140,0xd400841);
LAB_600a2450:
  if ((*(int *)(param_1 + 0x280) != 0) && ((param_3 == 0 || (param_3 == 2)))) {
    flix(0xb0b02a5,0xd000841);
    aud_module_push_data(param_1,0,*(undefined4 *)(param_1 + 0x280));
    aud_module_sync(param_1,0);
    *(undefined4 *)(param_1 + 0x280) = 0;
  }
  iVar5 = aud_data_new_metadata(param_1 + 0x130);
  if (iVar5 == 0) {
    return 0;
  }
  if ((*puVar1 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_write_metadata_600a0898,0x4ed,
                 PTR_s_push_metadata___size__d_600a08a0,*(undefined4 *)(iVar5 + 0x34));
  }
  aud_module_push_data(param_1,0,iVar5);
  aud_module_sync(param_1,0);
  return 0;
}


/* @600a23e0 _lipsync_write_metadata__blk_600a23e0 */

undefined4 _lipsync_write_metadata__blk_600a23e0(void)

{
  return 0;
}


/* @600a24f5 _lipsync_write_metadata__blk_600a24f5 */

undefined4 _lipsync_write_metadata__blk_600a24f5(undefined4 param_1,undefined4 param_2)

{
  aud_module_push_data(param_1,0,param_2);
  aud_module_sync(param_1,0);
  return 0;
}


/* @600a2525 _lipsync_write_metadata__blk_600a2525 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_write_metadata__blk_600a2525(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_write_metadata_600a0898,0x4e0,_DAT_600a08a4);
}


/* @600a2536 _lipsync_write_metadata__blk_600a2536 */

undefined4 _lipsync_write_metadata__blk_600a2536(int param_1)

{
  int iVar1;
  uint *in_a6;
  undefined4 in_a7;
  
  aud_module_push_data(param_1,0,*(undefined4 *)(param_1 + 0x280));
  aud_module_sync(param_1,0);
  *(undefined4 *)(param_1 + 0x280) = 0;
  iVar1 = aud_data_new_metadata(in_a7);
  if (iVar1 == 0) {
    return 0;
  }
  if ((*in_a6 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_write_metadata_600a0898,0x4ed,
                 PTR_s_push_metadata___size__d_600a08a0,*(undefined4 *)(iVar1 + 0x34));
  }
  aud_module_push_data(param_1,0,iVar1);
  aud_module_sync(param_1,0);
  return 0;
}


/* @600a253c _lipsync_write_metadata__blk_600a253c */

void _lipsync_write_metadata__blk_600a253c(void)

{
  undefined4 in_a7;
  undefined4 in_stack_00000010;
  
                    /* WARNING: Subroutine does not return */
  logmm_printl(in_a7,3,PTR_s__lipsync_write_metadata_600a0898,0x49e,
               PTR_s_send_first_event___s__600a08a8,in_stack_00000010);
}


/* @600a2551 _lipsync_write_metadata__blk_600a2551 */

undefined4
_lipsync_write_metadata__blk_600a2551
          (int param_1,undefined4 param_2,int param_3,int param_4,uint *param_5,undefined4 param_6)

{
  undefined *puVar1;
  int iVar2;
  undefined4 in_stack_00000010;
  
  _BufMonitor_RemoveBufferIndex(in_stack_00000010);
  puVar1 = PTR_s__lipsync_write_metadata_600a0898;
  *(undefined4 *)(param_1 + 0x110) = 1;
  if ((*(int *)(param_4 + 0x28) == 1) ||
     ((*(char *)(param_4 + 0xd1) == '\0' &&
      ((*(uint *)(param_1 + 0x1d8) < 3 || (*(int *)(param_1 + 0x17c) == 1)))))) {
    if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
      logmm_printl(param_6,5,puVar1,0x4aa,PTR_s_this_lipsync_module_is_ES_mode___600a089c,
                   *(undefined4 *)(param_1 + 0x1d4));
    }
    return 0;
  }
  if (*(int *)(param_4 + 0x6c) != 0) {
    if (param_3 == 0) {
      msf_update_read_buf(param_1 + 0x114,param_2);
      return 0;
    }
    flix(0xc0b0140,0xd400d41);
    return 0;
  }
  flix(0xb0b01e4,0xd7ff641);
  if (param_3 == 0) {
    while( true ) {
      iVar2 = msf_get_metadata(param_1 + 0x114,param_2);
      if (iVar2 == 0) break;
      msf_add_put_metadata(param_1 + 0x130,iVar2);
      aud_meta_data_free(iVar2);
    }
LAB_600a24c3:
    msf_add_update_write_buf(param_1 + 0x130,param_2);
    if (param_3 == 0) {
      msf_update_read_buf(param_1 + 0x114,param_2);
      goto LAB_600a2450;
    }
  }
  else if (param_3 == 2) goto LAB_600a24c3;
  flix(0x10b0140,0xd400841);
LAB_600a2450:
  if ((*(int *)(param_4 + 0xd4) != 0) && ((param_3 == 0 || (param_3 == 2)))) {
    flix(0xb0b02a5,0xd000841);
    aud_module_push_data(param_1,0,*(undefined4 *)(param_1 + 0x280));
    aud_module_sync(param_1,0);
    *(undefined4 *)(param_1 + 0x280) = 0;
  }
  iVar2 = aud_data_new_metadata(param_1 + 0x130);
  if (iVar2 == 0) {
    return 0;
  }
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_write_metadata_600a0898,0x4ed,
                 PTR_s_push_metadata___size__d_600a08a0,*(undefined4 *)(iVar2 + 0x34));
  }
  aud_module_push_data(param_1,0,iVar2);
  aud_module_sync(param_1,0);
  return 0;
}


/* @600a2554 _lipsync_write_output */

undefined4 _lipsync_write_output(undefined4 *param_1,int param_2)

{
  uint *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  
  puVar3 = PTR_s__lipsync_write_output_600a08ac;
  puVar2 = PTR_DAT_600a0634;
  puVar1 = SUB_600a062c;
  if ((param_1[0x86] != 0) || (0 < (int)param_1[0x50])) {
    if ((*SUB_600a062c & 8) != 0) {
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_write_output_600a08ac,0x530,
                   PTR_s__s__drop_data__count____d__600a08b0,*param_1,param_1[0x50]);
    }
    aud_object_put(param_2);
    return 0;
  }
  if ((param_1[0x44] == 0) && (param_1[0x8e] != 0)) {
    uVar4 = aud_event_str();
    uVar5 = aud_object_get(param_1[0x8e]);
    aud_module_push_event(param_1,0,uVar5);
    if ((*puVar1 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
      logmm_printl(puVar2,3,puVar3,0x507,PTR_s_send_first_event___s__600a08a8,uVar4);
    }
    _BufMonitor_RemoveBufferIndex(uVar4);
    param_1[0x44] = 1;
  }
  iVar6 = param_1[0x51];
  iVar7 = param_1[0x75];
  *(undefined4 *)(param_2 + 0x18) = param_1[0x74];
  *(uint *)(param_2 + 0x10) = (uint)(iVar7 == 1);
  if (iVar6 != 0) {
    AuiLogger_Init(*(undefined4 *)(param_2 + 0x38),0x1f,*(undefined4 *)(param_2 + 0x34));
  }
  if ((*puVar1 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(puVar2,5,puVar3,0x528,PTR_s_push_PCM___size__d_600a08b4,
                 *(undefined4 *)(param_2 + 0x34));
  }
  aud_module_push_data(param_1,0,param_2);
  aud_module_sync(param_1,0);
  return 0;
}


/* @600a25e1 _lipsync_write_output__blk_600a25e1 */

undefined4 _lipsync_write_output__blk_600a25e1(undefined4 param_1,undefined4 param_2)

{
  aud_object_put(param_2);
  return 0;
}


/* @600a25f8 _lipsync_write_output__blk_600a25f8 */

undefined4 _lipsync_write_output__blk_600a25f8(undefined4 param_1,undefined4 param_2)

{
  aud_module_push_data(param_1,0,param_2);
  aud_module_sync(param_1,0);
  return 0;
}


/* @600a2641 _lipsync_write_output__blk_600a2641 */

undefined4
_lipsync_write_output__blk_600a2641
          (int param_1,int param_2,undefined4 param_3,undefined4 param_4,uint *param_5,
          undefined4 param_6)

{
  int iVar1;
  int iVar2;
  
  _BufMonitor_RemoveBufferIndex(param_6);
  *(undefined4 *)(param_1 + 0x110) = 1;
  iVar1 = *(int *)(param_1 + 0x144);
  iVar2 = *(int *)(param_1 + 0x1d4);
  *(undefined4 *)(param_2 + 0x18) = *(undefined4 *)(param_1 + 0x1d0);
  *(uint *)(param_2 + 0x10) = (uint)(iVar2 == 1);
  if (iVar1 != 0) {
    AuiLogger_Init(*(undefined4 *)(param_2 + 0x38),0x1f,*(undefined4 *)(param_2 + 0x34));
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(param_4,5,param_3,0x528,PTR_s_push_PCM___size__d_600a08b4,
                 *(undefined4 *)(param_2 + 0x34));
  }
  aud_module_push_data(param_1,0,param_2);
  aud_module_sync(param_1,0);
  return 0;
}


/* @600a2644 _lipsync_state */

/* WARNING: Control flow encountered bad instruction data */

undefined1  [12] _lipsync_state(undefined4 *param_1)

{
  undefined1 auVar1 [12];
  uint *puVar2;
  undefined4 extraout_a2;
  undefined4 extraout_a3;
  undefined4 extraout_a4;
  undefined1 auVar3 [12];
  undefined1 auVar4 [12];
  undefined1 auVar5 [12];
  undefined1 auVar6 [12];
  undefined1 auVar7 [12];
  int iVar8;
  undefined *puVar9;
  undefined4 local_40;
  undefined4 uStack_3c;
  int iStack_38;
  int iStack_30;
  
  puVar2 = SUB_600a062c;
  if ((*(char *)(param_1 + 0x8c) != '\0') || (*(char *)((int)param_1 + 0x231) != '\0')) {
    if ((*SUB_600a062c & 8) != 0) {
      puVar9 = PTR_s_offset_600a08bc;
      if (*(char *)(param_1 + 0x8c) == '\0') {
        puVar9 = PTR_s_state_600a08b8;
      }
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_state_600a08c0,0x54a,
                   PTR_s__s_is_changed__600a08c4,puVar9);
    }
    *(undefined1 *)((int)param_1 + 0x231) = 0;
    *(undefined1 *)(param_1 + 0x8c) = 0;
    param_1[0x55] = 0;
    param_1[0x50] = param_1[0x79];
    if ((*(char *)((int)param_1 + 0x27d) == '\0') || (*(char *)((int)param_1 + 0x232) == '\0')) {
      param_1[0x57] = 6;
    }
    else {
      *(undefined1 *)((int)param_1 + 0x232) = 0;
      flix(0xc0b02a2,0xd001641);
    }
  }
  auVar6._4_4_ = param_1[99];
  if (auVar6._4_4_ < 100) {
    param_1[99] = param_1[99] + 1;
    auVar6._4_4_ = param_1[99];
  }
  if (auVar6._4_4_ < 0) {
    if (param_1[0x87] == 0) {
      flix(0xc0b0190,0xd402741);
      flix(0x40b0ba4,0xd3ff341);
      auVar6._4_4_ = param_1[0x56];
      flix(0xa0b0ade,0xd002741);
      iVar8 = param_1[0x53];
      param_1[0x53] = iVar8 + 1;
      if (10 < iVar8 + 1) {
        flix(0xc0b02a4,0xd001841);
        param_1[0x53] = 0;
        param_1[0x52] = 0;
        param_1[0x56] = param_1[0x71];
        flix(0xe0b02c4,0xd001241);
        auVar4._8_4_ = param_1[0x84];
        auVar4._4_4_ = param_1[0x83];
        local_40 = auVar4._8_4_;
        uStack_3c = auVar4._4_4_;
        aimc_raise_named_event(param_1[0x1a],param_1[1],PTR_s_adec_master_info_600a08d4,&local_40,8)
        ;
        flix(0x90b05d7,0xd001241);
        if ((*puVar2 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_state_600a08c0,0x6da,
                       PTR_s___s__base_line_Noti_audio_master_600a08d8,*param_1,param_1[0x84],
                       param_1[0x83]);
        }
        param_1[0x67] = 1;
        auVar4._0_4_ = 0xc;
        return auVar4;
      }
      goto LAB_600a2924;
    }
  }
  else {
    flix(0x20b0fb0,0xd400941);
    flix(0xa0b08b9,0xd000841);
    if (param_1[0x87] == 0) {
      if ((param_1[0x7a] == 1) && (param_1[0x85] == 0)) {
        iStack_38 = param_1[0x80];
        if (iStack_38 != 0x200) {
          iStack_30 = param_1[0x57];
          flix(0x90b0c98,0xd001141);
                    /* WARNING: Could not recover jumptable at 0x600a2859. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          (**(code **)(PTR_PTR_600a08d0 + iStack_30 * 4))(param_1,param_1 + 0x4f,200);
          auVar3._4_4_ = extraout_a3;
          auVar3._0_4_ = extraout_a2;
          auVar3._8_4_ = extraout_a4;
          return auVar3;
        }
      }
      else {
        flix(0x20b0bc4,0xd3fe241);
      }
      auVar6._4_4_ = param_1[0x56];
      iVar8 = param_1[0x71] - auVar6._4_4_;
      if (iVar8 <= param_1[0x7b] * 0x5a) {
        if (-1 < iVar8 + (param_1[0x7c] + param_1[0x8d]) * 0x5a) {
          param_1[0x53] = 0;
          param_1[0x52] = 0;
          auVar6._0_4_ = 0xc;
          auVar6._8_4_ = 0x200;
          return auVar6;
        }
        iVar8 = param_1[0x52];
        param_1[0x52] = iVar8 + 1;
        if (200 < iVar8 + 1) {
          flix(0x50b00f4,0xd3fee41);
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_state_600a08c0,0x6c3,
                       PTR_s___s___instead_skip__prev_diff__d_600a08dc,*param_1,auVar6._4_4_,
                       param_1[0x71]);
        }
        param_1[0x53] = 0;
        auVar7._4_4_ = auVar6._4_4_;
        auVar7._0_4_ = 0xc;
        auVar7._8_4_ = 0x200;
        return auVar7;
      }
      param_1[0x53] = param_1[0x53] + 1;
      flix(0xe0b09de,0xd3ff541);
LAB_600a2924:
      param_1[0x52] = 0;
      auVar5._4_4_ = auVar6._4_4_;
      auVar5._0_4_ = 0xc;
      auVar5._8_4_ = 0x200;
      return auVar5;
    }
  }
  flix(0x10b0180,0xd401241);
  flix(0xd0b089a,0xd001341);
  if (param_1[0x85] == 1) {
    flix(0xf0b0990,0xd401f41);
    flix(0x70b039c,0xd400e41);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  auVar1._4_4_ = param_1[0x87];
  auVar1._0_4_ = param_1 + 0x4f;
  auVar1._8_4_ = 0;
  return auVar1 << 0x20;
}


/* @600a2751 _lipsync_state__blk_600a2751 */

undefined4 _lipsync_state__blk_600a2751(int param_1,int param_2)

{
  undefined4 in_a14;
  undefined4 in_a15;
  
  if (*(int *)(param_1 + 0x18) == 0) {
    in_a14 = in_a15;
  }
  flix(0xf0b044c,0xd401c41);
  flix(0x50b04b0,0xd401c41);
  memw();
  *(undefined4 *)(param_2 + 0x90) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  return in_a14;
}


/* @600a2785 _lipsync_state__blk_600a2785 */

/* WARNING: Control flow encountered bad instruction data */

undefined1  [12]
_lipsync_state__blk_600a2785(int param_1,int param_2,int param_3,undefined4 param_4)

{
  undefined1 auVar1 [12];
  undefined1 auVar2 [12];
  
  if (*(int *)(param_2 + 0xe0) == 0) {
    flix(0xa0b0180,0xd402941);
    if (*(int *)(param_2 + 0xc4) == param_3) {
      auVar2._4_4_ = *(undefined4 *)(param_1 + 0x158);
      flix(0x80b0ade,0xd001c41);
      *(int *)(param_1 + 0x14c) = *(int *)(param_1 + 0x14c) + 1;
      flix(0x10b06a4,0xd400c41);
      *(undefined4 *)(param_1 + 0x148) = param_4;
      auVar2._0_4_ = 0xc;
      auVar2._8_4_ = param_3;
      return auVar2;
    }
  }
  else {
    param_3 = *(int *)(param_1 + 0x21c);
    flix(0x10b0180,0xd401241);
    flix(0xd0b089a,0xd001341);
  }
  if (*(int *)(param_1 + 0x214) == 1) {
    flix(0xf0b0990,0xd401f41);
    flix(0x70b039c,0xd400e41);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  auVar1._4_4_ = param_3;
  auVar1._0_4_ = param_2;
  auVar1._8_4_ = 0;
  return auVar1 << 0x20;
}


/* @600a2814 _lipsync_state__blk_600a2814 */

void _lipsync_state__blk_600a2814(int param_1)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_state_600a08c0,0x553,
               PTR_s_lipsync_state_keep_stay__prevAct_600a08cc,*(undefined4 *)(param_1 + 0x15c));
}


/* @600a2828 _lipsync_state__blk_600a2828 */

/* WARNING: Control flow encountered bad instruction data */

undefined1  [12]
_lipsync_state__blk_600a2828
          (undefined4 *param_1,int param_2,undefined4 param_3,undefined4 param_4,uint *param_5,
          uint param_6)

{
  undefined1 auVar1 [12];
  undefined4 extraout_a2;
  undefined4 extraout_a3;
  undefined4 extraout_a4;
  undefined1 auVar2 [12];
  undefined1 auVar3 [12];
  undefined1 auVar4 [12];
  undefined1 auVar5 [12];
  undefined1 auVar6 [12];
  int iVar7;
  int iVar8;
  int iVar9;
  undefined4 uStack00000010;
  undefined4 in_stack_00000014;
  int in_stack_00000018;
  int in_stack_00000020;
  
  auVar5._4_4_ = *(int *)(param_2 + 0x50);
  iVar7 = *(int *)(param_2 + 0xf8);
  iVar9 = *(int *)(param_2 + 0xb4);
  iVar8 = *(int *)(param_2 + 0xb0);
  if (auVar5._4_4_ < 100) {
    param_1[99] = param_1[99] + 1;
    auVar5._4_4_ = *(int *)(param_2 + 0x50);
  }
  if (auVar5._4_4_ < 0) {
    if (*(int *)(param_2 + 0xe0) == 0) {
      flix(0xc0b0190,0xd402741);
      flix(0x40b0ba4,0xd3ff341);
      auVar5._4_4_ = param_1[0x56];
      flix(0xa0b0ade,0xd002741);
      iVar7 = param_1[0x53];
      param_1[0x53] = iVar7 + 1;
      if (10 < iVar7 + 1) {
        flix(0xc0b02a4,0xd001841);
        param_1[0x53] = param_4;
        param_1[0x52] = param_4;
        param_1[0x56] = param_1[0x71];
        flix(0xe0b02c4,0xd001241);
        auVar3._8_4_ = param_1[0x84];
        auVar3._4_4_ = param_1[0x83];
        uStack00000010 = auVar3._8_4_;
        in_stack_00000014 = auVar3._4_4_;
        aimc_raise_named_event
                  (param_1[0x1a],param_1[1],PTR_s_adec_master_info_600a08d4,&uStack00000010,8);
        flix(0x90b05d7,0xd001241);
        if ((*param_5 & param_6) != 0) {
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_state_600a08c0,0x6da,
                       PTR_s___s__base_line_Noti_audio_master_600a08d8,*param_1);
        }
        param_1[0x67] = 1;
        auVar3._0_4_ = 0xc;
        return auVar3;
      }
      goto LAB_600a2924;
    }
  }
  else {
    flix(0x20b0fb0,0xd400941);
    flix(0xa0b08b9,0xd000841);
    if (*(int *)(param_2 + 0xe0) == 0) {
      if ((*(int *)(param_2 + 0xac) == 1) && (*(int *)(param_2 + 0xd8) == 0)) {
        in_stack_00000018 = *(int *)(param_2 + 0xc4);
        if (in_stack_00000018 != 0x200) {
          in_stack_00000020 = *(int *)(param_2 + 0x20);
          flix(0x90b0c98,0xd001141);
                    /* WARNING: Could not recover jumptable at 0x600a2859. Too many branches */
                    /* WARNING: Treating indirect jump as call */
          (**(code **)(PTR_PTR_600a08d0 + in_stack_00000020 * 4))(param_1,param_2,200);
          auVar2._4_4_ = extraout_a3;
          auVar2._0_4_ = extraout_a2;
          auVar2._8_4_ = extraout_a4;
          return auVar2;
        }
      }
      else {
        flix(0x20b0bc4,0xd3fe241);
      }
      auVar5._4_4_ = param_1[0x56];
      if (param_1[0x71] - auVar5._4_4_ <= iVar8 * 0x5a) {
        if (-1 < (param_1[0x71] - auVar5._4_4_) + (iVar9 + iVar7) * 0x5a) {
          param_1[0x53] = param_4;
          param_1[0x52] = param_4;
          auVar5._0_4_ = 0xc;
          auVar5._8_4_ = 0x200;
          return auVar5;
        }
        iVar7 = param_1[0x52];
        param_1[0x52] = iVar7 + 1;
        if (200 < iVar7 + 1) {
          flix(0x50b00f4,0xd3fee41);
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_state_600a08c0,0x6c3,
                       PTR_s___s___instead_skip__prev_diff__d_600a08dc,*param_1);
        }
        param_1[0x53] = param_4;
        auVar6._4_4_ = auVar5._4_4_;
        auVar6._0_4_ = 0xc;
        auVar6._8_4_ = 0x200;
        return auVar6;
      }
      param_1[0x53] = param_1[0x53] + 1;
      flix(0xe0b09de,0xd3ff541);
LAB_600a2924:
      param_1[0x52] = param_4;
      auVar4._4_4_ = auVar5._4_4_;
      auVar4._0_4_ = 0xc;
      auVar4._8_4_ = 0x200;
      return auVar4;
    }
  }
  flix(0x10b0180,0xd401241);
  flix(0xd0b089a,0xd001341);
  if (param_1[0x85] == 1) {
    flix(0xf0b0990,0xd401f41);
    flix(0x70b039c,0xd400e41);
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  auVar1._4_4_ = param_1[0x87];
  auVar1._0_4_ = param_2;
  auVar1._8_4_ = 0;
  return auVar1 << 0x20;
}


/* @600a282e _lipsync_state__blk_600a282e */

undefined4 _lipsync_state__blk_600a282e(void)

{
  return 10;
}


/* @600a2832 _lipsync_state__blk_600a2832 */

/* WARNING: Control flow encountered bad instruction data */

undefined4
_lipsync_state__blk_600a2832
          (undefined4 *param_1,int param_2,undefined4 param_3,undefined4 param_4,uint *param_5,
          uint param_6)

{
  undefined4 uVar1;
  undefined4 uStack00000010;
  undefined4 in_stack_00000014;
  int iStack00000018;
  int in_stack_00000020;
  
  if (*(int *)(param_2 + 0xd8) != 0) {
    flix(0xd0b089a,0xd001341);
    if (param_1[0x85] == 1) {
      flix(0xf0b0990,0xd401f41);
      flix(0x70b039c,0xd400e41);
                    /* WARNING: Bad instruction - Truncating control flow here */
      halt_baddata();
    }
    return 0;
  }
  iStack00000018 = *(int *)(param_2 + 0xc4);
  if (iStack00000018 != 0x200) {
    in_stack_00000020 = *(int *)(param_2 + 0x20);
    flix(0x90b0c98,0xd001141);
                    /* WARNING: Could not recover jumptable at 0x600a2859. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(PTR_PTR_600a08d0 + in_stack_00000020 * 4))();
    return uVar1;
  }
  flix(0x60b0ade,0xd000c41);
  param_1[0x53] = param_1[0x53] + 1;
  flix(0xa0b0a49,0xd000a41);
  flix(0xc0b02a4,0xd001841);
  param_1[0x53] = param_4;
  param_1[0x52] = param_4;
  param_1[0x56] = param_1[0x71];
  flix(0xe0b02c4,0xd001241);
  uStack00000010 = param_1[0x84];
  in_stack_00000014 = param_1[0x83];
  aimc_raise_named_event(param_1[0x1a],param_1[1],PTR_s_adec_master_info_600a08d4,&uStack00000010,8)
  ;
  flix(0x90b05d7,0xd001241);
  if ((*param_5 & param_6) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_state_600a08c0,0x6da,
                 PTR_s___s__base_line_Noti_audio_master_600a08d8,*param_1);
  }
  param_1[0x67] = 1;
  return 0xc;
}


/* @600a292b _lipsync_state__blk_600a292b */

undefined4 _lipsync_state__blk_600a292b(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  return param_3;
}


/* @600a292f _lipsync_state__blk_600a292f */

undefined4
_lipsync_state__blk_600a292f(undefined4 *param_1,undefined4 param_2,int param_3,undefined4 param_4)

{
  int in_a14;
  int iVar1;
  int in_a15;
  undefined4 uStack00000000;
  
  if (-1 < in_a14 + in_a15) {
    param_1[0x53] = param_4;
    param_1[0x52] = param_4;
    return 0xc;
  }
  iVar1 = param_1[0x52];
  param_1[0x52] = iVar1 + 1;
  if (param_3 < iVar1 + 1) {
    flix(0x50b00f4,0xd3fee41);
    uStack00000000 = param_2;
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_state_600a08c0,0x6c3,
                 PTR_s___s___instead_skip__prev_diff__d_600a08dc,*param_1);
  }
  param_1[0x53] = param_4;
  return 0xc;
}


/* @600a293e _lipsync_state__blk_600a293e */

/* WARNING: Control flow encountered bad instruction data */

void _lipsync_state__blk_600a293e(void)

{
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}


/* @600a2946 _lipsync_state__blk_600a2946 */

undefined4 _lipsync_state__blk_600a2946(undefined4 param_1,int param_2,undefined4 param_3)

{
  undefined4 unaff_a9;
  int unaff_a10;
  
  if (unaff_a10 == 0) {
    param_3 = unaff_a9;
  }
  flix(0xf0b044c,0xd401c41);
  flix(0x50b04b0,0xd401c41);
  memw();
  *(undefined4 *)(param_2 + 0x90) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  return param_3;
}


/* @600a29c0 _lipsync_state__blk_600a29c0 */

undefined4
_lipsync_state__blk_600a29c0
          (undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
          uint *param_5,uint param_6)

{
  undefined1 uStack00000010;
  undefined4 uStack00000014;
  
  param_1[0x53] = param_4;
  param_1[0x52] = param_4;
  param_1[0x56] = param_1[0x71];
  flix(0xe0b02c4,0xd001241);
  _uStack00000010 = param_1[0x84];
  uStack00000014 = param_1[0x83];
  aimc_raise_named_event(param_1[0x1a],param_1[1],PTR_s_adec_master_info_600a08d4,&uStack00000010,8)
  ;
  flix(0x90b05d7,0xd001241);
  if ((*param_5 & param_6) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_state_600a08c0,0x6da,
                 PTR_s___s__base_line_Noti_audio_master_600a08d8,*param_1);
  }
  param_1[0x67] = 1;
  return 0xc;
}


/* @600a29c6 _lipsync_state__blk_600a29c6 */

void _lipsync_state__blk_600a29c6(undefined4 *param_1)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  undefined4 uStack00000008;
  
  uStack00000000 = param_1[0x84];
  uStack00000004 = param_1[0x83];
  uStack00000008 = param_1[0x7d];
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_state_600a08c0,0x6d6,
               PTR_s___s__BaseST_0x_x__BaseCT_0x_x_of_600a08e0,*param_1);
}


/* @600a29eb _lipsync_state__blk_600a29eb */

undefined4 _lipsync_state__blk_600a29eb(undefined4 *param_1)

{
  uint *in_a6;
  uint in_a7;
  undefined1 uStack00000010;
  undefined4 uStack00000014;
  
  _uStack00000010 = param_1[0x84];
  uStack00000014 = param_1[0x83];
  aimc_raise_named_event(param_1[0x1a],param_1[1],PTR_s_adec_master_info_600a08d4,&uStack00000010,8)
  ;
  flix(0x90b05d7,0xd001241);
  if ((*in_a6 & in_a7) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_state_600a08c0,0x6da,
                 PTR_s___s__base_line_Noti_audio_master_600a08d8,*param_1);
  }
  param_1[0x67] = 1;
  return 0xc;
}


/* @600a29ee _lipsync_state__blk_600a29ee */

void _lipsync_state__blk_600a29ee(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  
  uStack00000000 = param_3;
  uStack00000004 = param_2;
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_noti_no_pcm_basetime_600a08e4,0x457,
               PTR_s___s__Send_Noti___LIPSYNC_EVT_NOP_600a08e8,*param_1);
}


/* @600a2a08 _lipsync_state__blk_600a2a08 */

undefined4 _lipsync_state__blk_600a2a08(undefined4 *param_1)

{
  uint *in_a6;
  uint in_a7;
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  
  if ((*in_a6 & in_a7) != 0) {
    uStack00000000 = param_1[0x84];
    uStack00000004 = param_1[0x83];
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_state_600a08c0,0x6da,
                 PTR_s___s__base_line_Noti_audio_master_600a08d8,*param_1);
  }
  param_1[0x67] = 1;
  return 0xc;
}


/* @600a2a10 _lipsync_state__blk_600a2a10 */

void _lipsync_state__blk_600a2a10(undefined4 *param_1,undefined4 param_2)

{
  undefined4 uStack00000000;
  
  uStack00000000 = param_2;
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_state_600a08c0,0x6b5,
               PTR_s___s___instead_repeat__prev_diff___600a08ec,*param_1);
}


/* @600a2a2a _lipsync_state__blk_600a2a2a */

undefined4
_lipsync_state__blk_600a2a2a
          (undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
          uint *param_5,uint param_6)

{
  undefined1 uStack00000010;
  undefined4 uStack00000014;
  
  param_1[0x53] = param_4;
  param_1[0x52] = param_4;
  param_1[0x56] = param_1[0x71];
  flix(0xe0b02c4,0xd001241);
  _uStack00000010 = param_1[0x84];
  uStack00000014 = param_1[0x83];
  aimc_raise_named_event(param_1[0x1a],param_1[1],PTR_s_adec_master_info_600a08d4,&uStack00000010,8)
  ;
  flix(0x90b05d7,0xd001241);
  if ((*param_5 & param_6) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_state_600a08c0,0x6da,
                 PTR_s___s__base_line_Noti_audio_master_600a08d8,*param_1);
  }
  param_1[0x67] = 1;
  return 0xc;
}


/* @600a2a60 _lipsync_state__blk_600a2a60 */

undefined8
_lipsync_state__blk_600a2a60
          (undefined4 *param_1,int param_2,undefined4 param_3,undefined4 param_4,uint *param_5,
          uint param_6)

{
  undefined4 uVar1;
  int iVar2;
  undefined8 uVar3;
  undefined4 uStack00000010;
  undefined4 in_stack_00000014;
  undefined4 in_stack_00000018;
  int in_stack_00000020;
  
  if (*(int *)(param_2 + 0xd8) == 0) {
    in_stack_00000018 = *(undefined4 *)(param_2 + 0xc4);
    flix(0x40b0894,0xd3fd841);
    in_stack_00000020 = *(int *)(param_2 + 0x20);
    flix(0x90b0c98,0xd001141);
                    /* WARNING: Could not recover jumptable at 0x600a2859. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar3 = (**(code **)(PTR_PTR_600a08d0 + in_stack_00000020 * 4))(param_1,param_2,10);
    return uVar3;
  }
  flix(0x40b0ba4,0xd3ff341);
  flix(0xa0b0ade,0xd002741);
  iVar2 = param_1[0x53];
  param_1[0x53] = iVar2 + 1;
  if (iVar2 + 1 < 0xb) {
    param_1[0x52] = param_4;
    return CONCAT44(param_1[0x56],0xc);
  }
  flix(0xc0b02a4,0xd001841);
  param_1[0x53] = param_4;
  param_1[0x52] = param_4;
  param_1[0x56] = param_1[0x71];
  flix(0xe0b02c4,0xd001241);
  uStack00000010 = param_1[0x84];
  uVar1 = param_1[0x83];
  in_stack_00000014 = uVar1;
  aimc_raise_named_event(param_1[0x1a],param_1[1],PTR_s_adec_master_info_600a08d4,&uStack00000010,8)
  ;
  flix(0x90b05d7,0xd001241);
  if ((*param_5 & param_6) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_state_600a08c0,0x6da,
                 PTR_s___s__base_line_Noti_audio_master_600a08d8,*param_1);
  }
  param_1[0x67] = 1;
  return CONCAT44(uVar1,0xc);
}


/* @600a2a7a _lipsync_state__blk_600a2a7a */

undefined4
_lipsync_state__blk_600a2a7a(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int in_a14;
  int in_a15;
  
  if (in_a14 + in_a15 < 0) {
    *(int *)(param_1 + 0x148) = *(int *)(param_1 + 0x148) + 1;
    flix(0xf0b09cd,0xd3ff041);
    *(undefined4 *)(param_1 + 0x14c) = param_4;
    return 0xc;
  }
  *(undefined4 *)(param_1 + 0x14c) = param_4;
  *(undefined4 *)(param_1 + 0x148) = param_4;
  return 0xc;
}


/* @600a2aa0 _lipsync_state__blk_600a2aa0 */

undefined4
_lipsync_state__blk_600a2aa0(int param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  if (param_1 == 0) {
    if (*(int *)(param_2 + 0x8c) != 1) {
      return 0xb;
    }
  }
  else {
    flix(0x90b01bc,0xd7fca41);
  }
  *(undefined4 *)(param_1 + 0x154) = param_4;
  return 6;
}


/* @600a2b4f _lipsync_state__blk_600a2b4f */

/* WARNING: Control flow encountered bad instruction data */

undefined4
_lipsync_state__blk_600a2b4f(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  if (param_1 == 0) {
                    /* WARNING: Bad instruction - Truncating control flow here */
    halt_baddata();
  }
  flix(0x90b01bc,0xd7fca41);
  *(undefined4 *)(param_1 + 0x154) = param_4;
  return 6;
}


/* @600a2bdf _lipsync_state__blk_600a2bdf */

undefined4
_lipsync_state__blk_600a2bdf(int param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int iVar2;
  int in_a15;
  
  if (*(int *)(param_1 + 0x1c4) + in_a15 < 0) {
    *(int *)(param_1 + 0x148) = *(int *)(param_1 + 0x148) + 1;
    flix(0xb0b0a4f,0xd000941);
    iVar1 = *(int *)(param_1 + 0x1d8) * *(int *)(param_1 + 0x1dc) * *(int *)(param_1 + 0x1e0);
    iVar2 = *(int *)(param_1 + 0x100) - *(int *)(param_1 + 0x104);
    if (iVar1 == 0) {
      iVar1 = 0x180;
    }
    if ((iVar2 * 0x5a < -(*(int *)(param_1 + 0x1c4) * iVar1)) && (iVar2 < *(int *)(param_1 + 0x160))
       ) {
      *(undefined4 *)(param_1 + 0x148) = param_4;
      flix(0x60b019c,0xd7fed41);
    }
    else {
      iVar1 = *(int *)(param_2 + 0x8c);
      *(undefined4 *)(param_1 + 0x148) = param_4;
      if (iVar1 != 1) {
        return 3;
      }
    }
  }
  else {
    *(undefined4 *)(param_1 + 0x14c) = param_4;
    *(undefined4 *)(param_1 + 0x148) = param_4;
    flix(0xd0b01fc,0xd7fd741);
  }
  *(undefined4 *)(param_1 + 0x154) = param_4;
  return 6;
}


/* @600a2bfb _lipsync_state__blk_600a2bfb */

undefined4 _lipsync_state__blk_600a2bfb(int param_1,int param_2,int param_3,undefined4 param_4)

{
  int in_a15;
  int iVar1;
  
  if (*(int *)(param_1 + 0x1c4) + in_a15 < 0) {
    *(undefined4 *)(param_1 + 0x14c) = param_4;
    iVar1 = *(int *)(param_1 + 0x150) + 1;
    *(int *)(param_1 + 0x150) = iVar1;
    flix(0xa0b02a5,0xd000841);
    if (param_3 < iVar1) {
      iVar1 = *(int *)(param_2 + 0x8c);
      *(undefined4 *)(param_1 + 0x148) = param_4;
      if (iVar1 != 1) {
        return 3;
      }
    }
    else {
      flix(0xd0b01ac,0xd7fe841);
    }
  }
  else {
    *(undefined4 *)(param_1 + 0x14c) = param_4;
    *(undefined4 *)(param_1 + 0x148) = param_4;
    flix(0x10b019c,0xd7fd641);
  }
  *(undefined4 *)(param_1 + 0x154) = param_4;
  return 6;
}


/* @600a2c17 _lipsync_state__blk_600a2c17 */

undefined4
_lipsync_state__blk_600a2c17(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int in_a15;
  
  if (*(int *)(param_1 + 0x1c4) + in_a15 < 0) {
    flix(0xc0b01ec,0xd7fd541);
  }
  else {
    *(undefined4 *)(param_1 + 0x14c) = param_4;
    *(undefined4 *)(param_1 + 0x148) = param_4;
    flix(0x50b01bc,0xd7fd441);
  }
  *(undefined4 *)(param_1 + 0x154) = param_4;
  return 6;
}


/* @600a2c33 _lipsync_state__blk_600a2c33 */

/* WARNING: Control flow encountered bad instruction data */

void _lipsync_state__blk_600a2c33(void)

{
  flix(0x40b01b6,0xd401e41);
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}


/* @600a2c3f _lipsync_state__blk_600a2c3f */

/* WARNING: Instruction at (ram,0x600a2c6a) overlaps instruction at (ram,0x600a2c69)
    */

void _lipsync_state__blk_600a2c3f(undefined4 *param_1)

{
  undefined4 in_a7;
  int unaff_a11;
  int in_a14;
  int iStack00000000;
  undefined4 uStack00000004;
  
  if ((param_1 == (undefined4 *)0x0) && (in_a14 != 0)) {
    in_a7 = 5;
  }
  flix(0x80b00f4,0xd3fe541);
  iStack00000000 = unaff_a11 / 0x5a - param_1[0x7d];
  uStack00000004 = in_a7;
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_state_600a08c0,0x63d,
               PTR_s___s__first_gap____d_ms__idle_>_d_600a08f0,*param_1);
}


/* @600a2c78 _lipsync_state__blk_600a2c78 */

undefined4
_lipsync_state__blk_600a2c78(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  flix(0x90b01bc,0xd7fca41);
  *(undefined4 *)(param_1 + 0x154) = param_4;
  return 6;
}


/* @600a2c7b _lipsync_state__blk_600a2c7b */

/* WARNING: Control flow encountered bad instruction data */

void _lipsync_state__blk_600a2c7b(void)

{
  memw();
                    /* WARNING: Bad instruction - Truncating control flow here */
  halt_baddata();
}


/* @600a2c8e _lipsync_state__blk_600a2c8e */

undefined4
_lipsync_state__blk_600a2c8e(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  flix(0x90b01bc,0xd7fca41);
  *(undefined4 *)(param_1 + 0x154) = param_4;
  return 6;
}


/* @600a2c96 _lipsync_state__blk_600a2c96 */

undefined4
_lipsync_state__blk_600a2c96(int param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  int unaff_a10;
  int in_a14;
  
  if ((in_a14 + unaff_a10 < 1) || (-1 < in_a14)) {
    flix(0xb01dc,0xd7fe841);
  }
  else {
    *(undefined4 *)(param_2 + 0xc) = param_4;
    if (*(int *)(param_2 + 0x8c) != 1) {
      return 3;
    }
  }
  *(undefined4 *)(param_1 + 0x154) = param_4;
  return 6;
}


/* @600a2cac _lipsync_state__blk_600a2cac */

undefined4
_lipsync_state__blk_600a2cac(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  *(undefined4 *)(param_1 + 0x148) = param_4;
  flix(0x30b01e0,0xd7fda41);
  return 7;
}


/* @600a2cbe _lipsync_state__blk_600a2cbe */

undefined4
_lipsync_state__blk_600a2cbe(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  *(undefined4 *)(param_1 + 0x148) = param_4;
  flix(0x90b01fc,0xd7fca41);
  *(undefined4 *)(param_1 + 0x154) = param_4;
  return 6;
}


/* @600a2d13 _lipsync_state__blk_600a2d13 */

undefined4 _lipsync_state__blk_600a2d13(undefined4 param_1,int param_2)

{
  *(int *)(param_2 + 0x18) = *(int *)(param_2 + 0x18) + 1;
  flix(0xb0b0a8d,0xd000c41);
  flix(0x60b0190,0xd7fee41);
  return 6;
}


/* @600a2d21 _lipsync_state__blk_600a2d21 */

undefined4
_lipsync_state__blk_600a2d21(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  flix(0x90b019c,0xd7fc441);
  *(undefined4 *)(param_1 + 0x154) = param_4;
  return 6;
}


/* @600a2d2f _lipsync_state__blk_600a2d2f */

undefined4
_lipsync_state__blk_600a2d2f(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int unaff_a10;
  
  if (*(int *)(param_1 + 0x1c4) + unaff_a10 < 0) {
    flix(0x60b01e0,0xd7fbf41);
    return 5;
  }
  *(int *)(param_1 + 0x148) = *(int *)(param_1 + 0x148) + 1;
  flix(0xf0b02c5,0xd001141);
  flix(0xb0b0a4f,0xd000b41);
  *(undefined4 *)(param_1 + 0x150) = param_4;
  flix(0x80b018c,0xd7ff541);
  *(undefined4 *)(param_1 + 0x154) = param_4;
  return 6;
}


/* @600a2d69 _lipsync_state__blk_600a2d69 */

undefined4
_lipsync_state__blk_600a2d69(int param_1,int param_2,undefined4 param_3,undefined4 param_4)

{
  *(undefined4 *)(param_1 + 0x14c) = param_4;
  flix(0x60b0190,0xd7fce41);
  flix(0x50b04b0,0xd401c41);
  memw();
  *(undefined4 *)(param_2 + 0x90) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  return 4;
}


/* @600a2e0f _lipsync_state__blk_600a2e0f */

undefined4 _lipsync_state__blk_600a2e0f(void)

{
  flix(0x30b0190,0xd7fc441);
  return 3;
}


/* @600a2e1e _lipsync_state__blk_600a2e1e */

void _lipsync_state__blk_600a2e1e(undefined4 *param_1)

{
  int unaff_a10;
  int unaff_a11;
  undefined4 in_a14;
  int iStack00000000;
  undefined4 uStack00000004;
  
  if (unaff_a11 < 0) {
    if (unaff_a11 + unaff_a10 < 0) {
      in_a14 = 5;
    }
  }
  else {
    in_a14 = 9;
  }
  flix(0x80b00f4,0xd3fe541);
  iStack00000000 = unaff_a11 / 0x5a - param_1[0x7d];
  uStack00000004 = in_a14;
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_state_600a08c0,0x63d,
               PTR_s___s__first_gap____d_ms__idle_>_d_600a08f0,*param_1);
}


/* @600a2e3f _lipsync_state__blk_600a2e3f */

undefined4
_lipsync_state__blk_600a2e3f(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  *(undefined4 *)(param_1 + 0x14c) = param_4;
  flix(0x80b01cc,0xd7fb241);
  *(undefined4 *)(param_1 + 0x154) = param_4;
  return 6;
}


/* @600a2e6b _lipsync_state__blk_600a2e6b */

void _lipsync_state__blk_600a2e6b(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_state_600a08c0,0x5d8,
               PTR_s__SKIP_SkipCnt____d_600a08fc);
}


/* @600a2e7d _lipsync_state__blk_600a2e7d */

undefined4
_lipsync_state__blk_600a2e7d(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  flix(0xb0b0a4f,0xd000b41);
  *(undefined4 *)(param_1 + 0x150) = param_4;
  flix(0x80b018c,0xd7ff541);
  *(undefined4 *)(param_1 + 0x154) = param_4;
  return 6;
}


/* @600a2e86 _lipsync_state__blk_600a2e86 */

void _lipsync_state__blk_600a2e86(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_state_600a08c0,0x65c,
               PTR_s__EXCEPT_ExceptCnt____d_600a0900);
}


/* @600a2e97 _lipsync_state__blk_600a2e97 */

undefined4 _lipsync_state__blk_600a2e97(int param_1,int param_2,int param_3,undefined4 param_4)

{
  int iVar1;
  
  if (param_3 < *(int *)(param_1 + 0x150)) {
    iVar1 = *(int *)(param_2 + 0x8c);
    *(undefined4 *)(param_1 + 0x148) = param_4;
    if (iVar1 != 1) {
      return 3;
    }
  }
  else {
    flix(0xd0b01ac,0xd7fe841);
  }
  *(undefined4 *)(param_1 + 0x154) = param_4;
  return 6;
}


/* @600a2eac _lipsync_get_pts */

uint _lipsync_get_pts(int param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  
  uVar2 = SUB_600a086c;
  uVar1 = DAT_600a035c;
  iVar3 = *(int *)(param_1 + 0x200);
  if ((iVar3 == 1) || (iVar3 == 2)) {
    return param_2 & SUB_600a086c;
  }
  if (iVar3 == 0x100) {
    uVar1 = _lipsync_get_pts_for_media(param_1,param_2);
    return uVar1 & uVar2;
  }
  if (iVar3 == 0x101) {
    return param_2 & SUB_600a086c;
  }
  if (iVar3 == 0x11) {
    uVar2 = _lipsync_get_pts_for_media(param_1,param_2);
    return uVar2 & uVar1;
  }
  if (iVar3 == 0x200) {
    return param_2 & DAT_600a035c;
  }
  uVar1 = _lipsync_get_pts_for_media(param_1,param_2);
  return uVar1 & uVar2;
}


/* @600a2f08 _lipsync_get_stc */

uint _lipsync_get_stc(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x200);
  if (iVar1 == 1) {
    memw();
    return *(uint *)(DAT_600a0904 + 0x220) & SUB_600a086c;
  }
  if (iVar1 == 2) {
    memw();
    return *(uint *)(DAT_600a0904 + 0x224) & SUB_600a086c;
  }
  if (iVar1 == 0x100) {
    memw();
    return *(uint *)(DAT_600a08c8 + 0x84) & SUB_600a086c;
  }
  if (iVar1 == 0x101) {
    memw();
    return *(uint *)(DAT_600a08c8 + 0x8c) & SUB_600a086c;
  }
  if (iVar1 == 0x11) {
    memw();
    return *(uint *)(DAT_600a08c8 + 0x84) & DAT_600a035c;
  }
  if (iVar1 == 0x200) {
    memw();
    return *(uint *)(DAT_600a08c8 + 0x84) & DAT_600a035c;
  }
  memw();
  return *(uint *)(DAT_600a08c8 + 0x84) & SUB_600a086c;
}


/* @600a2f84 _lipsync_get_maxpts */

undefined4 _lipsync_get_maxpts(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x200);
  if ((((iVar1 == 1) || (iVar1 == 2)) || (iVar1 == 0x100)) ||
     ((iVar1 == 0x101 || ((iVar1 != 0x11 && (iVar1 != 0x200)))))) {
    return SUB_600a086c;
  }
  return DAT_600a035c;
}


/* @600a2fb0 _lipsync_get_delay_seamless_mode */

undefined4 _lipsync_get_delay_seamless_mode(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 + 0x180);
  if (iVar1 < 0x41a) {
    if (iVar1 < 0x2ee) {
      if (iVar1 < 500) {
        if (iVar1 == 0xfa) {
          return DAT_600a0918;
        }
      }
      else if (iVar1 < 0x1f5) {
        return DAT_600a0910;
      }
    }
    else {
      if (iVar1 < 0x2ef) {
        return DAT_600a0920;
      }
      if ((iVar1 < 1000) && (iVar1 == 0x3b6)) {
        return DAT_600a090c;
      }
    }
  }
  else if (0x41a < iVar1) {
    if (iVar1 < 0x6d6) {
      if (iVar1 < 0x5dc) {
        if (iVar1 == 0x4e2) {
          return DAT_600a091c;
        }
      }
      else if (iVar1 < 0x5dd) {
        return DAT_600a0914;
      }
    }
    else {
      if (iVar1 < 0x6d7) {
        return 0x77f;
      }
      if (iVar1 == 2000) {
        return 0x5a0;
      }
    }
  }
  return DAT_600a0908;
}


/* @600a3040 _lipsync_core */

undefined4 _lipsync_core(undefined4 *param_1)

{
  uint *puVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  undefined4 *puVar7;
  int iVar8;
  uint uVar9;
  undefined *puVar10;
  uint uVar11;
  int iStack_50;
  int iStack_4c;
  int iStack_3c;
  
  if (param_1 == (undefined4 *)0x0) {
    __assert_failed(PTR_s__lipsync_core_600a0924,DAT_600a09c0,PTR_s__m____NULL_600a09c4);
  }
  puVar1 = SUB_600a062c;
  iStack_3c = 0;
  puVar7 = param_1 + 0x8f;
  iVar2 = aud_data_adapter_peep(param_1 + 0x3c);
  if (iVar2 == 0) {
    if ((*puVar1 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_get_timestamp_600a0928,0x3a7,
                   PTR_s_empty_input_data_600a092c);
    }
    iVar2 = SUB_600a086c;
  }
  else {
    iVar2 = *(int *)(iVar2 + 0x18);
    flix(0x90b02e5,0xd000e41);
  }
  param_1[0x74] = iVar2;
  flix(0x90b02b5,0xd000f41);
  if (param_1[0x41] != 0) {
    iVar8 = param_1[0x77] * param_1[0x76] * param_1[0x78];
    if (iVar8 != 0) {
      iStack_3c = (param_1[0x41] * 0x5a) / iVar8;
    }
    iVar2 = iStack_3c + iVar2;
    flix(0xb0b02d5,0xd000e41);
  }
  iVar6 = param_1[0x7d] * 0x5a;
  flix(0x60b08d8,0xd000f41);
  flix(0xb0b08d9,0xd000f41);
  flix(0xb01a0,0xd401041);
  uVar3 = _lipsync_get_pts(param_1,iVar2);
  iVar8 = _lipsync_get_stc(param_1);
  iVar4 = _lipsync_get_maxpts(param_1);
  flix(0x30b08bc,0xd002741);
  if (*(char *)((int)param_1 + 0x27d) == '\0') {
    param_1[0x71] = iVar6 + (uVar3 - iVar8) + -0x1734;
    flix(0x50b0afe,0xd000a41);
    iStack_50 = DAT_600a095c;
    param_1[0x71] = (iVar6 + (uVar3 - (iVar8 + 0x1734))) - iVar4;
  }
  else {
    iStack_50 = _lipsync_get_delay_seamless_mode(param_1,param_1[0x80],0x200);
    iStack_50 = iStack_50 + 0x1734;
    iVar6 = iVar6 + (uVar3 - (iVar8 + iStack_50));
    param_1[0x71] = iVar6;
    flix(0x10b09fe,0xd002741);
    flix(0xc0b0ae8,0xd002641);
    param_1[0x71] = iVar6 + iVar4;
  }
  iVar4 = DAT_600a0960;
  param_1[0x72] = 0;
  if ((iVar4 < iVar8 - param_1[0x6c]) &&
     ((((int)param_1[0x71] < param_1[0x7c] * 0x5a || (param_1[0x7b] * 0x5a < (int)param_1[0x71])) &&
      (param_1[0x72] = 1, (*puVar1 & 0x10) != 0)))) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a096c,
                 PTR_s___s__discontinuity__last_run_stc_600a0970,*param_1,param_1[0x6c],iVar8,
                 iVar8 - param_1[0x6c]);
  }
  flix(0xf0b0ad8,0xd000841);
  uVar9 = 100;
  iVar4 = 100;
  flix(0xe0b0a95,0xd000741);
  param_1[0x6e] = 100;
  if ((*puVar1 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a0964,
                 PTR_s___s__pcr_change_preSTC_0x_x__d__S_600a0968,*param_1,param_1[0x6c],
                 param_1[0x6c],iVar8,iVar8);
  }
  param_1[0x6c] = iVar8;
  if (uVar3 < (uint)param_1[0x6d]) {
    param_1[0x72] = 1;
    uVar9 = *puVar1;
    flix(0x40b02b4,0xd000b41);
    param_1[0x6e] = 0;
    iVar4 = 0;
  }
  param_1[0x6d] = uVar3;
  if (iVar4 != 0) {
    param_1[0x6e] = param_1[0x6e] + -1;
  }
  if (*(char *)(param_1 + 0x88) == '\0') {
    uVar9 = _lipsync_state(param_1,uVar9);
  }
  else {
    flix(0x10b0c89,0xd000d41);
    if (param_1[0x8b] != 0) {
      flix(0x20b02a2,0xd000a41);
    }
    memw();
    uVar9 = 4;
    param_1[0x73] = *(undefined4 *)(DAT_600a08c8 + 0x84);
  }
  flix(0x40b02c5,0xd000b41);
  uVar11 = param_1[0x57];
  if ((uVar11 != uVar9) && (param_1[0x87] == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1,iVar2,uVar3,iVar8,
                 iVar8 + iStack_50,(int)param_1[0x71] / 0x5a,
                 *(undefined4 *)(PTR_PTR_600a0984 + uVar11 * 4),uVar11,
                 *(undefined4 *)(PTR_PTR_600a0984 + uVar9 * 4),uVar9);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_1 + 0x88) == '\0') {
    uVar3 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar4 = param_1[0x8a];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_1 + 0x88) = 0;
    param_1[0x8a] = 0;
    uVar3 = (uint)(iVar4 - param_1[0x89]) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack_4c = uVar3 * 1000;
  if ((*puVar1 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*puVar1 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack_4c);
  }
  aud_mutex_lock(puVar7);
  aud_cond_wait_timeout(param_1 + 0x96,puVar7,iStack_4c);
  aud_mutex_unlock(puVar7);
  if ((*puVar1 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != uVar9) {
    _lipsync_write_lipsync_state(param_1,uVar9);
  }
  param_1[0x57] = uVar9;
  iVar4 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar4;
  if (iVar4 == iVar8) {
    iVar6 = param_1[0x6f];
    param_1[0x6f] = iVar6 + 1U;
    if (10 < iVar6 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1,iVar4,iVar8,
                   iVar8 - iVar4);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)((int)param_1 + 0x269) == '\0') {
    if (*(char *)(param_1 + 0x9f) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)((int)param_1 + 0x269) = 0;
    *(undefined1 *)((int)param_1 + 0x231) = 1;
  }
  iVar8 = param_1[0x76] * param_1[0x78] * param_1[0x9d];
  puVar10 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (uVar9 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar5 = (**(code **)(PTR_PTR_600a09b4 + uVar9 * 4))();
    return uVar5;
  }
  if (uVar9 != 4) {
    param_1[0x4e] = iVar8 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (uVar9 != 3) {
    puVar10 = (undefined *)(iVar8 + param_1[0x4f]);
    param_1[0x4f] = puVar10;
  }
  if (param_1[0x86] == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar3 = param_1[0x5a];
  if (uVar3 != 0) {
    if ((uint)param_1[0x65] < uVar3) {
      if (param_1[0x57] == uVar9) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar3;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (param_1[0x70] == 0) {
    memw();
    *(int *)(DAT_600a09b8 + 0x74) = iVar2;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar10 = DAT_600a09bc;
  }
  if (param_1[0x57] == uVar9) {
    param_1[0x57] = uVar9;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,uVar9,puVar10);
  param_1[0x57] = uVar9;
  return 0;
}


/* @600a30ad _lipsync_core__blk_600a30ad */

undefined4
_lipsync_core__blk_600a30ad
          (undefined4 *param_1,int param_2,undefined4 param_3,undefined4 param_4,uint *param_5)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  undefined *puVar10;
  int iStack00000034;
  uint in_stack_00000044;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  int in_stack_000000a4;
  
  iStack00000034 = SUB_600a086c;
  *(int *)(param_2 + 0x48) = SUB_600a086c;
  flix(0x90b02b5,0xd000f41);
  if (param_1[0x41] != 0) {
    iVar8 = param_1[0x77] * param_1[0x76] * param_1[0x78];
    if (iVar8 != 0) {
      in_stack_000000a4 = (param_1[0x41] * 0x5a) / iVar8;
    }
    iStack00000034 = in_stack_000000a4 + iStack00000034;
    flix(0xb0b02d5,0xd000e41);
  }
  iVar8 = iStack00000034;
  iVar7 = *(int *)(param_2 + 0x6c) * 0x5a;
  flix(0x60b08d8,0xd000f41);
  flix(0xb0b08d9,0xd000f41);
  flix(0xb01a0,0xd401041);
  uVar2 = _lipsync_get_pts(param_1,iStack00000034);
  iVar3 = _lipsync_get_stc(param_1);
  iVar4 = _lipsync_get_maxpts(param_1);
  flix(0x30b08bc,0xd002741);
  if (*(char *)(param_2 + 0xf5) == '\0') {
    *(uint *)(param_2 + 0x3c) = iVar7 + (uVar2 - iVar3) + -0x1734;
    flix(0x50b0afe,0xd000a41);
    param_1[0x71] = (iVar7 + (uVar2 - (iVar3 + 0x1734))) - iVar4;
  }
  else {
    iVar5 = _lipsync_get_delay_seamless_mode(param_1,param_1[0x80],0x200);
    iVar7 = iVar7 + (uVar2 - (iVar3 + iVar5 + 0x1734));
    *(int *)(param_2 + 0x3c) = iVar7;
    flix(0x10b09fe,0xd002741);
    flix(0xc0b0ae8,0xd002641);
    param_1[0x71] = iVar7 + iVar4;
  }
  iVar4 = DAT_600a0960;
  *(uint *)(param_2 + 0x40) = in_stack_00000044 >> 0x13 & 1;
  if ((iVar4 < iVar3 - *(int *)(param_2 + 0x28)) &&
     (((*(int *)(param_2 + 0x3c) < *(int *)(param_2 + 0x68) * 0x5a ||
       (*(int *)(param_2 + 100) * 0x5a < *(int *)(param_2 + 0x3c))) &&
      (*(undefined4 *)(param_2 + 0x40) = 1, (*param_5 & 0x10) != 0)))) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a096c,
                 PTR_s___s__discontinuity__last_run_stc_600a0970,*param_1);
  }
  flix(0xf0b0ad8,0xd000841);
  uVar9 = 100;
  iVar4 = 100;
  flix(0xe0b0a95,0xd000741);
  *(undefined4 *)(param_2 + 0x30) = 100;
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a0964,
                 PTR_s___s__pcr_change_preSTC_0x_x__d__S_600a0968,*param_1);
  }
  *(int *)(param_2 + 0x28) = iVar3;
  if (uVar2 < *(uint *)(param_2 + 0x2c)) {
    *(undefined4 *)(param_2 + 0x40) = 1;
    uVar9 = *param_5;
    flix(0x40b02b4,0xd000b41);
    *(undefined4 *)(param_2 + 0x30) = 0;
    iVar4 = 0;
  }
  *(uint *)(param_2 + 0x2c) = uVar2;
  cVar1 = *(char *)(param_2 + 0x98);
  if (iVar4 != 0) {
    param_1[0x6e] = param_1[0x6e] + -1;
  }
  if (cVar1 == '\0') {
    uVar2 = _lipsync_state(param_1,uVar9);
  }
  else {
    flix(0x10b0c89,0xd000d41);
    if (*(int *)(param_2 + 0xa4) != 0) {
      flix(0x20b02a2,0xd000a41);
    }
    memw();
    uVar2 = 4;
    *(undefined4 *)(param_2 + 0x44) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  }
  flix(0x40b02c5,0xd000b41);
  if ((param_1[0x57] != uVar2) && (*(int *)(param_2 + 0x94) == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar9 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar7 = param_1[0x8a];
    iVar4 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar9 = (uint)(iVar7 - iVar4) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar9 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != uVar2) {
    _lipsync_write_lipsync_state(param_1,uVar2);
  }
  param_1[0x57] = uVar2;
  iVar4 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar4;
  if (iVar4 == iVar3) {
    iVar3 = param_1[0x6f];
    param_1[0x6f] = iVar3 + 1U;
    if (10 < iVar3 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar3 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar10 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (uVar2 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar6 = (**(code **)(PTR_PTR_600a09b4 + uVar2 * 4))();
    return uVar6;
  }
  if (uVar2 != 4) {
    param_1[0x4e] = iVar3 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (uVar2 != 3) {
    puVar10 = (undefined *)(iVar3 + param_1[0x4f]);
    param_1[0x4f] = puVar10;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar9 = param_1[0x5a];
  if (uVar9 != 0) {
    if ((uint)param_1[0x65] < uVar9) {
      if (param_1[0x57] == uVar2) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar9;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(int *)(DAT_600a09b8 + 0x74) = iVar8;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar10 = DAT_600a09bc;
  }
  if (param_1[0x57] == uVar2) {
    param_1[0x57] = uVar2;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,uVar2,puVar10);
  param_1[0x57] = uVar2;
  return 0;
}


/* @600a31a2 _lipsync_core__blk_600a31a2 */

void _lipsync_core__blk_600a31a2(int param_1)

{
  undefined4 in_a7;
  int iStack00000000;
  
  iStack00000000 = *(int *)(param_1 + 0x100) - *(int *)(param_1 + 0x104);
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_get_timestamp_600a0928,0x3ac,
               PTR_s_timestamp__u__total__u_600a0930,in_a7);
}


/* @600a31c3 _lipsync_core__blk_600a31c3 */

undefined4
_lipsync_core__blk_600a31c3
          (undefined4 *param_1,int param_2,undefined4 param_3,undefined4 param_4,uint *param_5,
          int param_6)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  undefined *puVar10;
  int iStack00000034;
  uint in_stack_00000044;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  int in_stack_000000a4;
  
  *(int *)(param_2 + 0x48) = param_6;
  flix(0x90b02b5,0xd000f41);
  iStack00000034 = param_6;
  if (param_1[0x41] != 0) {
    iVar8 = param_1[0x77] * param_1[0x76] * param_1[0x78];
    if (iVar8 != 0) {
      in_stack_000000a4 = (param_1[0x41] * 0x5a) / iVar8;
    }
    iStack00000034 = in_stack_000000a4 + param_6;
    flix(0xb0b02d5,0xd000e41);
  }
  iVar8 = iStack00000034;
  iVar7 = *(int *)(param_2 + 0x6c) * 0x5a;
  flix(0x60b08d8,0xd000f41);
  flix(0xb0b08d9,0xd000f41);
  flix(0xb01a0,0xd401041);
  uVar2 = _lipsync_get_pts(param_1,iStack00000034);
  iVar3 = _lipsync_get_stc(param_1);
  iVar4 = _lipsync_get_maxpts(param_1);
  flix(0x30b08bc,0xd002741);
  if (*(char *)(param_2 + 0xf5) == '\0') {
    *(uint *)(param_2 + 0x3c) = iVar7 + (uVar2 - iVar3) + -0x1734;
    flix(0x50b0afe,0xd000a41);
    param_1[0x71] = (iVar7 + (uVar2 - (iVar3 + 0x1734))) - iVar4;
  }
  else {
    iVar5 = _lipsync_get_delay_seamless_mode(param_1,param_1[0x80],0x200);
    iVar7 = iVar7 + (uVar2 - (iVar3 + iVar5 + 0x1734));
    *(int *)(param_2 + 0x3c) = iVar7;
    flix(0x10b09fe,0xd002741);
    flix(0xc0b0ae8,0xd002641);
    param_1[0x71] = iVar7 + iVar4;
  }
  iVar4 = DAT_600a0960;
  *(uint *)(param_2 + 0x40) = in_stack_00000044 >> 0x13 & 1;
  if ((iVar4 < iVar3 - *(int *)(param_2 + 0x28)) &&
     (((*(int *)(param_2 + 0x3c) < *(int *)(param_2 + 0x68) * 0x5a ||
       (*(int *)(param_2 + 100) * 0x5a < *(int *)(param_2 + 0x3c))) &&
      (*(undefined4 *)(param_2 + 0x40) = 1, (*param_5 & 0x10) != 0)))) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a096c,
                 PTR_s___s__discontinuity__last_run_stc_600a0970,*param_1);
  }
  flix(0xf0b0ad8,0xd000841);
  uVar9 = 100;
  iVar4 = 100;
  flix(0xe0b0a95,0xd000741);
  *(undefined4 *)(param_2 + 0x30) = 100;
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a0964,
                 PTR_s___s__pcr_change_preSTC_0x_x__d__S_600a0968,*param_1);
  }
  *(int *)(param_2 + 0x28) = iVar3;
  if (uVar2 < *(uint *)(param_2 + 0x2c)) {
    *(undefined4 *)(param_2 + 0x40) = 1;
    uVar9 = *param_5;
    flix(0x40b02b4,0xd000b41);
    *(undefined4 *)(param_2 + 0x30) = 0;
    iVar4 = 0;
  }
  *(uint *)(param_2 + 0x2c) = uVar2;
  cVar1 = *(char *)(param_2 + 0x98);
  if (iVar4 != 0) {
    param_1[0x6e] = param_1[0x6e] + -1;
  }
  if (cVar1 == '\0') {
    uVar2 = _lipsync_state(param_1,uVar9);
  }
  else {
    flix(0x10b0c89,0xd000d41);
    if (*(int *)(param_2 + 0xa4) != 0) {
      flix(0x20b02a2,0xd000a41);
    }
    memw();
    uVar2 = 4;
    *(undefined4 *)(param_2 + 0x44) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  }
  flix(0x40b02c5,0xd000b41);
  if ((param_1[0x57] != uVar2) && (*(int *)(param_2 + 0x94) == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar9 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar7 = param_1[0x8a];
    iVar4 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar9 = (uint)(iVar7 - iVar4) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar9 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != uVar2) {
    _lipsync_write_lipsync_state(param_1,uVar2);
  }
  param_1[0x57] = uVar2;
  iVar4 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar4;
  if (iVar4 == iVar3) {
    iVar3 = param_1[0x6f];
    param_1[0x6f] = iVar3 + 1U;
    if (10 < iVar3 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar3 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar10 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (uVar2 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar6 = (**(code **)(PTR_PTR_600a09b4 + uVar2 * 4))();
    return uVar6;
  }
  if (uVar2 != 4) {
    param_1[0x4e] = iVar3 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (uVar2 != 3) {
    puVar10 = (undefined *)(iVar3 + param_1[0x4f]);
    param_1[0x4f] = puVar10;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar9 = param_1[0x5a];
  if (uVar9 != 0) {
    if ((uint)param_1[0x65] < uVar9) {
      if (param_1[0x57] == uVar2) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar9;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(int *)(DAT_600a09b8 + 0x74) = iVar8;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar10 = DAT_600a09bc;
  }
  if (param_1[0x57] == uVar2) {
    param_1[0x57] = uVar2;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,uVar2,puVar10);
  param_1[0x57] = uVar2;
  return 0;
}


/* @600a31c6 _lipsync_core__blk_600a31c6 */

void _lipsync_core__blk_600a31c6(int param_1,undefined4 param_2,undefined4 param_3,int param_4)

{
  int iStack00000000;
  
  iStack00000000 = *(int *)(param_1 + 0x100) - param_4;
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_get_offset_600a0934,0x3b4,
               PTR_s_offset_of_first_AU__d__total__u_600a0938,param_4);
}


/* @600a31e4 _lipsync_core__blk_600a31e4 */

undefined4
_lipsync_core__blk_600a31e4
          (undefined4 *param_1,int param_2,int param_3,int param_4,uint *param_5,int param_6)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  uint uVar8;
  undefined *puVar9;
  uint in_stack_00000044;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  int in_stack_000000a4;
  
  if (param_4 != 0) {
    iVar7 = param_1[0x77] * param_1[0x76] * param_1[0x78];
    if (iVar7 != 0) {
      in_stack_000000a4 = (param_4 * 0x5a) / iVar7;
    }
    param_3 = in_stack_000000a4 + param_6;
    flix(0xb0b02d5,0xd000e41);
  }
  iVar6 = *(int *)(param_2 + 0x6c) * 0x5a;
  flix(0x60b08d8,0xd000f41);
  flix(0xb0b08d9,0xd000f41);
  flix(0xb01a0,0xd401041);
  uVar2 = _lipsync_get_pts(param_1,param_3);
  iVar7 = _lipsync_get_stc(param_1);
  iVar3 = _lipsync_get_maxpts(param_1);
  flix(0x30b08bc,0xd002741);
  if (*(char *)(param_2 + 0xf5) == '\0') {
    *(uint *)(param_2 + 0x3c) = iVar6 + (uVar2 - iVar7) + -0x1734;
    flix(0x50b0afe,0xd000a41);
    param_1[0x71] = (iVar6 + (uVar2 - (iVar7 + 0x1734))) - iVar3;
  }
  else {
    iVar4 = _lipsync_get_delay_seamless_mode(param_1,param_1[0x80],0x200);
    iVar6 = iVar6 + (uVar2 - (iVar7 + iVar4 + 0x1734));
    *(int *)(param_2 + 0x3c) = iVar6;
    flix(0x10b09fe,0xd002741);
    flix(0xc0b0ae8,0xd002641);
    param_1[0x71] = iVar6 + iVar3;
  }
  iVar3 = DAT_600a0960;
  *(uint *)(param_2 + 0x40) = in_stack_00000044 >> 0x13 & 1;
  if ((iVar3 < iVar7 - *(int *)(param_2 + 0x28)) &&
     (((*(int *)(param_2 + 0x3c) < *(int *)(param_2 + 0x68) * 0x5a ||
       (*(int *)(param_2 + 100) * 0x5a < *(int *)(param_2 + 0x3c))) &&
      (*(undefined4 *)(param_2 + 0x40) = 1, (*param_5 & 0x10) != 0)))) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a096c,
                 PTR_s___s__discontinuity__last_run_stc_600a0970,*param_1);
  }
  flix(0xf0b0ad8,0xd000841);
  uVar8 = 100;
  iVar3 = 100;
  flix(0xe0b0a95,0xd000741);
  *(undefined4 *)(param_2 + 0x30) = 100;
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a0964,
                 PTR_s___s__pcr_change_preSTC_0x_x__d__S_600a0968,*param_1);
  }
  *(int *)(param_2 + 0x28) = iVar7;
  if (uVar2 < *(uint *)(param_2 + 0x2c)) {
    *(undefined4 *)(param_2 + 0x40) = 1;
    uVar8 = *param_5;
    flix(0x40b02b4,0xd000b41);
    *(undefined4 *)(param_2 + 0x30) = 0;
    iVar3 = 0;
  }
  *(uint *)(param_2 + 0x2c) = uVar2;
  cVar1 = *(char *)(param_2 + 0x98);
  if (iVar3 != 0) {
    param_1[0x6e] = param_1[0x6e] + -1;
  }
  if (cVar1 == '\0') {
    uVar2 = _lipsync_state(param_1,uVar8);
  }
  else {
    flix(0x10b0c89,0xd000d41);
    if (*(int *)(param_2 + 0xa4) != 0) {
      flix(0x20b02a2,0xd000a41);
    }
    memw();
    uVar2 = 4;
    *(undefined4 *)(param_2 + 0x44) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  }
  flix(0x40b02c5,0xd000b41);
  if ((param_1[0x57] != uVar2) && (*(int *)(param_2 + 0x94) == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar8 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar6 = param_1[0x8a];
    iVar3 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar8 = (uint)(iVar6 - iVar3) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar8 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != uVar2) {
    _lipsync_write_lipsync_state(param_1,uVar2);
  }
  param_1[0x57] = uVar2;
  iVar3 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar3;
  if (iVar3 == iVar7) {
    iVar7 = param_1[0x6f];
    param_1[0x6f] = iVar7 + 1U;
    if (10 < iVar7 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar7 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar9 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (uVar2 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar5 = (**(code **)(PTR_PTR_600a09b4 + uVar2 * 4))();
    return uVar5;
  }
  if (uVar2 != 4) {
    param_1[0x4e] = iVar7 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (uVar2 != 3) {
    puVar9 = (undefined *)(iVar7 + param_1[0x4f]);
    param_1[0x4f] = puVar9;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar8 = param_1[0x5a];
  if (uVar8 != 0) {
    if ((uint)param_1[0x65] < uVar8) {
      if (param_1[0x57] == uVar2) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar8;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(int *)(DAT_600a09b8 + 0x74) = param_3;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar9 = DAT_600a09bc;
  }
  if (param_1[0x57] == uVar2) {
    param_1[0x57] = uVar2;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,uVar2,puVar9);
  param_1[0x57] = uVar2;
  return 0;
}


/* @600a31e7 _lipsync_core__blk_600a31e7 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_core__blk_600a31e7
               (int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  
  uStack00000000 = *(undefined4 *)(param_1 + 0x1d0);
  uStack00000004 = param_3;
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,_LAB_600a093c,_LAB_600a093e_2,param_4
              );
}


/* @600a3204 _lipsync_core__blk_600a3204 */

undefined4
_lipsync_core__blk_600a3204
          (undefined4 *param_1,int param_2,undefined4 param_3,undefined4 param_4,uint *param_5)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  int iVar7;
  uint uVar8;
  undefined *puVar9;
  uint in_stack_00000044;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  iVar7 = *(int *)(param_2 + 0x6c) * 0x5a;
  flix(0x60b08d8,0xd000f41);
  flix(0xb0b08d9,0xd000f41);
  flix(0xb01a0,0xd401041);
  uVar2 = _lipsync_get_pts(param_1,param_3);
  iVar3 = _lipsync_get_stc(param_1);
  iVar4 = _lipsync_get_maxpts(param_1);
  flix(0x30b08bc,0xd002741);
  if (*(char *)(param_2 + 0xf5) == '\0') {
    *(uint *)(param_2 + 0x3c) = iVar7 + (uVar2 - iVar3) + -0x1734;
    flix(0x50b0afe,0xd000a41);
    param_1[0x71] = (iVar7 + (uVar2 - (iVar3 + 0x1734))) - iVar4;
  }
  else {
    iVar5 = _lipsync_get_delay_seamless_mode(param_1,param_1[0x80],0x200);
    iVar7 = iVar7 + (uVar2 - (iVar3 + iVar5 + 0x1734));
    *(int *)(param_2 + 0x3c) = iVar7;
    flix(0x10b09fe,0xd002741);
    flix(0xc0b0ae8,0xd002641);
    param_1[0x71] = iVar7 + iVar4;
  }
  iVar4 = DAT_600a0960;
  *(uint *)(param_2 + 0x40) = in_stack_00000044 >> 0x13 & 1;
  if ((iVar4 < iVar3 - *(int *)(param_2 + 0x28)) &&
     (((*(int *)(param_2 + 0x3c) < *(int *)(param_2 + 0x68) * 0x5a ||
       (*(int *)(param_2 + 100) * 0x5a < *(int *)(param_2 + 0x3c))) &&
      (*(undefined4 *)(param_2 + 0x40) = 1, (*param_5 & 0x10) != 0)))) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a096c,
                 PTR_s___s__discontinuity__last_run_stc_600a0970,*param_1);
  }
  flix(0xf0b0ad8,0xd000841);
  uVar8 = 100;
  iVar4 = 100;
  flix(0xe0b0a95,0xd000741);
  *(undefined4 *)(param_2 + 0x30) = 100;
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a0964,
                 PTR_s___s__pcr_change_preSTC_0x_x__d__S_600a0968,*param_1);
  }
  *(int *)(param_2 + 0x28) = iVar3;
  if (uVar2 < *(uint *)(param_2 + 0x2c)) {
    *(undefined4 *)(param_2 + 0x40) = 1;
    uVar8 = *param_5;
    flix(0x40b02b4,0xd000b41);
    *(undefined4 *)(param_2 + 0x30) = 0;
    iVar4 = 0;
  }
  *(uint *)(param_2 + 0x2c) = uVar2;
  cVar1 = *(char *)(param_2 + 0x98);
  if (iVar4 != 0) {
    param_1[0x6e] = param_1[0x6e] + -1;
  }
  if (cVar1 == '\0') {
    uVar2 = _lipsync_state(param_1,uVar8);
  }
  else {
    flix(0x10b0c89,0xd000d41);
    if (*(int *)(param_2 + 0xa4) != 0) {
      flix(0x20b02a2,0xd000a41);
    }
    memw();
    uVar2 = 4;
    *(undefined4 *)(param_2 + 0x44) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  }
  flix(0x40b02c5,0xd000b41);
  if ((param_1[0x57] != uVar2) && (*(int *)(param_2 + 0x94) == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar8 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar7 = param_1[0x8a];
    iVar4 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar8 = (uint)(iVar7 - iVar4) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar8 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != uVar2) {
    _lipsync_write_lipsync_state(param_1,uVar2);
  }
  param_1[0x57] = uVar2;
  iVar4 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar4;
  if (iVar4 == iVar3) {
    iVar3 = param_1[0x6f];
    param_1[0x6f] = iVar3 + 1U;
    if (10 < iVar3 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar3 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar9 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (uVar2 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar6 = (**(code **)(PTR_PTR_600a09b4 + uVar2 * 4))();
    return uVar6;
  }
  if (uVar2 != 4) {
    param_1[0x4e] = iVar3 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (uVar2 != 3) {
    puVar9 = (undefined *)(iVar3 + param_1[0x4f]);
    param_1[0x4f] = puVar9;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar8 = param_1[0x5a];
  if (uVar8 != 0) {
    if ((uint)param_1[0x65] < uVar8) {
      if (param_1[0x57] == uVar2) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar8;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = param_3;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar9 = DAT_600a09bc;
  }
  if (param_1[0x57] == uVar2) {
    param_1[0x57] = uVar2;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,uVar2,puVar9);
  param_1[0x57] = uVar2;
  return 0;
}


/* @600a3207 _lipsync_core__blk_600a3207 */

undefined4 _lipsync_core__blk_600a3207(int param_1,int param_2,uint param_3)

{
  uint uVar1;
  undefined1 uStack00000050;
  uint uStack00000054;
  uint uStack00000070;
  
  uVar1 = DAT_600a035c;
  flix(0x80b01ac,0xd7ff041);
  memw();
  uStack00000054 = *(uint *)(DAT_600a08c8 + 0x84);
  *(undefined4 *)(param_2 + 8) = 0;
  *(uint *)(param_2 + 0x88) = param_3;
  *(uint *)(param_2 + 0x84) = uStack00000054;
  *(uint *)(param_2 + 0x80) = param_3 & uVar1;
  *(uint *)(param_2 + 0x7c) = uStack00000054 & uVar1;
  _uStack00000050 = param_3;
  uStack00000070 = param_3;
  aimc_raise_named_event
            (*(undefined4 *)(param_1 + 0x68),*(undefined4 *)(param_1 + 4),
             PTR_s_adec_master_info_600a08d4,&uStack00000050,8);
  flix(0x20b02e2,0xd000f41);
  flix(0x10b02e4,0xd001141);
  *(undefined4 *)(param_2 + 0x80) = 0xffffffff;
  *(undefined4 *)(param_2 + 0x7c) = 0xffffffff;
  return 1;
}


/* @600a3217 _lipsync_core__blk_600a3217 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_core__blk_600a3217
          (undefined4 *param_1,int param_2,uint param_3,undefined4 param_4,uint *param_5)

{
  uint uVar1;
  undefined1 uStack00000050;
  uint uStack00000054;
  uint uStack00000070;
  
  uVar1 = DAT_600a035c;
  flix(0x30b01bc,0xd7ff041);
  memw();
  uStack00000054 = *(uint *)(DAT_600a08c8 + 0x84);
  *(undefined4 *)(param_2 + 8) = 0;
  *(uint *)(param_2 + 0x88) = param_3;
  *(uint *)(param_2 + 0x84) = uStack00000054;
  *(uint *)(param_2 + 0x80) = param_3 & uVar1;
  *(uint *)(param_2 + 0x7c) = uStack00000054 & uVar1;
  _uStack00000050 = param_3;
  uStack00000070 = param_3;
  aimc_raise_named_event(param_1[0x1a],param_1[1],PTR_s_adec_master_info_600a08d4,&uStack00000050,8)
  ;
  flix(0xd0b0589,0xd000d41);
  if ((*param_5 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,_LAB_600a09f4,_LAB_600a09f7_1,
                 *param_1);
  }
  *(undefined4 *)(param_2 + 0x14) = 1;
  return 1;
}


/* @600a3227 _lipsync_core__blk_600a3227 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_core__blk_600a3227
          (undefined4 *param_1,int param_2,undefined4 param_3,int param_4,uint *param_5)

{
  char cVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  undefined *puVar8;
  int iVar9;
  int iVar10;
  undefined8 uVar11;
  uint in_stack_00000044;
  int iStack0000008c;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  iStack0000008c = param_4;
  uVar2 = _lipsync_get_pts(param_1,param_3);
  iVar3 = _lipsync_get_stc(param_1);
  uVar11 = _lipsync_get_maxpts(param_1);
  iVar5 = (int)((ulonglong)uVar11 >> 0x20);
  iVar9 = (int)uVar11;
  iVar7 = param_1[0x82];
  flix(0x10b00c0,0xd40ab41);
  iVar10 = iVar9 >> 1;
  if (param_1[0x80] == 0x200) {
    if (*(int *)(param_2 + 0x14) == 1) {
      iVar5 = iVar3 - param_1[0x81];
      if (iVar10 < iVar5) {
        iVar5 = iVar5 - iVar9;
      }
      else if (iVar5 < -iVar9) {
        iVar5 = iVar5 + iVar9;
      }
      flix(0x30b0a9b,0xd000d41);
      if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,_LAB_600a0943_1,_LAB_600a0948,
                     *param_1);
      }
      if (*(char *)(param_2 + 0xf5) == '\0') {
        iVar7 = uVar2 - iVar3;
        iVar5 = iStack0000008c + iVar7 + -0x1734;
        param_1[0x71] = iVar5;
        if (iVar10 < iVar5) {
          iVar5 = iStack0000008c + (uVar2 - (iVar3 + 0x1734));
          goto LAB_600a3335;
        }
        if (iVar5 < -iVar9) {
          iVar5 = iStack0000008c + (uVar2 - (iVar3 + 0x1734));
          goto LAB_600a32db;
        }
      }
      else {
        iVar7 = _lipsync_get_delay_seamless_mode(param_1,iVar5,iVar7);
        iVar5 = iStack0000008c + (uVar2 - (iVar3 + iVar7 + 0x1734));
        param_1[0x71] = iVar5;
        iVar7 = iVar10;
        if (iVar10 < iVar5) {
LAB_600a3335:
          iVar5 = iVar5 - iVar9;
          param_1[0x71] = iVar5;
        }
        else if (iVar5 < -iVar9) {
LAB_600a32db:
          iVar5 = iVar5 + iVar9;
          param_1[0x71] = iVar5;
        }
      }
      param_1[0x56] = iVar5;
      param_1[0x67] = 0;
    }
    *(uint *)(param_2 + 0x80) = uVar2;
    *(int *)(param_2 + 0x7c) = iVar3;
    *(int *)(param_2 + 0x84) = iVar3;
    *(undefined4 *)(param_2 + 0x88) = param_3;
  }
  if (*(char *)(param_2 + 0xf5) == '\0') {
    *(uint *)(param_2 + 0x3c) = iStack0000008c + (uVar2 - iVar3) + -0x1734;
    flix(0x50b0afe,0xd000a41);
    param_1[0x71] = (iStack0000008c + (uVar2 - (iVar3 + 0x1734))) - iVar9;
  }
  else {
    iVar7 = _lipsync_get_delay_seamless_mode(param_1,iVar5,iVar7);
    iVar7 = iStack0000008c + (uVar2 - (iVar3 + iVar7 + 0x1734));
    *(int *)(param_2 + 0x3c) = iVar7;
    flix(0x10b09fe,0xd002741);
    flix(0xc0b0ae8,0xd002641);
    param_1[0x71] = iVar7 + iVar9;
  }
  iVar7 = DAT_600a0960;
  *(uint *)(param_2 + 0x40) = in_stack_00000044 >> 0x13 & 1;
  if ((iVar7 < iVar3 - *(int *)(param_2 + 0x28)) &&
     (((*(int *)(param_2 + 0x3c) < *(int *)(param_2 + 0x68) * 0x5a ||
       (*(int *)(param_2 + 100) * 0x5a < *(int *)(param_2 + 0x3c))) &&
      (*(undefined4 *)(param_2 + 0x40) = 1, (*param_5 & 0x10) != 0)))) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a096c,
                 PTR_s___s__discontinuity__last_run_stc_600a0970,*param_1);
  }
  flix(0xf0b0ad8,0xd000841);
  uVar6 = 100;
  iVar7 = 100;
  flix(0xe0b0a95,0xd000741);
  *(undefined4 *)(param_2 + 0x30) = 100;
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a0964,
                 PTR_s___s__pcr_change_preSTC_0x_x__d__S_600a0968,*param_1);
  }
  *(int *)(param_2 + 0x28) = iVar3;
  if (uVar2 < *(uint *)(param_2 + 0x2c)) {
    *(undefined4 *)(param_2 + 0x40) = 1;
    uVar6 = *param_5;
    flix(0x40b02b4,0xd000b41);
    *(undefined4 *)(param_2 + 0x30) = 0;
    iVar7 = 0;
  }
  *(uint *)(param_2 + 0x2c) = uVar2;
  cVar1 = *(char *)(param_2 + 0x98);
  if (iVar7 != 0) {
    param_1[0x6e] = param_1[0x6e] + -1;
  }
  if (cVar1 == '\0') {
    uVar2 = _lipsync_state(param_1,uVar6);
  }
  else {
    flix(0x10b0c89,0xd000d41);
    if (*(int *)(param_2 + 0xa4) != 0) {
      flix(0x20b02a2,0xd000a41);
    }
    memw();
    uVar2 = 4;
    *(undefined4 *)(param_2 + 0x44) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  }
  flix(0x40b02c5,0xd000b41);
  if ((param_1[0x57] != uVar2) && (*(int *)(param_2 + 0x94) == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar6 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar9 = param_1[0x8a];
    iVar7 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar6 = (uint)(iVar9 - iVar7) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar6 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != uVar2) {
    _lipsync_write_lipsync_state(param_1,uVar2);
  }
  param_1[0x57] = uVar2;
  iVar7 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar7;
  if (iVar7 == iVar3) {
    iVar7 = param_1[0x6f];
    param_1[0x6f] = iVar7 + 1U;
    if (10 < iVar7 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar7 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar8 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (uVar2 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar4 = (**(code **)(PTR_PTR_600a09b4 + uVar2 * 4))();
    return uVar4;
  }
  if (uVar2 != 4) {
    param_1[0x4e] = iVar7 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (uVar2 != 3) {
    puVar8 = (undefined *)(iVar7 + param_1[0x4f]);
    param_1[0x4f] = puVar8;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar6 = param_1[0x5a];
  if (uVar6 != 0) {
    if ((uint)param_1[0x65] < uVar6) {
      if (param_1[0x57] == uVar2) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar6;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = param_3;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar8 = DAT_600a09bc;
  }
  if (param_1[0x57] == uVar2) {
    param_1[0x57] = uVar2;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,uVar2,puVar8);
  param_1[0x57] = uVar2;
  return 0;
}


/* @600a330a _lipsync_core__blk_600a330a */

undefined4
_lipsync_core__blk_600a330a
          (undefined4 *param_1,int param_2,undefined4 param_3,int param_4,uint *param_5,uint param_6
          )

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  undefined *puVar6;
  int iVar7;
  uint in_stack_00000044;
  int in_stack_00000078;
  int in_stack_0000008c;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  int in_stack_000000a8;
  
  if (*(char *)(param_2 + 0xf5) == '\0') {
    iVar2 = in_stack_0000008c + (param_6 - param_4) + -0x1734;
    param_1[0x71] = iVar2;
    if (in_stack_00000078 < iVar2) {
      iVar2 = in_stack_0000008c + (param_6 - (param_4 + 0x1734));
      goto LAB_600a3335;
    }
    if (iVar2 < -in_stack_000000a8) {
      iVar2 = in_stack_0000008c + (param_6 - (param_4 + 0x1734));
      goto LAB_600a32db;
    }
  }
  else {
    iVar2 = _lipsync_get_delay_seamless_mode(param_1);
    iVar2 = in_stack_0000008c + (param_6 - (param_4 + iVar2 + 0x1734));
    param_1[0x71] = iVar2;
    if (in_stack_00000078 < iVar2) {
LAB_600a3335:
      iVar2 = iVar2 - in_stack_000000a8;
      param_1[0x71] = iVar2;
    }
    else if (iVar2 < -in_stack_000000a8) {
LAB_600a32db:
      iVar2 = iVar2 + in_stack_000000a8;
      param_1[0x71] = iVar2;
    }
  }
  param_1[0x56] = iVar2;
  param_1[0x67] = 0;
  *(uint *)(param_2 + 0x80) = param_6;
  *(int *)(param_2 + 0x7c) = param_4;
  *(int *)(param_2 + 0x84) = param_4;
  *(undefined4 *)(param_2 + 0x88) = param_3;
  if (*(char *)(param_2 + 0xf5) == '\0') {
    *(uint *)(param_2 + 0x3c) = in_stack_0000008c + (param_6 - param_4) + -0x1734;
    flix(0x50b0afe,0xd000a41);
    param_1[0x71] = (in_stack_0000008c + (param_6 - (param_4 + 0x1734))) - in_stack_000000a8;
  }
  else {
    iVar2 = _lipsync_get_delay_seamless_mode(param_1);
    in_stack_0000008c = in_stack_0000008c + (param_6 - (param_4 + iVar2 + 0x1734));
    *(int *)(param_2 + 0x3c) = in_stack_0000008c;
    flix(0x10b09fe,0xd002741);
    flix(0xc0b0ae8,0xd002641);
    param_1[0x71] = in_stack_0000008c + in_stack_000000a8;
  }
  iVar2 = DAT_600a0960;
  *(uint *)(param_2 + 0x40) = in_stack_00000044 >> 0x13 & 1;
  if ((iVar2 < param_4 - *(int *)(param_2 + 0x28)) &&
     (((*(int *)(param_2 + 0x3c) < *(int *)(param_2 + 0x68) * 0x5a ||
       (*(int *)(param_2 + 100) * 0x5a < *(int *)(param_2 + 0x3c))) &&
      (*(undefined4 *)(param_2 + 0x40) = 1, (*param_5 & 0x10) != 0)))) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a096c,
                 PTR_s___s__discontinuity__last_run_stc_600a0970,*param_1);
  }
  flix(0xf0b0ad8,0xd000841);
  uVar5 = 100;
  iVar2 = 100;
  flix(0xe0b0a95,0xd000741);
  *(undefined4 *)(param_2 + 0x30) = 100;
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a0964,
                 PTR_s___s__pcr_change_preSTC_0x_x__d__S_600a0968,*param_1);
  }
  *(int *)(param_2 + 0x28) = param_4;
  if (param_6 < *(uint *)(param_2 + 0x2c)) {
    *(undefined4 *)(param_2 + 0x40) = 1;
    uVar5 = *param_5;
    flix(0x40b02b4,0xd000b41);
    *(undefined4 *)(param_2 + 0x30) = 0;
    iVar2 = 0;
  }
  *(uint *)(param_2 + 0x2c) = param_6;
  cVar1 = *(char *)(param_2 + 0x98);
  if (iVar2 != 0) {
    param_1[0x6e] = param_1[0x6e] + -1;
  }
  if (cVar1 == '\0') {
    uVar5 = _lipsync_state(param_1,uVar5);
  }
  else {
    flix(0x10b0c89,0xd000d41);
    if (*(int *)(param_2 + 0xa4) != 0) {
      flix(0x20b02a2,0xd000a41);
    }
    memw();
    uVar5 = 4;
    *(undefined4 *)(param_2 + 0x44) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  }
  flix(0x40b02c5,0xd000b41);
  if ((param_1[0x57] != uVar5) && (*(int *)(param_2 + 0x94) == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar4 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar7 = param_1[0x8a];
    iVar2 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar4 = (uint)(iVar7 - iVar2) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar4 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != uVar5) {
    _lipsync_write_lipsync_state(param_1,uVar5);
  }
  param_1[0x57] = uVar5;
  iVar2 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar2;
  if (iVar2 == param_4) {
    iVar2 = param_1[0x6f];
    param_1[0x6f] = iVar2 + 1U;
    if (10 < iVar2 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar2 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar6 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (uVar5 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar3 = (**(code **)(PTR_PTR_600a09b4 + uVar5 * 4))();
    return uVar3;
  }
  if (uVar5 != 4) {
    param_1[0x4e] = iVar2 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (uVar5 != 3) {
    puVar6 = (undefined *)(iVar2 + param_1[0x4f]);
    param_1[0x4f] = puVar6;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar4 = param_1[0x5a];
  if (uVar4 != 0) {
    if ((uint)param_1[0x65] < uVar4) {
      if (param_1[0x57] == uVar5) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar4;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = param_3;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar6 = DAT_600a09bc;
  }
  if (param_1[0x57] == uVar5) {
    param_1[0x57] = uVar5;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,uVar5,puVar6);
  param_1[0x57] = uVar5;
  return 0;
}


/* @600a335d _lipsync_core__blk_600a335d */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_core__blk_600a335d(void)

{
  int iVar1;
  int unaff_a9;
  int unaff_a11;
  uint in_a14;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  iVar1 = ((unaff_a9 - unaff_a11) * 1000) / 0x5a;
  if ((in_a14 & 4) == 0) {
    aud_mutex_lock(in_stack_000000a0);
    aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iVar1);
    aud_mutex_unlock(in_stack_000000a0);
    flix(0x20b0082,0xd3fce41);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,_LAB_600a094a_2,_LAB_600a0950);
  }
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,_LAB_600a0952_2,_LAB_600a0958,iVar1);
}


/* @600a33c1 _lipsync_core__blk_600a33c1 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_core__blk_600a33c1(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,param_3);
  aud_mutex_unlock(in_stack_000000a0);
  flix(0x20b0082,0xd3fce41);
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,_LAB_600a094a_2,_LAB_600a0950);
}


/* @600a33c4 _lipsync_core__blk_600a33c4 */

undefined4
_lipsync_core__blk_600a33c4
          (undefined4 *param_1,int param_2,undefined4 param_3,int param_4,uint *param_5,uint param_6
          )

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  uint uVar4;
  uint uVar5;
  undefined *puVar6;
  int in_a13;
  int iVar7;
  uint in_stack_00000044;
  int in_stack_0000008c;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  param_1[0x82] = param_6;
  param_1[0x81] = param_4;
  param_1[0x83] = param_4;
  param_1[0x84] = param_3;
  if (*(char *)(param_2 + 0xf5) == '\0') {
    *(uint *)(param_2 + 0x3c) = in_stack_0000008c + (param_6 - param_4) + -0x1734;
    flix(0x50b0afe,0xd000a41);
    param_1[0x71] = (in_stack_0000008c + (param_6 - (param_4 + 0x1734))) - in_a13;
  }
  else {
    iVar2 = _lipsync_get_delay_seamless_mode(param_1);
    in_stack_0000008c = in_stack_0000008c + (param_6 - (param_4 + iVar2 + 0x1734));
    *(int *)(param_2 + 0x3c) = in_stack_0000008c;
    flix(0x10b09fe,0xd002741);
    flix(0xc0b0ae8,0xd002641);
    param_1[0x71] = in_stack_0000008c + in_a13;
  }
  iVar2 = DAT_600a0960;
  *(uint *)(param_2 + 0x40) = in_stack_00000044 >> 0x13 & 1;
  if ((iVar2 < param_4 - *(int *)(param_2 + 0x28)) &&
     (((*(int *)(param_2 + 0x3c) < *(int *)(param_2 + 0x68) * 0x5a ||
       (*(int *)(param_2 + 100) * 0x5a < *(int *)(param_2 + 0x3c))) &&
      (*(undefined4 *)(param_2 + 0x40) = 1, (*param_5 & 0x10) != 0)))) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a096c,
                 PTR_s___s__discontinuity__last_run_stc_600a0970,*param_1);
  }
  flix(0xf0b0ad8,0xd000841);
  uVar5 = 100;
  iVar2 = 100;
  flix(0xe0b0a95,0xd000741);
  *(undefined4 *)(param_2 + 0x30) = 100;
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a0964,
                 PTR_s___s__pcr_change_preSTC_0x_x__d__S_600a0968,*param_1);
  }
  *(int *)(param_2 + 0x28) = param_4;
  if (param_6 < *(uint *)(param_2 + 0x2c)) {
    *(undefined4 *)(param_2 + 0x40) = 1;
    uVar5 = *param_5;
    flix(0x40b02b4,0xd000b41);
    *(undefined4 *)(param_2 + 0x30) = 0;
    iVar2 = 0;
  }
  *(uint *)(param_2 + 0x2c) = param_6;
  cVar1 = *(char *)(param_2 + 0x98);
  if (iVar2 != 0) {
    param_1[0x6e] = param_1[0x6e] + -1;
  }
  if (cVar1 == '\0') {
    uVar5 = _lipsync_state(param_1,uVar5);
  }
  else {
    flix(0x10b0c89,0xd000d41);
    if (*(int *)(param_2 + 0xa4) != 0) {
      flix(0x20b02a2,0xd000a41);
    }
    memw();
    uVar5 = 4;
    *(undefined4 *)(param_2 + 0x44) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  }
  flix(0x40b02c5,0xd000b41);
  if ((param_1[0x57] != uVar5) && (*(int *)(param_2 + 0x94) == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar4 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar7 = param_1[0x8a];
    iVar2 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar4 = (uint)(iVar7 - iVar2) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar4 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != uVar5) {
    _lipsync_write_lipsync_state(param_1,uVar5);
  }
  param_1[0x57] = uVar5;
  iVar2 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar2;
  if (iVar2 == param_4) {
    iVar2 = param_1[0x6f];
    param_1[0x6f] = iVar2 + 1U;
    if (10 < iVar2 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar2 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar6 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (uVar5 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar3 = (**(code **)(PTR_PTR_600a09b4 + uVar5 * 4))();
    return uVar3;
  }
  if (uVar5 != 4) {
    param_1[0x4e] = iVar2 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (uVar5 != 3) {
    puVar6 = (undefined *)(iVar2 + param_1[0x4f]);
    param_1[0x4f] = puVar6;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar4 = param_1[0x5a];
  if (uVar4 != 0) {
    if ((uint)param_1[0x65] < uVar4) {
      if (param_1[0x57] == uVar5) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar4;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = param_3;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar6 = DAT_600a09bc;
  }
  if (param_1[0x57] == uVar5) {
    param_1[0x57] = uVar5;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,uVar5,puVar6);
  param_1[0x57] = uVar5;
  return 0;
}


/* @600a3484 _lipsync_core__blk_600a3484 */

undefined4
_lipsync_core__blk_600a3484
          (undefined4 *param_1,int param_2,undefined4 param_3,int param_4,uint *param_5,uint param_6
          )

{
  char cVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  undefined *puVar5;
  int iVar6;
  int iVar7;
  undefined4 uStack00000070;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  iVar6 = *(int *)(param_2 + 0x30);
  *(int *)(param_2 + 0x28) = param_4;
  if ((iVar6 != 0) && (param_6 < *(uint *)(param_2 + 0x2c))) {
    *(undefined4 *)(param_2 + 0x40) = 1;
    flix(0x40b02b4,0xd000b41);
    *(undefined4 *)(param_2 + 0x30) = 0;
    iVar6 = 0;
  }
  *(uint *)(param_2 + 0x2c) = param_6;
  cVar1 = *(char *)(param_2 + 0x98);
  if (iVar6 != 0) {
    param_1[0x6e] = param_1[0x6e] + -1;
  }
  uStack00000070 = param_3;
  if (cVar1 == '\0') {
    uVar3 = _lipsync_state(param_1);
  }
  else {
    flix(0x10b0c89,0xd000d41);
    if (*(int *)(param_2 + 0xa4) != 0) {
      flix(0x20b02a2,0xd000a41);
    }
    memw();
    uVar3 = 4;
    *(undefined4 *)(param_2 + 0x44) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  }
  flix(0x40b02c5,0xd000b41);
  if ((param_1[0x57] != uVar3) && (*(int *)(param_2 + 0x94) == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar4 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar7 = param_1[0x8a];
    iVar6 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar4 = (uint)(iVar7 - iVar6) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar4 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != uVar3) {
    _lipsync_write_lipsync_state(param_1,uVar3);
  }
  param_1[0x57] = uVar3;
  iVar6 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar6;
  if (iVar6 == param_4) {
    iVar6 = param_1[0x6f];
    param_1[0x6f] = iVar6 + 1U;
    if (10 < iVar6 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar6 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar5 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (uVar3 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar2 = (**(code **)(PTR_PTR_600a09b4 + uVar3 * 4))();
    return uVar2;
  }
  if (uVar3 != 4) {
    param_1[0x4e] = iVar6 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (uVar3 != 3) {
    puVar5 = (undefined *)(iVar6 + param_1[0x4f]);
    param_1[0x4f] = puVar5;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar4 = param_1[0x5a];
  if (uVar4 != 0) {
    if ((uint)param_1[0x65] < uVar4) {
      if (param_1[0x57] == uVar3) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar4;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = uStack00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar5 = DAT_600a09bc;
  }
  if (param_1[0x57] == uVar3) {
    param_1[0x57] = uVar3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,uVar3,puVar5);
  param_1[0x57] = uVar3;
  return 0;
}


/* @600a3489 _lipsync_core__blk_600a3489 */

undefined4
_lipsync_core__blk_600a3489
          (undefined4 *param_1,int param_2,undefined4 param_3,int param_4,uint *param_5,uint param_6
          )

{
  char cVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  undefined *puVar5;
  int iVar6;
  int in_a13;
  int iVar7;
  int in_a14;
  uint in_stack_00000044;
  int in_stack_0000008c;
  undefined4 uStack00000090;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  uStack00000090 = DAT_600a095c;
  if (in_a14 < -in_a13) {
    param_1[0x71] = in_stack_0000008c + (param_6 - (param_4 + 0x1734)) + in_a13;
  }
  iVar6 = DAT_600a0960;
  *(uint *)(param_2 + 0x40) = in_stack_00000044 >> 0x13 & 1;
  if ((iVar6 < param_4 - *(int *)(param_2 + 0x28)) &&
     (((*(int *)(param_2 + 0x3c) < *(int *)(param_2 + 0x68) * 0x5a ||
       (*(int *)(param_2 + 100) * 0x5a < *(int *)(param_2 + 0x3c))) &&
      (*(undefined4 *)(param_2 + 0x40) = 1, (*param_5 & 0x10) != 0)))) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a096c,
                 PTR_s___s__discontinuity__last_run_stc_600a0970,*param_1);
  }
  flix(0xf0b0ad8,0xd000841);
  uVar4 = 100;
  iVar6 = 100;
  flix(0xe0b0a95,0xd000741);
  *(undefined4 *)(param_2 + 0x30) = 100;
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a0964,
                 PTR_s___s__pcr_change_preSTC_0x_x__d__S_600a0968,*param_1);
  }
  *(int *)(param_2 + 0x28) = param_4;
  if (param_6 < *(uint *)(param_2 + 0x2c)) {
    *(undefined4 *)(param_2 + 0x40) = 1;
    uVar4 = *param_5;
    flix(0x40b02b4,0xd000b41);
    *(undefined4 *)(param_2 + 0x30) = 0;
    iVar6 = 0;
  }
  *(uint *)(param_2 + 0x2c) = param_6;
  cVar1 = *(char *)(param_2 + 0x98);
  if (iVar6 != 0) {
    param_1[0x6e] = param_1[0x6e] + -1;
  }
  if (cVar1 == '\0') {
    uVar4 = _lipsync_state(param_1,uVar4);
  }
  else {
    flix(0x10b0c89,0xd000d41);
    if (*(int *)(param_2 + 0xa4) != 0) {
      flix(0x20b02a2,0xd000a41);
    }
    memw();
    uVar4 = 4;
    *(undefined4 *)(param_2 + 0x44) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  }
  flix(0x40b02c5,0xd000b41);
  if ((param_1[0x57] != uVar4) && (*(int *)(param_2 + 0x94) == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar3 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar7 = param_1[0x8a];
    iVar6 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar3 = (uint)(iVar7 - iVar6) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar3 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != uVar4) {
    _lipsync_write_lipsync_state(param_1,uVar4);
  }
  param_1[0x57] = uVar4;
  iVar6 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar6;
  if (iVar6 == param_4) {
    iVar6 = param_1[0x6f];
    param_1[0x6f] = iVar6 + 1U;
    if (10 < iVar6 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar6 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar5 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (uVar4 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar2 = (**(code **)(PTR_PTR_600a09b4 + uVar4 * 4))();
    return uVar2;
  }
  if (uVar4 != 4) {
    param_1[0x4e] = iVar6 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (uVar4 != 3) {
    puVar5 = (undefined *)(iVar6 + param_1[0x4f]);
    param_1[0x4f] = puVar5;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar3 = param_1[0x5a];
  if (uVar3 != 0) {
    if ((uint)param_1[0x65] < uVar3) {
      if (param_1[0x57] == uVar4) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar3;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = param_3;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar5 = DAT_600a09bc;
  }
  if (param_1[0x57] == uVar4) {
    param_1[0x57] = uVar4;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,uVar4,puVar5);
  param_1[0x57] = uVar4;
  return 0;
}


/* @600a34d1 _lipsync_core__blk_600a34d1 */

undefined4
_lipsync_core__blk_600a34d1
          (undefined4 *param_1,int param_2,undefined4 param_3,int param_4,uint *param_5,uint param_6
          )

{
  char cVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  undefined *puVar5;
  int iVar6;
  int iVar7;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  flix(0xf0b0ad8,0xd000841);
  uVar4 = 100;
  iVar6 = 100;
  flix(0xe0b0a95,0xd000741);
  *(undefined4 *)(param_2 + 0x30) = 100;
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,DAT_600a0964,
                 PTR_s___s__pcr_change_preSTC_0x_x__d__S_600a0968,*param_1);
  }
  *(int *)(param_2 + 0x28) = param_4;
  if (param_6 < *(uint *)(param_2 + 0x2c)) {
    *(undefined4 *)(param_2 + 0x40) = 1;
    uVar4 = *param_5;
    flix(0x40b02b4,0xd000b41);
    *(undefined4 *)(param_2 + 0x30) = 0;
    iVar6 = 0;
  }
  *(uint *)(param_2 + 0x2c) = param_6;
  cVar1 = *(char *)(param_2 + 0x98);
  if (iVar6 != 0) {
    param_1[0x6e] = param_1[0x6e] + -1;
  }
  if (cVar1 == '\0') {
    uVar4 = _lipsync_state(param_1,uVar4);
  }
  else {
    flix(0x10b0c89,0xd000d41);
    if (*(int *)(param_2 + 0xa4) != 0) {
      flix(0x20b02a2,0xd000a41);
    }
    memw();
    uVar4 = 4;
    *(undefined4 *)(param_2 + 0x44) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  }
  flix(0x40b02c5,0xd000b41);
  if ((param_1[0x57] != uVar4) && (*(int *)(param_2 + 0x94) == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar3 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar7 = param_1[0x8a];
    iVar6 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar3 = (uint)(iVar7 - iVar6) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar3 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != uVar4) {
    _lipsync_write_lipsync_state(param_1,uVar4);
  }
  param_1[0x57] = uVar4;
  iVar6 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar6;
  if (iVar6 == param_4) {
    iVar6 = param_1[0x6f];
    param_1[0x6f] = iVar6 + 1U;
    if (10 < iVar6 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar6 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar5 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (uVar4 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar2 = (**(code **)(PTR_PTR_600a09b4 + uVar4 * 4))();
    return uVar2;
  }
  if (uVar4 != 4) {
    param_1[0x4e] = iVar6 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (uVar4 != 3) {
    puVar5 = (undefined *)(iVar6 + param_1[0x4f]);
    param_1[0x4f] = puVar5;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar3 = param_1[0x5a];
  if (uVar3 != 0) {
    if ((uint)param_1[0x65] < uVar3) {
      if (param_1[0x57] == uVar4) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar3;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = param_3;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar5 = DAT_600a09bc;
  }
  if (param_1[0x57] == uVar4) {
    param_1[0x57] = uVar4;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,uVar4,puVar5);
  param_1[0x57] = uVar4;
  return 0;
}


/* @600a34d6 _lipsync_core__blk_600a34d6 */

undefined4
_lipsync_core__blk_600a34d6
          (undefined4 *param_1,int param_2,undefined4 param_3,int param_4,uint *param_5,uint param_6
          )

{
  char cVar1;
  undefined4 uVar2;
  uint uVar3;
  uint uVar4;
  undefined *puVar5;
  int iVar6;
  int iVar7;
  undefined4 uStack00000070;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  iVar6 = *(int *)(param_2 + 0x30);
  *(int *)(param_2 + 0x28) = param_4;
  if ((iVar6 != 0) && (param_6 < *(uint *)(param_2 + 0x2c))) {
    *(undefined4 *)(param_2 + 0x40) = 1;
    flix(0x40b02b4,0xd000b41);
    *(undefined4 *)(param_2 + 0x30) = 0;
    iVar6 = 0;
  }
  *(uint *)(param_2 + 0x2c) = param_6;
  cVar1 = *(char *)(param_2 + 0x98);
  if (iVar6 != 0) {
    param_1[0x6e] = param_1[0x6e] + -1;
  }
  uStack00000070 = param_3;
  if (cVar1 == '\0') {
    uVar3 = _lipsync_state(param_1);
  }
  else {
    flix(0x10b0c89,0xd000d41);
    if (*(int *)(param_2 + 0xa4) != 0) {
      flix(0x20b02a2,0xd000a41);
    }
    memw();
    uVar3 = 4;
    *(undefined4 *)(param_2 + 0x44) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  }
  flix(0x40b02c5,0xd000b41);
  if ((param_1[0x57] != uVar3) && (*(int *)(param_2 + 0x94) == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar4 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar7 = param_1[0x8a];
    iVar6 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar4 = (uint)(iVar7 - iVar6) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar4 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != uVar3) {
    _lipsync_write_lipsync_state(param_1,uVar3);
  }
  param_1[0x57] = uVar3;
  iVar6 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar6;
  if (iVar6 == param_4) {
    iVar6 = param_1[0x6f];
    param_1[0x6f] = iVar6 + 1U;
    if (10 < iVar6 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar6 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar5 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (uVar3 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar2 = (**(code **)(PTR_PTR_600a09b4 + uVar3 * 4))();
    return uVar2;
  }
  if (uVar3 != 4) {
    param_1[0x4e] = iVar6 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (uVar3 != 3) {
    puVar5 = (undefined *)(iVar6 + param_1[0x4f]);
    param_1[0x4f] = puVar5;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar4 = param_1[0x5a];
  if (uVar4 != 0) {
    if ((uint)param_1[0x65] < uVar4) {
      if (param_1[0x57] == uVar3) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar4;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = uStack00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar5 = DAT_600a09bc;
  }
  if (param_1[0x57] == uVar3) {
    param_1[0x57] = uVar3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,uVar3,puVar5);
  param_1[0x57] = uVar3;
  return 0;
}


/* @600a35a0 _lipsync_core__blk_600a35a0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_core__blk_600a35a0(undefined4 *param_1)

{
  undefined4 in_a7;
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  undefined4 uStack00000008;
  undefined4 uStack0000000c;
  undefined4 uStack00000010;
  
  uStack00000000 = param_1[0x6d];
  uStack00000010 = param_1[0x6e];
  uStack00000004 = uStack00000000;
  uStack00000008 = in_a7;
  uStack0000000c = in_a7;
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,_LAB_600a0974,_LAB_600a0976_2,
               *param_1);
}


/* @600a35c8 _lipsync_core__blk_600a35c8 */

/* WARNING: Removing unreachable block (ram,0x600a34fd) */

undefined4
_lipsync_core__blk_600a35c8
          (undefined4 *param_1,int param_2,undefined4 param_3,int param_4,uint *param_5,
          undefined4 param_6)

{
  undefined4 uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  undefined *puVar5;
  int iVar6;
  undefined4 uStack00000070;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  *(undefined4 *)(param_2 + 0x30) = 0;
  *(undefined4 *)(param_2 + 0x2c) = param_6;
  uStack00000070 = param_3;
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar2 = _lipsync_state(param_1);
  }
  else {
    flix(0x10b0c89,0xd000d41);
    if (*(int *)(param_2 + 0xa4) != 0) {
      flix(0x20b02a2,0xd000a41);
    }
    memw();
    uVar2 = 4;
    *(undefined4 *)(param_2 + 0x44) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  }
  flix(0x40b02c5,0xd000b41);
  if ((param_1[0x57] != uVar2) && (*(int *)(param_2 + 0x94) == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar4 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar6 = param_1[0x8a];
    iVar3 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar4 = (uint)(iVar6 - iVar3) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar4 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != uVar2) {
    _lipsync_write_lipsync_state(param_1,uVar2);
  }
  param_1[0x57] = uVar2;
  iVar3 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar3;
  if (iVar3 == param_4) {
    iVar3 = param_1[0x6f];
    param_1[0x6f] = iVar3 + 1U;
    if (10 < iVar3 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar3 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar5 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (uVar2 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(PTR_PTR_600a09b4 + uVar2 * 4))();
    return uVar1;
  }
  if (uVar2 != 4) {
    param_1[0x4e] = iVar3 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (uVar2 != 3) {
    puVar5 = (undefined *)(iVar3 + param_1[0x4f]);
    param_1[0x4f] = puVar5;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar4 = param_1[0x5a];
  if (uVar4 != 0) {
    if ((uint)param_1[0x65] < uVar4) {
      if (param_1[0x57] == uVar2) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar4;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = uStack00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar5 = DAT_600a09bc;
  }
  if (param_1[0x57] == uVar2) {
    param_1[0x57] = uVar2;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,uVar2,puVar5);
  param_1[0x57] = uVar2;
  return 0;
}


/* @600a35cb _lipsync_core__blk_600a35cb */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_core__blk_600a35cb(int param_1)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,_LAB_600a097b_1,_LAB_600a0980,
               *(undefined4 *)(param_1 + 0x22c));
}


/* @600a35df _lipsync_core__blk_600a35df */

/* WARNING: Removing unreachable block (ram,0x600a3796) */
/* WARNING: Removing unreachable block (ram,0x600a3799) */
/* WARNING: Removing unreachable block (ram,0x600a37a7) */
/* WARNING: Removing unreachable block (ram,0x600a37ad) */
/* WARNING: Removing unreachable block (ram,0x600a37b0) */
/* WARNING: Removing unreachable block (ram,0x600a37b8) */
/* WARNING: Removing unreachable block (ram,0x600a3874) */
/* WARNING: Removing unreachable block (ram,0x600a37be) */
/* WARNING: Removing unreachable block (ram,0x600a37c8) */
/* WARNING: Removing unreachable block (ram,0x600a37d7) */
/* WARNING: Removing unreachable block (ram,0x600a37ce) */
/* WARNING: Removing unreachable block (ram,0x600a37da) */
/* WARNING: Removing unreachable block (ram,0x600a37e7) */
/* WARNING: Removing unreachable block (ram,0x600a3868) */
/* WARNING: Removing unreachable block (ram,0x600a37ef) */
/* WARNING: Removing unreachable block (ram,0x600a380a) */
/* WARNING: Removing unreachable block (ram,0x600a3810) */
/* WARNING: Removing unreachable block (ram,0x600a3828) */

undefined4
_lipsync_core__blk_600a35df
          (undefined4 *param_1,int param_2,undefined4 param_3,int param_4,uint *param_5)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  undefined4 uStack00000070;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  memw();
  *(undefined4 *)(param_2 + 0x44) = *(undefined4 *)(DAT_600a08c8 + 0x84);
  flix(0x40b02c5,0xd000b41);
  uStack00000070 = param_3;
  if ((param_1[0x57] != 4) && (*(int *)(param_2 + 0x94) == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar3 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar4 = param_1[0x8a];
    iVar2 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar3 = (uint)(iVar4 - iVar2) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar3 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != 4) {
    _lipsync_write_lipsync_state(param_1,4);
  }
  param_1[0x57] = 4;
  iVar2 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar2;
  if (iVar2 == param_4) {
    iVar2 = param_1[0x6f];
    param_1[0x6f] = iVar2 + 1U;
    if (10 < iVar2 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  uVar1 = (**(code **)(PTR_PTR_600a09b4 + 0x10))();
  return uVar1;
}


/* @600a35f5 _lipsync_core__blk_600a35f5 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_core__blk_600a35f5(int param_1,undefined4 param_2,int param_3)

{
  undefined4 uStack00000000;
  
  uStack00000000 = *(undefined4 *)(PTR_PTR_600a0984 + param_3 * 4);
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,_LAB_600a0988,_LAB_600a098c,
               *(int *)(param_1 + 0x1c4) / 0x5a);
}


/* @600a361b _lipsync_core__blk_600a361b */

undefined4
_lipsync_core__blk_600a361b(undefined4 *param_1,int param_2,uint param_3,int param_4,uint *param_5)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  undefined *puVar4;
  int iVar5;
  undefined4 in_stack_00000070;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  if ((param_1[0x57] != param_3) && (*(int *)(param_2 + 0x94) == 0)) {
    flix(0xb00a2,0xd3ff341);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,DAT_600a0990,
                 PTR_s___s__p___10u___10u__s___10u___10_600a0994,*param_1);
  }
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar3 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar5 = param_1[0x8a];
    iVar2 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar3 = (uint)(iVar5 - iVar2) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar3 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != param_3) {
    _lipsync_write_lipsync_state(param_1,param_3);
  }
  param_1[0x57] = param_3;
  iVar2 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar2;
  if (iVar2 == param_4) {
    iVar2 = param_1[0x6f];
    param_1[0x6f] = iVar2 + 1U;
    if (10 < iVar2 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar2 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar4 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (param_3 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(PTR_PTR_600a09b4 + param_3 * 4))();
    return uVar1;
  }
  if (param_3 != 4) {
    param_1[0x4e] = iVar2 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (param_3 != 3) {
    puVar4 = (undefined *)(iVar2 + param_1[0x4f]);
    param_1[0x4f] = puVar4;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar3 = param_1[0x5a];
  if (uVar3 != 0) {
    if ((uint)param_1[0x65] < uVar3) {
      if (param_1[0x57] == param_3) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar3;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar4 = DAT_600a09bc;
  }
  if (param_1[0x57] == param_3) {
    param_1[0x57] = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3,puVar4);
  param_1[0x57] = param_3;
  return 0;
}


/* @600a3673 _lipsync_core__blk_600a3673 */

undefined4
_lipsync_core__blk_600a3673(undefined4 *param_1,int param_2,uint param_3,int param_4,uint *param_5)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  undefined *puVar4;
  int iVar5;
  undefined4 in_stack_00000070;
  int iStack00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  flix(0xc0b044c,0xd402041);
  if (*(char *)(param_2 + 0x98) == '\0') {
    uVar3 = (param_1[0x71] + 0x59) / 0x5a;
  }
  else {
    iVar5 = param_1[0x8a];
    iVar2 = param_1[0x89];
    flix(0x90b0cd8,0xd000f41);
    *(undefined1 *)(param_2 + 0x98) = 0;
    param_1[0x8a] = 0;
    uVar3 = (uint)(iVar5 - iVar2) / 0x5a;
    param_1[0x89] = 0;
  }
  iStack00000094 = uVar3 * 1000;
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a0,
                 PTR_s_start_sleep_600a09a4);
  }
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,iStack00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,iStack00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != param_3) {
    _lipsync_write_lipsync_state(param_1,param_3);
  }
  param_1[0x57] = param_3;
  iVar2 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar2;
  if (iVar2 == param_4) {
    iVar2 = param_1[0x6f];
    param_1[0x6f] = iVar2 + 1U;
    if (10 < iVar2 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar2 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar4 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (param_3 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(PTR_PTR_600a09b4 + param_3 * 4))();
    return uVar1;
  }
  if (param_3 != 4) {
    param_1[0x4e] = iVar2 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (param_3 != 3) {
    puVar4 = (undefined *)(iVar2 + param_1[0x4f]);
    param_1[0x4f] = puVar4;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar3 = param_1[0x5a];
  if (uVar3 != 0) {
    if ((uint)param_1[0x65] < uVar3) {
      if (param_1[0x57] == param_3) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar3;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar4 = DAT_600a09bc;
  }
  if (param_1[0x57] == param_3) {
    param_1[0x57] = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3,puVar4);
  param_1[0x57] = param_3;
  return 0;
}


/* @600a370a _lipsync_core__blk_600a370a */

undefined4
_lipsync_core__blk_600a370a(undefined4 *param_1,int param_2,uint param_3,int param_4,uint *param_5)

{
  int iVar1;
  undefined4 uVar2;
  undefined *puVar3;
  uint uVar4;
  undefined4 in_stack_00000070;
  undefined4 in_stack_00000094;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a0998,
                 PTR_s_sleep_time___d_600a099c,in_stack_00000094);
  }
  aud_mutex_lock(in_stack_000000a0);
  aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,in_stack_00000094);
  aud_mutex_unlock(in_stack_000000a0);
  if ((*param_5 & 0x20) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_core_600a0924,DAT_600a09a8,
                 PTR_s_end_sleep_600a09ac);
  }
  if (param_1[0x57] != param_3) {
    _lipsync_write_lipsync_state(param_1,param_3);
  }
  param_1[0x57] = param_3;
  iVar1 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar1;
  if (iVar1 == param_4) {
    iVar1 = param_1[0x6f];
    param_1[0x6f] = iVar1 + 1U;
    if (10 < iVar1 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar1 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar3 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (param_3 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar2 = (**(code **)(PTR_PTR_600a09b4 + param_3 * 4))();
    return uVar2;
  }
  if (param_3 != 4) {
    param_1[0x4e] = iVar1 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (param_3 != 3) {
    puVar3 = (undefined *)(iVar1 + param_1[0x4f]);
    param_1[0x4f] = puVar3;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar4 = param_1[0x5a];
  if (uVar4 != 0) {
    if ((uint)param_1[0x65] < uVar4) {
      if (param_1[0x57] == param_3) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar4;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar3 = DAT_600a09bc;
  }
  if (param_1[0x57] == param_3) {
    param_1[0x57] = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3,puVar3);
  param_1[0x57] = param_3;
  return 0;
}


/* @600a3727 _lipsync_core__blk_600a3727 */

undefined4 _lipsync_core__blk_600a3727(undefined4 *param_1,int param_2,uint param_3,int param_4)

{
  int iVar1;
  undefined4 uVar2;
  undefined *puVar3;
  uint uVar4;
  undefined4 in_stack_00000070;
  
  if (param_1[0x57] != param_3) {
    _lipsync_write_lipsync_state(param_1,param_3);
  }
  param_1[0x57] = param_3;
  iVar1 = _lipsync_get_stc(param_1);
  param_1[0x6c] = iVar1;
  if (iVar1 == param_4) {
    iVar1 = param_1[0x6f];
    param_1[0x6f] = iVar1 + 1U;
    if (10 < iVar1 + 1U) {
      flix(0xa0b0090,0xd002b41);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_core_600a0924,DAT_600a09e4,
                   PTR_s___s__STC_not_changed__last_run_s_600a09e8,*param_1);
    }
  }
  else {
    param_1[0x6f] = 0;
  }
  if (*(char *)(param_2 + 0xe1) == '\0') {
    if (*(char *)(param_2 + 0xf4) != '\0') {
      flix(0x50b00c3,0xd3ff241);
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,DAT_600a09c8,
                   PTR_s_breaking_the_loop__s__caused_by_f_600a09cc,
                   *(undefined4 *)(PTR_DAT_600a0634 + param_1[0x57] * 4 + 0x70));
    }
    flix(0x50b0440,0xd7f9241);
  }
  else {
    flix(0xe0b02a3,0xd002241);
    *(undefined1 *)(param_2 + 0xe1) = 0;
    *(undefined1 *)(param_2 + 0xa9) = 1;
  }
  iVar1 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar3 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (param_3 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar2 = (**(code **)(PTR_PTR_600a09b4 + param_3 * 4))();
    return uVar2;
  }
  if (param_3 != 4) {
    param_1[0x4e] = iVar1 + param_1[0x4e];
    if (0 < (int)param_1[0x50]) {
      param_1[0x50] = param_1[0x50] + -1;
    }
  }
  if (param_3 != 3) {
    puVar3 = (undefined *)(iVar1 + param_1[0x4f]);
    param_1[0x4f] = puVar3;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  uVar4 = param_1[0x5a];
  if (uVar4 != 0) {
    if ((uint)param_1[0x65] < uVar4) {
      if (param_1[0x57] == param_3) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar4;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar3 = DAT_600a09bc;
  }
  if (param_1[0x57] == param_3) {
    param_1[0x57] = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3,puVar3);
  param_1[0x57] = param_3;
  return 0;
}


/* @600a3865 _lipsync_core__blk_600a3865 */

undefined4 _lipsync_core__blk_600a3865(int param_1,int param_2,uint param_3)

{
  undefined4 uVar1;
  undefined *puVar2;
  int iVar3;
  uint uVar4;
  undefined4 in_stack_00000070;
  
  iVar3 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar2 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (param_3 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(PTR_PTR_600a09b4 + param_3 * 4))();
    return uVar1;
  }
  if (param_3 != 4) {
    *(int *)(param_1 + 0x138) = iVar3 + *(int *)(param_1 + 0x138);
    if (0 < *(int *)(param_1 + 0x140)) {
      *(int *)(param_1 + 0x140) = *(int *)(param_1 + 0x140) + -1;
    }
  }
  if (param_3 != 3) {
    puVar2 = (undefined *)(iVar3 + *(int *)(param_1 + 0x13c));
    *(undefined **)(param_1 + 0x13c) = puVar2;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
  }
  uVar4 = *(uint *)(param_1 + 0x168);
  if (uVar4 != 0) {
    if (*(uint *)(param_1 + 0x194) < uVar4) {
      if (*(uint *)(param_1 + 0x15c) == param_3) goto LAB_600a37e7;
    }
    else {
      *(uint *)(param_1 + 0x194) = *(uint *)(param_1 + 0x194) - uVar4;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar2 = DAT_600a09bc;
  }
  if (*(uint *)(param_1 + 0x15c) == param_3) {
    *(uint *)(param_1 + 0x15c) = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3,puVar2);
  *(uint *)(param_1 + 0x15c) = param_3;
  return 0;
}


/* @600a387f _lipsync_core__blk_600a387f */

undefined4 _lipsync_core__blk_600a387f(int param_1,undefined4 param_2,int param_3)

{
  int unaff_a10;
  undefined4 in_a13;
  undefined4 uStack00000068;
  undefined4 in_stack_00000070;
  
  memw();
  *(undefined4 *)(unaff_a10 + 0x78) = in_stack_00000070;
  if (*(int *)(param_1 + 0x15c) == param_3) {
    *(int *)(param_1 + 0x15c) = param_3;
    return in_a13;
  }
  uStack00000068 = in_a13;
  _lipsync_write_lipsync_state(param_1,param_3);
  *(int *)(param_1 + 0x15c) = param_3;
  return uStack00000068;
}


/* @600a38dd FUN_600a38dd */

undefined4 FUN_600a38dd(int param_1,int param_2)

{
  if (*(int *)(param_2 + 0x90) == 0) {
    *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
    return 0xffffffff;
  }
  return 0xffffffff;
}


/* @600a391e _lipsync_core__blk_600a391e */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_core__blk_600a391e
               (undefined4 *param_1,undefined4 param_2,int param_3,int param_4,undefined4 param_5,
               undefined4 param_6)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  int iStack00000008;
  int iStack0000000c;
  int iStack00000010;
  undefined4 uStack00000014;
  int iStack00000018;
  undefined4 uStack0000001c;
  int iStack00000020;
  int iStack00000024;
  undefined4 in_stack_00000070;
  int in_stack_00000090;
  
  iStack00000018 = param_1[0x57];
  iStack0000000c = param_4 + in_stack_00000090;
  uStack00000000 = in_stack_00000070;
  iStack00000024 = in_stack_00000090 / 0x5a;
  iStack00000010 = (int)param_1[0x71] / 0x5a;
  uStack00000014 = *(undefined4 *)(PTR_PTR_600a0984 + iStack00000018 * 4);
  uStack0000001c = *(undefined4 *)(PTR_PTR_600a0984 + param_3 * 4);
  uStack00000004 = param_6;
  iStack00000008 = param_4;
  iStack00000020 = param_3;
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_core_600a0924,_DAT_600a09d0,_LAB_600a09d4,*param_1)
  ;
}


/* @600a396b _lipsync_core__blk_600a396b */

undefined4 _lipsync_core__blk_600a396b(int param_1,int param_2,int param_3)

{
  undefined4 in_stack_00000068;
  undefined4 in_stack_00000070;
  
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
  }
  if (*(int *)(param_1 + 0x15c) == param_3) {
    *(int *)(param_1 + 0x15c) = param_3;
    return in_stack_00000068;
  }
  _lipsync_write_lipsync_state(param_1,param_3);
  *(int *)(param_1 + 0x15c) = param_3;
  return in_stack_00000068;
}


/* @600a3971 _lipsync_core__blk_600a3971 */

undefined4 _lipsync_core__blk_600a3971(int param_1,undefined4 param_2,int param_3)

{
  int in_a12;
  undefined4 in_a13;
  undefined4 uStack00000068;
  undefined4 in_stack_00000070;
  
  memw();
  *(undefined4 *)(in_a12 + 0x3fc) = in_stack_00000070;
  if (*(int *)(param_1 + 0x15c) == param_3) {
    *(int *)(param_1 + 0x15c) = param_3;
    return in_a13;
  }
  uStack00000068 = in_a13;
  _lipsync_write_lipsync_state(param_1,param_3);
  *(int *)(param_1 + 0x15c) = param_3;
  return uStack00000068;
}


/* @600a397d _lipsync_core__blk_600a397d */

undefined4 _lipsync_core__blk_600a397d(int param_1,undefined4 param_2,int param_3)

{
  undefined4 *unaff_a10;
  undefined4 in_a13;
  undefined4 uStack00000068;
  undefined4 in_stack_00000070;
  
  memw();
  *unaff_a10 = in_stack_00000070;
  if (*(int *)(param_1 + 0x15c) == param_3) {
    *(int *)(param_1 + 0x15c) = param_3;
    return in_a13;
  }
  uStack00000068 = in_a13;
  _lipsync_write_lipsync_state(param_1,param_3);
  *(int *)(param_1 + 0x15c) = param_3;
  return uStack00000068;
}


/* @600a39a5 _lipsync_core__blk_600a39a5 */

undefined4 _lipsync_core__blk_600a39a5(int param_1,int param_2,uint param_3)

{
  undefined4 uVar1;
  undefined *puVar2;
  int iVar3;
  uint uVar4;
  undefined4 in_stack_00000070;
  
  *(undefined1 *)(param_2 + 0xe1) = 0;
  *(undefined1 *)(param_2 + 0xa9) = 1;
  iVar3 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar2 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (param_3 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(PTR_PTR_600a09b4 + param_3 * 4))();
    return uVar1;
  }
  if (param_3 != 4) {
    *(int *)(param_1 + 0x138) = iVar3 + *(int *)(param_1 + 0x138);
    if (0 < *(int *)(param_1 + 0x140)) {
      *(int *)(param_1 + 0x140) = *(int *)(param_1 + 0x140) + -1;
    }
  }
  if (param_3 != 3) {
    puVar2 = (undefined *)(iVar3 + *(int *)(param_1 + 0x13c));
    *(undefined **)(param_1 + 0x13c) = puVar2;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
  }
  uVar4 = *(uint *)(param_1 + 0x168);
  if (uVar4 != 0) {
    if (*(uint *)(param_1 + 0x194) < uVar4) {
      if (*(uint *)(param_1 + 0x15c) == param_3) goto LAB_600a37e7;
    }
    else {
      *(uint *)(param_1 + 0x194) = *(uint *)(param_1 + 0x194) - uVar4;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar2 = DAT_600a09bc;
  }
  if (*(uint *)(param_1 + 0x15c) == param_3) {
    *(uint *)(param_1 + 0x15c) = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3,puVar2);
  *(uint *)(param_1 + 0x15c) = param_3;
  return 0;
}


/* @600a39ab FUN_600a39ab */

undefined4 FUN_600a39ab(int param_1,int param_2,uint param_3)

{
  undefined4 uVar1;
  undefined *puVar2;
  int iVar3;
  uint uVar4;
  undefined4 in_stack_00000070;
  
  *(undefined1 *)(param_2 + 0xa9) = 1;
  *(undefined4 *)(param_1 + 0x1bc) = 0;
  iVar3 = *(int *)(param_2 + 0x50) * *(int *)(param_2 + 0x58) * *(int *)(param_2 + 0xec);
  puVar2 = PTR_s__lipsync_action_600a09b0 + -0x80;
  if (param_3 < 0xe) {
                    /* WARNING: Could not recover jumptable at 0x600a3793. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    uVar1 = (**(code **)(PTR_PTR_600a09b4 + param_3 * 4))();
    return uVar1;
  }
  if (param_3 != 4) {
    *(int *)(param_1 + 0x138) = iVar3 + *(int *)(param_1 + 0x138);
    if (0 < *(int *)(param_1 + 0x140)) {
      *(int *)(param_1 + 0x140) = *(int *)(param_1 + 0x140) + -1;
    }
  }
  if (param_3 != 3) {
    puVar2 = (undefined *)(iVar3 + *(int *)(param_1 + 0x13c));
    *(undefined **)(param_1 + 0x13c) = puVar2;
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
  }
  uVar4 = *(uint *)(param_1 + 0x168);
  if (uVar4 != 0) {
    if (*(uint *)(param_1 + 0x194) < uVar4) {
      if (*(uint *)(param_1 + 0x15c) == param_3) goto LAB_600a37e7;
    }
    else {
      *(uint *)(param_1 + 0x194) = *(uint *)(param_1 + 0x194) - uVar4;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    puVar2 = DAT_600a09bc;
  }
  if (*(uint *)(param_1 + 0x15c) == param_3) {
    *(uint *)(param_1 + 0x15c) = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3,puVar2);
  *(uint *)(param_1 + 0x15c) = param_3;
  return 0;
}


/* @600a39b8 _lipsync_core__blk_600a39b8 */

int _lipsync_core__blk_600a39b8(int param_1,int param_2,int param_3)

{
  int iVar1;
  int in_a13;
  uint uVar2;
  undefined4 in_stack_00000070;
  int iStack00000088;
  undefined4 in_stack_0000009c;
  
  flix(0xc0b02f5,0xd002a41);
  iStack00000088 = in_a13;
  iVar1 = aud_data_adapter_get_data(in_stack_0000009c,in_a13);
  if (iVar1 == 0) {
    flix(0x60b0280,0xd002f41);
    if (*(int *)(param_2 + 0x90) == 0) {
      *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
      return 1;
    }
    return 1;
  }
  if (*(int *)(param_2 + 0x4c) != 1) {
    lipsync_fadeout(*(undefined4 *)(param_1 + 0x1d8),*(undefined4 *)(iVar1 + 0x38),
                    *(undefined4 *)(iVar1 + 0x38),*(undefined4 *)(iVar1 + 0x34));
  }
  _lipsync_write_metadata(param_1,*(undefined4 *)(param_2 + 0xec),0);
  iVar1 = _lipsync_write_output(param_1,iVar1);
  _lipsync_put_data(param_1,iStack00000088);
  if (iVar1 == 0) {
    if (param_3 != 4) {
      *(int *)(param_1 + 0x138) = iStack00000088 + *(int *)(param_1 + 0x138);
      if (0 < *(int *)(param_1 + 0x140)) {
        *(int *)(param_1 + 0x140) = *(int *)(param_1 + 0x140) + -1;
      }
    }
    flix(0xc0b0340,0xd7fe541);
    *(int *)(param_1 + 0x13c) = iStack00000088 + *(int *)(param_1 + 0x13c);
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
  }
  if (iVar1 != 0) {
    return iVar1;
  }
  uVar2 = *(uint *)(param_1 + 0x168);
  if (uVar2 != 0) {
    if (*(uint *)(param_1 + 0x194) < uVar2) {
      if (*(int *)(param_1 + 0x15c) == param_3) goto LAB_600a37e7;
    }
    else {
      *(uint *)(param_1 + 0x194) = *(uint *)(param_1 + 0x194) - uVar2;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
  }
  if (*(int *)(param_1 + 0x15c) == param_3) {
    *(int *)(param_1 + 0x15c) = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3);
  *(int *)(param_1 + 0x15c) = param_3;
  return 0;
}


/* @600a39f2 _lipsync_core__blk_600a39f2 */

int _lipsync_core__blk_600a39f2(int param_1,int param_2,int param_3)

{
  int iVar1;
  int in_a13;
  uint uVar2;
  undefined4 in_stack_00000070;
  int iStack00000088;
  undefined4 in_stack_00000098;
  undefined4 in_stack_0000009c;
  undefined4 in_stack_000000a0;
  
  flix(0xb0b02b5,0xd002841);
  iStack00000088 = in_a13;
  iVar1 = aud_data_adapter_get_data(in_stack_0000009c,in_a13);
  if (iVar1 == 0) {
    flix(0x50b02c0,0xd002d41);
    if (*(int *)(param_2 + 0x90) == 0) {
      *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
      return 1;
    }
    return 1;
  }
  if (*(int *)(param_2 + 0x4c) != 1) {
    lipsync_fadein(*(undefined4 *)(param_1 + 0x1d8),*(undefined4 *)(iVar1 + 0x38),
                   *(undefined4 *)(iVar1 + 0x38),*(undefined4 *)(iVar1 + 0x34));
  }
  _lipsync_write_metadata(param_1,*(undefined4 *)(param_2 + 0xec),0);
  iVar1 = _lipsync_write_output(param_1,iVar1);
  _lipsync_put_data(param_1,iStack00000088);
  if (*(char *)(param_2 + 0xf5) != '\0') {
    aud_mutex_lock(in_stack_000000a0);
    aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,1000);
    aud_mutex_unlock(in_stack_000000a0);
  }
  if (iVar1 == 0) {
    if (param_3 != 4) {
      *(int *)(param_1 + 0x138) = iStack00000088 + *(int *)(param_1 + 0x138);
      if (0 < *(int *)(param_1 + 0x140)) {
        *(int *)(param_1 + 0x140) = *(int *)(param_1 + 0x140) + -1;
      }
    }
    flix(0xc0b0340,0xd7fe541);
    *(int *)(param_1 + 0x13c) = iStack00000088 + *(int *)(param_1 + 0x13c);
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
  }
  if (iVar1 != 0) {
    return iVar1;
  }
  uVar2 = *(uint *)(param_1 + 0x168);
  if (uVar2 != 0) {
    if (*(uint *)(param_1 + 0x194) < uVar2) {
      if (*(int *)(param_1 + 0x15c) == param_3) goto LAB_600a37e7;
    }
    else {
      *(uint *)(param_1 + 0x194) = *(uint *)(param_1 + 0x194) - uVar2;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
  }
  if (*(int *)(param_1 + 0x15c) == param_3) {
    *(int *)(param_1 + 0x15c) = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3);
  *(int *)(param_1 + 0x15c) = param_3;
  return 0;
}


/* @600a3a2a _lipsync_core__blk_600a3a2a */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int _lipsync_core__blk_600a3a2a(undefined4 *param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  int iVar2;
  int in_a13;
  uint uVar3;
  undefined4 uStack00000058;
  undefined4 uStack0000005c;
  undefined4 *in_stack_00000060;
  undefined4 in_stack_00000064;
  int in_stack_00000068;
  undefined4 in_stack_0000006c;
  undefined4 in_stack_00000070;
  int in_stack_0000007c;
  int iStack00000088;
  undefined4 in_stack_0000009c;
  
  flix(0xb0b02f5,0xd002641);
  iStack00000088 = in_a13;
  in_stack_0000007c = aud_data_adapter_get_data(in_stack_0000009c,in_a13);
  if (in_stack_0000007c == 0) {
    flix(0xa0b0280,0xd002641);
    if (*(int *)(param_2 + 0x90) == 0) {
      param_1[0x65] = param_1[0x65] + 1;
      return 1;
    }
    return 1;
  }
  _lipsync_write_metadata(param_1,*(undefined4 *)(param_2 + 0xec),0);
  _lipsync_write_output(param_1,in_stack_0000007c);
  in_stack_00000068 = _lipsync_put_data(param_1,iStack00000088);
  iVar2 = *(int *)(param_2 + 0x10);
  if (iVar2 == 0) {
    uStack00000058 = param_1[0x74];
    in_stack_00000060 = &uStack00000058;
    uStack0000005c = 1;
    in_stack_00000064 = 4;
    in_stack_0000006c = uStack00000058;
    uVar1 = IMC_GetLocalImc(0);
    IMC_RaiseEvent(uVar1,_LAB_600a09fa_2,param_1[0x43],&uStack0000005c);
    flix(0x70b0094,0xd3fdd41);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,_DAT_600a0a00,0x468,lgsrcSinctable_makeTable__blk_600a0a01,
                 *param_1);
  }
  *(undefined4 *)(param_2 + 0x10) = 1;
  if (in_stack_00000068 == 0) {
    if (param_3 != 4) {
      param_1[0x4e] = iStack00000088 + param_1[0x4e];
      if (0 < (int)param_1[0x50]) {
        iVar2 = param_1[0x50] + -1;
        param_1[0x50] = iVar2;
      }
    }
    flix(0xc0b0340,0xd7fe541);
    param_1[0x4f] = iStack00000088 + param_1[0x4f];
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  if (in_stack_00000068 != 0) {
    return in_stack_00000068;
  }
  uVar3 = param_1[0x5a];
  if (uVar3 != 0) {
    if ((uint)param_1[0x65] < uVar3) {
      if (param_1[0x57] == param_3) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar3;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    iVar2 = DAT_600a09bc;
  }
  if (param_1[0x57] == param_3) {
    param_1[0x57] = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3,iVar2);
  param_1[0x57] = param_3;
  return in_stack_00000068;
}


/* @600a3b0b _lipsync_core__blk_600a3b0b */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_core__blk_600a3b0b(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,1);
}


/* @600a3b23 _lipsync_core__blk_600a3b23 */

undefined4 _lipsync_core__blk_600a3b23(int param_1,int param_2,int param_3)

{
  uint uVar1;
  undefined4 in_stack_00000070;
  int in_stack_00000088;
  
  _lipsync_write_metadata(param_1);
  _lipsync_put_data(param_1,in_stack_00000088);
  if (param_3 != 4) {
    *(int *)(param_1 + 0x138) = in_stack_00000088 + *(int *)(param_1 + 0x138);
    if (0 < *(int *)(param_1 + 0x140)) {
      *(int *)(param_1 + 0x140) = *(int *)(param_1 + 0x140) + -1;
    }
  }
  if (param_3 != 3) {
    *(int *)(param_1 + 0x13c) = in_stack_00000088 + *(int *)(param_1 + 0x13c);
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
  }
  uVar1 = *(uint *)(param_1 + 0x168);
  if (uVar1 != 0) {
    if (*(uint *)(param_1 + 0x194) < uVar1) {
      if (*(int *)(param_1 + 0x15c) == param_3) goto LAB_600a37e7;
    }
    else {
      *(uint *)(param_1 + 0x194) = *(uint *)(param_1 + 0x194) - uVar1;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
  }
  if (*(int *)(param_1 + 0x15c) == param_3) {
    *(int *)(param_1 + 0x15c) = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3);
  *(int *)(param_1 + 0x15c) = param_3;
  return 0;
}


/* @600a3b69 _lipsync_core__blk_600a3b69 */

undefined4 _lipsync_core__blk_600a3b69(int param_1,int param_2,int param_3)

{
  uint uVar1;
  undefined4 in_stack_00000070;
  int in_stack_00000088;
  undefined4 in_stack_00000098;
  undefined4 in_stack_000000a0;
  
  _lipsync_write_metadata(param_1);
  _lipsync_put_data(param_1,in_stack_00000088);
  if (*(char *)(param_2 + 0xf5) != '\0') {
    aud_mutex_lock(in_stack_000000a0);
    aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,1000);
    aud_mutex_unlock(in_stack_000000a0);
  }
  if (param_3 != 4) {
    *(int *)(param_1 + 0x138) = in_stack_00000088 + *(int *)(param_1 + 0x138);
    if (0 < *(int *)(param_1 + 0x140)) {
      *(int *)(param_1 + 0x140) = *(int *)(param_1 + 0x140) + -1;
    }
  }
  if (param_3 != 3) {
    *(int *)(param_1 + 0x13c) = in_stack_00000088 + *(int *)(param_1 + 0x13c);
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
  }
  uVar1 = *(uint *)(param_1 + 0x168);
  if (uVar1 != 0) {
    if (*(uint *)(param_1 + 0x194) < uVar1) {
      if (*(int *)(param_1 + 0x15c) == param_3) goto LAB_600a37e7;
    }
    else {
      *(uint *)(param_1 + 0x194) = *(uint *)(param_1 + 0x194) - uVar1;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
  }
  if (*(int *)(param_1 + 0x15c) == param_3) {
    *(int *)(param_1 + 0x15c) = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3);
  *(int *)(param_1 + 0x15c) = param_3;
  return 0;
}


/* @600a3c6d _lipsync_core__blk_600a3c6d */

void _lipsync_core__blk_600a3c6d(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,5);
}


/* @600a3c80 _lipsync_core__blk_600a3c80 */

int _lipsync_core__blk_600a3c80(int param_1,int param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  undefined4 in_stack_00000070;
  int in_stack_00000088;
  undefined4 in_stack_0000009c;
  
  iVar1 = aud_data_adapter_get_data(in_stack_0000009c,in_stack_00000088);
  if (iVar1 == 0) {
    flix(0x60b0280,0xd002f41);
    if (*(int *)(param_2 + 0x90) == 0) {
      *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
      return 1;
    }
    return 1;
  }
  if (*(int *)(param_2 + 0x4c) != 1) {
    lipsync_fadeout(*(undefined4 *)(param_1 + 0x1d8),*(undefined4 *)(iVar1 + 0x38),
                    *(undefined4 *)(iVar1 + 0x38),*(undefined4 *)(iVar1 + 0x34));
  }
  _lipsync_write_metadata(param_1,*(undefined4 *)(param_2 + 0xec),0);
  iVar1 = _lipsync_write_output(param_1,iVar1);
  _lipsync_put_data(param_1,in_stack_00000088);
  if (iVar1 == 0) {
    if (param_3 != 4) {
      *(int *)(param_1 + 0x138) = in_stack_00000088 + *(int *)(param_1 + 0x138);
      if (0 < *(int *)(param_1 + 0x140)) {
        *(int *)(param_1 + 0x140) = *(int *)(param_1 + 0x140) + -1;
      }
    }
    flix(0xc0b0340,0xd7fe541);
    *(int *)(param_1 + 0x13c) = in_stack_00000088 + *(int *)(param_1 + 0x13c);
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
  }
  if (iVar1 != 0) {
    return iVar1;
  }
  uVar2 = *(uint *)(param_1 + 0x168);
  if (uVar2 != 0) {
    if (*(uint *)(param_1 + 0x194) < uVar2) {
      if (*(int *)(param_1 + 0x15c) == param_3) goto LAB_600a37e7;
    }
    else {
      *(uint *)(param_1 + 0x194) = *(uint *)(param_1 + 0x194) - uVar2;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
  }
  if (*(int *)(param_1 + 0x15c) == param_3) {
    *(int *)(param_1 + 0x15c) = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3);
  *(int *)(param_1 + 0x15c) = param_3;
  return 0;
}


/* @600a3c86 _lipsync_core__blk_600a3c86 */

void _lipsync_core__blk_600a3c86(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,5);
}


/* @600a3c99 _lipsync_core__blk_600a3c99 */

int _lipsync_core__blk_600a3c99(int param_1,int param_2,int param_3)

{
  int iVar1;
  uint uVar2;
  undefined4 in_stack_00000070;
  int in_stack_00000088;
  undefined4 in_stack_00000098;
  undefined4 in_stack_0000009c;
  undefined4 in_stack_000000a0;
  
  iVar1 = aud_data_adapter_get_data(in_stack_0000009c,in_stack_00000088);
  if (iVar1 == 0) {
    flix(0x50b02c0,0xd002d41);
    if (*(int *)(param_2 + 0x90) == 0) {
      *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
      return 1;
    }
    return 1;
  }
  if (*(int *)(param_2 + 0x4c) != 1) {
    lipsync_fadein(*(undefined4 *)(param_1 + 0x1d8),*(undefined4 *)(iVar1 + 0x38),
                   *(undefined4 *)(iVar1 + 0x38),*(undefined4 *)(iVar1 + 0x34));
  }
  _lipsync_write_metadata(param_1,*(undefined4 *)(param_2 + 0xec),0);
  iVar1 = _lipsync_write_output(param_1,iVar1);
  _lipsync_put_data(param_1,in_stack_00000088);
  if (*(char *)(param_2 + 0xf5) != '\0') {
    aud_mutex_lock(in_stack_000000a0);
    aud_cond_wait_timeout(in_stack_00000098,in_stack_000000a0,1000);
    aud_mutex_unlock(in_stack_000000a0);
  }
  if (iVar1 == 0) {
    if (param_3 != 4) {
      *(int *)(param_1 + 0x138) = in_stack_00000088 + *(int *)(param_1 + 0x138);
      if (0 < *(int *)(param_1 + 0x140)) {
        *(int *)(param_1 + 0x140) = *(int *)(param_1 + 0x140) + -1;
      }
    }
    flix(0xc0b0340,0xd7fe541);
    *(int *)(param_1 + 0x13c) = in_stack_00000088 + *(int *)(param_1 + 0x13c);
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
  }
  if (iVar1 != 0) {
    return iVar1;
  }
  uVar2 = *(uint *)(param_1 + 0x168);
  if (uVar2 != 0) {
    if (*(uint *)(param_1 + 0x194) < uVar2) {
      if (*(int *)(param_1 + 0x15c) == param_3) goto LAB_600a37e7;
    }
    else {
      *(uint *)(param_1 + 0x194) = *(uint *)(param_1 + 0x194) - uVar2;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
  }
  if (*(int *)(param_1 + 0x15c) == param_3) {
    *(int *)(param_1 + 0x15c) = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3);
  *(int *)(param_1 + 0x15c) = param_3;
  return 0;
}


/* @600a3c9f _lipsync_core__blk_600a3c9f */

void _lipsync_core__blk_600a3c9f(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,5);
}


/* @600a3cb2 _lipsync_core__blk_600a3cb2 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int _lipsync_core__blk_600a3cb2(undefined4 *param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  int iVar2;
  uint uVar3;
  undefined4 uStack00000058;
  undefined4 uStack0000005c;
  undefined4 *in_stack_00000060;
  undefined4 in_stack_00000064;
  int in_stack_00000068;
  undefined4 in_stack_0000006c;
  undefined4 in_stack_00000070;
  int in_stack_0000007c;
  int in_stack_00000088;
  undefined4 in_stack_0000009c;
  
  in_stack_0000007c = aud_data_adapter_get_data(in_stack_0000009c,in_stack_00000088);
  if (in_stack_0000007c == 0) {
    flix(0xa0b0280,0xd002641);
    if (*(int *)(param_2 + 0x90) == 0) {
      param_1[0x65] = param_1[0x65] + 1;
      return 1;
    }
    return 1;
  }
  _lipsync_write_metadata(param_1,*(undefined4 *)(param_2 + 0xec),0);
  _lipsync_write_output(param_1,in_stack_0000007c);
  in_stack_00000068 = _lipsync_put_data(param_1,in_stack_00000088);
  iVar2 = *(int *)(param_2 + 0x10);
  if (iVar2 == 0) {
    uStack00000058 = param_1[0x74];
    in_stack_00000060 = &uStack00000058;
    uStack0000005c = 1;
    in_stack_00000064 = 4;
    in_stack_0000006c = uStack00000058;
    uVar1 = IMC_GetLocalImc(0);
    IMC_RaiseEvent(uVar1,_LAB_600a09fa_2,param_1[0x43],&uStack0000005c);
    flix(0x70b0094,0xd3fdd41);
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,_DAT_600a0a00,0x468,lgsrcSinctable_makeTable__blk_600a0a01,
                 *param_1);
  }
  *(undefined4 *)(param_2 + 0x10) = 1;
  if (in_stack_00000068 == 0) {
    if (param_3 != 4) {
      param_1[0x4e] = in_stack_00000088 + param_1[0x4e];
      if (0 < (int)param_1[0x50]) {
        iVar2 = param_1[0x50] + -1;
        param_1[0x50] = iVar2;
      }
    }
    flix(0xc0b0340,0xd7fe541);
    param_1[0x4f] = in_stack_00000088 + param_1[0x4f];
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    param_1[0x65] = param_1[0x65] + 1;
  }
  if (in_stack_00000068 != 0) {
    return in_stack_00000068;
  }
  uVar3 = param_1[0x5a];
  if (uVar3 != 0) {
    if ((uint)param_1[0x65] < uVar3) {
      if (param_1[0x57] == param_3) goto LAB_600a37e7;
    }
    else {
      param_1[0x65] = param_1[0x65] - uVar3;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
    iVar2 = DAT_600a09bc;
  }
  if (param_1[0x57] == param_3) {
    param_1[0x57] = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3,iVar2);
  param_1[0x57] = param_3;
  return in_stack_00000068;
}


/* @600a3cb8 _lipsync_core__blk_600a3cb8 */

void _lipsync_core__blk_600a3cb8(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_action_600a09b0,0x727,PTR_s_get_data_error_600a09f0
              );
}


/* @600a3cc9 _lipsync_core__blk_600a3cc9 */

undefined4 _lipsync_core__blk_600a3cc9(int param_1,int param_2)

{
  if (*(int *)(param_2 + 0x90) == 0) {
    *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
    return 1;
  }
  return 1;
}


/* @600a3ccf _lipsync_core__blk_600a3ccf */

void _lipsync_core__blk_600a3ccf(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_action_600a09b0,0x78f,PTR_s_get_data_error_600a09f0
              );
}


/* @600a3ce0 _lipsync_core__blk_600a3ce0 */

undefined4 _lipsync_core__blk_600a3ce0(int param_1,int param_2)

{
  if (*(int *)(param_2 + 0x90) == 0) {
    *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
    return 1;
  }
  return 1;
}


/* @600a3ce6 _lipsync_core__blk_600a3ce6 */

void _lipsync_core__blk_600a3ce6(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_action_600a09b0,0x76c,PTR_s_get_data_error_600a09f0
              );
}


/* @600a3cf7 _lipsync_core__blk_600a3cf7 */

undefined4 _lipsync_core__blk_600a3cf7(int param_1,int param_2)

{
  if (*(int *)(param_2 + 0x90) == 0) {
    *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
    return 1;
  }
  return 1;
}


/* @600a3cfd _lipsync_core__blk_600a3cfd */

undefined4 _lipsync_core__blk_600a3cfd(void)

{
  return 0xc;
}


/* @600a3e38 _lipsync_core__blk_600a3e38 */

int _lipsync_core__blk_600a3e38(int param_1,int param_2,int param_3)

{
  uint uVar1;
  int in_stack_00000068;
  undefined4 in_stack_00000070;
  int in_stack_00000088;
  
  *(undefined4 *)(param_2 + 0x10) = 1;
  if (in_stack_00000068 == 0) {
    if (param_3 != 4) {
      *(int *)(param_1 + 0x138) = in_stack_00000088 + *(int *)(param_1 + 0x138);
      if (0 < *(int *)(param_1 + 0x140)) {
        *(int *)(param_1 + 0x140) = *(int *)(param_1 + 0x140) + -1;
      }
    }
    flix(0xc0b0340,0xd7fe541);
    *(int *)(param_1 + 0x13c) = in_stack_00000088 + *(int *)(param_1 + 0x13c);
  }
  if (*(int *)(param_2 + 0x90) == 0) {
    *(int *)(param_1 + 0x194) = *(int *)(param_1 + 0x194) + 1;
  }
  if (in_stack_00000068 != 0) {
    return in_stack_00000068;
  }
  uVar1 = *(uint *)(param_1 + 0x168);
  if (uVar1 != 0) {
    if (*(uint *)(param_1 + 0x194) < uVar1) {
      if (*(int *)(param_1 + 0x15c) == param_3) goto LAB_600a37e7;
    }
    else {
      *(uint *)(param_1 + 0x194) = *(uint *)(param_1 + 0x194) - uVar1;
    }
    flix(0xb0b02a3,0xd001341);
  }
LAB_600a37e7:
  if (*(int *)(param_2 + 0x38) == 0) {
    memw();
    *(undefined4 *)(DAT_600a09b8 + 0x74) = in_stack_00000070;
  }
  else {
    flix(0xc0b0190,0xd400841);
    flix(0x30b0290,0xd401741);
    flix(0x70b0390,0xd401741);
  }
  if (*(int *)(param_1 + 0x15c) == param_3) {
    *(int *)(param_1 + 0x15c) = param_3;
    return 0;
  }
  _lipsync_write_lipsync_state(param_1,param_3);
  *(int *)(param_1 + 0x15c) = param_3;
  return 0;
}


/* @600a3e3e _lipsync_core__blk_600a3e3e */

void _lipsync_core__blk_600a3e3e(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  undefined4 in_stack_00000070;
  
  uStack00000000 = in_stack_00000070;
  uStack00000004 = param_3;
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_noti_no_pcm_basetime_600a08e4,0x457,
               PTR_s___s__Send_Noti___LIPSYNC_EVT_NOP_600a08e8,*param_1);
}


/* @600a3e5b _lipsync_core__blk_600a3e5b */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_core__blk_600a3e5b(undefined4 *param_1)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  
  flix(0x70b00e4,0xd3fef41);
  uStack00000000 = param_1[0x84];
  uStack00000004 = param_1[0x83];
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_core_600a0924,
               lgsrcSinctable_makeTable__blk_600a0a01,s___s__PcrBaseLine_Noti_nonpcr_mod_60051b34,
               *param_1);
}


/* @600a3e85 _lipsync_core__blk_600a3e85 */

undefined4 _lipsync_core__blk_600a3e85(undefined4 param_1,int param_2)

{
  *(undefined4 *)(param_2 + 0x80) = 0xffffffff;
  *(undefined4 *)(param_2 + 0x7c) = 0xffffffff;
  return 1;
}


/* @600a3e91 _lipsync_core__blk_600a3e91 */

void _lipsync_core__blk_600a3e91(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  undefined4 in_stack_00000070;
  
  uStack00000000 = in_stack_00000070;
  uStack00000004 = param_3;
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_noti_no_pcm_basetime_600a08e4,0x457,
               PTR_s___s__Send_Noti___LIPSYNC_EVT_NOP_600a08e8,*param_1);
}


/* @600a3eae _lipsync_core__blk_600a3eae */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_lipsync_core__blk_600a3eae
          (undefined4 *param_1,int param_2,undefined4 param_3,undefined4 param_4,uint *param_5)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  
  if ((*param_5 & 4) != 0) {
    uStack00000000 = param_1[0x84];
    uStack00000004 = param_1[0x83];
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_core_600a0924,_LAB_600a09f4,_LAB_600a09f7_1,
                 *param_1);
  }
  *(undefined4 *)(param_2 + 0x14) = 1;
  return 1;
}


/* @600a3eb8 _lipsync_chain */

int _lipsync_chain(int param_1,int param_2)

{
  uint *puVar1;
  int iVar2;
  undefined4 uVar3;
  undefined4 *puVar4;
  undefined *puVar5;
  undefined4 *puVar6;
  int iVar7;
  char cVar8;
  undefined *puVar9;
  char cVar10;
  uint uVar11;
  int iVar12;
  
  puVar4 = *(undefined4 **)(param_1 + 4);
  puVar6 = puVar4;
  if (*(char *)(puVar4 + 0x9a) != '\0') {
    _lipsync_flush(puVar4);
    *(undefined1 *)(puVar4 + 0x9a) = 0;
    puVar6 = *(undefined4 **)(param_1 + 4);
  }
  iVar2 = aud_module_have_active_output(puVar6);
  puVar1 = SUB_600a062c;
  if (iVar2 == 0) {
    flix(0x50b02f5,0xd001b41);
    aud_object_put(param_2);
    if (puVar4[0x40] != puVar4[0x41]) {
      aud_data_adapter_flush(puVar4 + 0x3c);
    }
  }
  else {
    if (puVar4[0x44] == 0) {
      if (puVar4[0x7a] == 0) {
        flix(0x30b00b3,0xd3fc641);
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a70,
                     PTR_s_lipsync_off_600a0a74);
      }
      flix(0x20b00b3,0xd000941);
      flix(0xd0b01b0,0xd401e41);
      flix(0xb0b02b0,0xd401e41);
      flix(0x70b08bd,0xd001e41);
      flix(0xc0b0fb0,0xd401f41);
      flix(0x70b08be,0xd001f41);
      flix(0x20b08bf,0xd001f41);
      puVar9 = PTR_s___no_delay_600a0a30;
      if (puVar4[0x85] == 0) {
        puVar9 = PTR_s__6004874c_0x14_600a0a2c;
      }
      puVar5 = PTR_s___overflow_protection_600a0a34;
      if (puVar4[0x86] == 0) {
        puVar5 = PTR_s__6004874c_0x14_600a0a2c;
      }
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a38,
                   PTR_s_lipsync_on__bound__d__d__freerun_600a0a3c,puVar4[0x7b],puVar4[0x7c],
                   puVar4[0x7e],puVar4[0x7f],puVar4[0x7d],PTR_s_UNKNOWN_600a0a28,puVar9,puVar5);
    }
    flix(0x70b02b0,0xd402a41);
    if (*(int *)(param_2 + 0x10) == 3) {
      flix(0x40b02d4,0xd003341);
      if (puVar4[0xa0] != 0) {
        flix(0xc0b02e3,0xd003341);
        aud_object_put();
      }
      uVar3 = aud_object_get(param_2);
      puVar4[0xa0] = uVar3;
      aud_object_put(param_2);
    }
    else {
      aud_data_adapter_push(puVar4 + 0x3c,param_2,puVar4[0x44],8);
      flix(0xe0b02b5,0xd002a41);
    }
    if (*(char *)((int)puVar4 + 0x27d) != '\0') {
      iVar7 = *(int *)PTR_DAT_6009fa98;
      iVar2 = 10;
      if (puVar4[0x44] == 0) {
        iVar2 = 9;
      }
      if (iVar7 == 0) {
        flix(0x90b003c,0xd7fd841);
      }
      else if (*(char *)(iVar7 + 0x32) != iVar2) {
        flix(0x90b0282,0xd002a41);
        aud_module_chain_locked(iVar7,iVar2);
      }
    }
    uVar3 = puVar4[0x41];
    uVar11 = puVar4[0x9d];
    iVar12 = puVar4[0x76];
    iVar2 = puVar4[0x78];
    iVar7 = iVar12 * uVar11;
    flix(0x80b0ebd,0xd002541);
    while( true ) {
      flix(0xd0b0eb8,0xd000a41);
      cVar10 = *(char *)((int)puVar4 + 0x231);
      if ((*(char *)(puVar4 + 0x8c) == '\0') && (cVar10 == '\0')) {
        cVar8 = '\0';
      }
      else {
        if ((*puVar1 & 8) != 0) {
          puVar9 = PTR_s_offset_600a08bc;
          if (*(char *)(puVar4 + 0x8c) == '\0') {
            puVar9 = PTR_s_state_600a08b8;
          }
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a14,
                       PTR_s__s_is_changed__600a08c4,puVar9);
        }
        uVar3 = puVar4[0x79];
        *(undefined1 *)((int)puVar4 + 0x231) = 0;
        *(undefined1 *)(puVar4 + 0x8c) = 0;
        cVar8 = *(char *)((int)puVar4 + 0x27d);
        puVar4[0x55] = 0;
        puVar4[0x50] = uVar3;
        if ((cVar8 == '\0') || (cVar10 = *(char *)((int)puVar4 + 0x232), cVar10 == '\0')) {
          puVar4[0x57] = 6;
        }
        else {
          *(undefined1 *)((int)puVar4 + 0x232) = 0;
          uVar11 = *puVar1;
          if ((uVar11 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
            logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_process_600a0a10,DAT_600a0a18,
                         PTR_s_lipsync_state_keep_stay__prevAct_600a08cc,puVar4[0x57]);
          }
        }
      }
      iVar12 = _lipsync_core(puVar4,cVar8,uVar3,cVar10,uVar11,iVar12);
      if (iVar12 != 0) goto LAB_600a405c;
      if ((puVar4[0x57] == 4) && (flix(0x60b02e1,0xd002741), *(char *)(puVar4 + 0x9f) != '\0'))
      break;
      uVar3 = puVar4[0x41];
      iVar12 = puVar4[0x76];
      uVar11 = puVar4[0x9d];
    }
    if ((*puVar1 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a8c,
                   PTR_s_flush_async__event_received__exi_600a0a90);
    }
    *(undefined1 *)(puVar4 + 0x9f) = 0;
LAB_600a405c:
    flix(0x50b0180,0xd401841);
    if ((puVar4[0x5f] != 0) && (*(char *)((int)puVar4 + 0x27d) != '\0')) {
      msf_flush_metadata(puVar4 + 0x45);
    }
    if (iVar12 != 0) {
      return iVar12;
    }
    if ((*puVar1 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a20,
                   PTR_s___s__bytes_done__used__d__total__600a0a24,*puVar4,iVar2 * iVar7,
                   puVar4[0x4e],puVar4[0x4f]);
    }
  }
  return 0;
}


/* @600a403b _lipsync_chain__blk_600a403b */

int _lipsync_chain__blk_600a403b
              (undefined4 param_1,undefined4 param_2,undefined4 *param_3,int param_4,uint *param_5,
              undefined4 param_6)

{
  char cVar1;
  int iVar2;
  undefined1 uVar3;
  undefined4 uVar4;
  undefined *puVar5;
  
  do {
    iVar2 = _lipsync_core(param_3);
    if (iVar2 != 0) {
LAB_600a405c:
      flix(0x50b0180,0xd401841);
      if ((param_3[0x5f] != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
        msf_flush_metadata(param_3 + 0x45);
      }
      if (iVar2 != 0) {
        return iVar2;
      }
      if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a20,
                     PTR_s___s__bytes_done__used__d__total__600a0a24,*param_3);
      }
      return 0;
    }
    uVar3 = (undefined1)param_6;
    if ((param_3[0x57] == 4) && (flix(0x60b02e1,0xd002741), *(char *)(param_4 + 0xd8) != '\0')) {
      if ((*param_5 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a8c,
                     PTR_s_flush_async__event_received__exi_600a0a90);
      }
      *(undefined1 *)(param_4 + 0xd8) = uVar3;
      goto LAB_600a405c;
    }
    flix(0xd0b0eb8,0xd000a41);
    if ((*(char *)(param_4 + 0x8c) != '\0') || (*(char *)(param_4 + 0x8d) != '\0')) {
      if ((*param_5 & 8) != 0) {
        puVar5 = PTR_s_offset_600a08bc;
        if (*(char *)(param_4 + 0x8c) == '\0') {
          puVar5 = PTR_s_state_600a08b8;
        }
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a14,
                     PTR_s__s_is_changed__600a08c4,puVar5);
      }
      uVar4 = *(undefined4 *)(param_4 + 0x40);
      *(undefined1 *)(param_4 + 0x8d) = uVar3;
      *(undefined1 *)(param_4 + 0x8c) = uVar3;
      cVar1 = *(char *)(param_4 + 0xd9);
      param_3[0x55] = param_6;
      param_3[0x50] = uVar4;
      if ((cVar1 == '\0') || (*(char *)(param_4 + 0x8e) == '\0')) {
        param_3[0x57] = 6;
      }
      else {
        *(undefined1 *)(param_4 + 0x8e) = uVar3;
        if ((*param_5 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_process_600a0a10,DAT_600a0a18,
                       PTR_s_lipsync_state_keep_stay__prevAct_600a08cc,param_3[0x57]);
        }
      }
    }
  } while( true );
}


/* @600a405a _lipsync_chain__blk_600a405a */

/* WARNING: Removing unreachable block (ram,0x600a4082) */
/* WARNING: Removing unreachable block (ram,0x600a4088) */
/* WARNING: Removing unreachable block (ram,0x600a40d2) */

undefined4
_lipsync_chain__blk_600a405a(undefined4 param_1,undefined4 param_2,int param_3,int param_4)

{
  flix(0x50b0180,0xd401841);
  if ((*(int *)(param_3 + 0x17c) != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
    msf_flush_metadata(param_3 + 0x114);
  }
  return 1;
}


/* @600a40ab _lipsync_chain__blk_600a40ab */

undefined4 _lipsync_chain__blk_600a40ab(void)

{
  return 0;
}


/* @600a40b9 FUN_600a40b9 */

undefined4 FUN_600a40b9(undefined4 param_1,undefined4 param_2,int param_3)

{
  aud_object_put(param_2);
  if (*(int *)(param_3 + 0x100) != *(int *)(param_3 + 0x104)) {
    aud_data_adapter_flush(param_3 + 0xf0);
  }
  return 0;
}


/* @600a4176 _lipsync_chain__blk_600a4176 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int _lipsync_chain__blk_600a4176
              (undefined4 param_1,int param_2,undefined4 *param_3,int param_4,uint *param_5,
              undefined4 param_6)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 uVar3;
  int iVar4;
  char cVar5;
  char cVar6;
  uint uVar7;
  undefined4 uVar8;
  undefined *puVar9;
  
  if ((*param_5 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_chain_600a0a1c,_LAB_600a0a40,_LAB_600a0a42_2,
                 param_3[0x83]);
  }
  flix(0x70b02b0,0xd402a41);
  if (*(int *)(param_2 + 0x10) == 3) {
    flix(0x40b02d4,0xd003341);
    if (param_3[0xa0] != 0) {
      flix(0xc0b02e3,0xd003341);
      aud_object_put();
    }
    uVar1 = aud_object_get(param_2);
    param_3[0xa0] = uVar1;
    aud_object_put(param_2);
  }
  else {
    aud_data_adapter_push(param_3 + 0x3c,param_2);
    flix(0xe0b02b5,0xd002a41);
  }
  if (*(char *)(param_4 + 0xd9) != '\0') {
    iVar4 = *(int *)PTR_DAT_6009fa98;
    iVar2 = 10;
    if (param_3[0x44] == 0) {
      iVar2 = 9;
    }
    if (iVar4 == 0) {
      flix(0x90b003c,0xd7fd841);
    }
    else if (*(char *)(iVar4 + 0x32) != iVar2) {
      flix(0x90b0282,0xd002a41);
      aud_module_chain_locked(iVar4,iVar2);
    }
  }
  uVar1 = param_3[0x41];
  uVar7 = *(uint *)(param_4 + 0xd0);
  uVar8 = *(undefined4 *)(param_4 + 0x34);
  flix(0x80b0ebd,0xd002541);
  while( true ) {
    flix(0xd0b0eb8,0xd000a41);
    cVar6 = *(char *)(param_4 + 0x8d);
    uVar3 = (undefined1)param_6;
    if ((*(char *)(param_4 + 0x8c) == '\0') && (cVar6 == '\0')) {
      cVar5 = '\0';
    }
    else {
      if ((*param_5 & 8) != 0) {
        puVar9 = PTR_s_offset_600a08bc;
        if (*(char *)(param_4 + 0x8c) == '\0') {
          puVar9 = PTR_s_state_600a08b8;
        }
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a14,
                     PTR_s__s_is_changed__600a08c4,puVar9);
      }
      uVar1 = *(undefined4 *)(param_4 + 0x40);
      *(undefined1 *)(param_4 + 0x8d) = uVar3;
      *(undefined1 *)(param_4 + 0x8c) = uVar3;
      cVar5 = *(char *)(param_4 + 0xd9);
      param_3[0x55] = param_6;
      param_3[0x50] = uVar1;
      if ((cVar5 == '\0') || (cVar6 = *(char *)(param_4 + 0x8e), cVar6 == '\0')) {
        param_3[0x57] = 6;
      }
      else {
        *(undefined1 *)(param_4 + 0x8e) = uVar3;
        uVar7 = *param_5;
        if ((uVar7 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_process_600a0a10,DAT_600a0a18,
                       PTR_s_lipsync_state_keep_stay__prevAct_600a08cc,param_3[0x57]);
        }
      }
    }
    iVar2 = _lipsync_core(param_3,cVar5,uVar1,cVar6,uVar7,uVar8);
    if (iVar2 != 0) goto LAB_600a405c;
    if ((param_3[0x57] == 4) && (flix(0x60b02e1,0xd002741), *(char *)(param_4 + 0xd8) != '\0'))
    break;
    uVar1 = param_3[0x41];
    uVar8 = *(undefined4 *)(param_4 + 0x34);
    uVar7 = *(uint *)(param_4 + 0xd0);
  }
  if ((*param_5 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a8c,
                 PTR_s_flush_async__event_received__exi_600a0a90);
  }
  *(undefined1 *)(param_4 + 0xd8) = uVar3;
LAB_600a405c:
  flix(0x50b0180,0xd401841);
  if ((param_3[0x5f] != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
    msf_flush_metadata(param_3 + 0x45);
  }
  if (iVar2 != 0) {
    return iVar2;
  }
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a20,
                 PTR_s___s__bytes_done__used__d__total__600a0a24,*param_3);
  }
  return 0;
}


/* @600a4199 _lipsync_chain__blk_600a4199 */

int _lipsync_chain__blk_600a4199
              (undefined4 param_1,int param_2,undefined4 *param_3,int param_4,uint *param_5,
              undefined4 param_6)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 uVar3;
  int iVar4;
  char cVar5;
  char cVar6;
  uint uVar7;
  undefined4 uVar8;
  undefined *puVar9;
  
  flix(0x70b02b0,0xd402a41);
  if (*(int *)(param_2 + 0x10) == 3) {
    flix(0x40b02d4,0xd003341);
    if (param_3[0xa0] != 0) {
      flix(0xc0b02e3,0xd003341);
      aud_object_put();
    }
    uVar1 = aud_object_get(param_2);
    param_3[0xa0] = uVar1;
    aud_object_put(param_2);
  }
  else {
    aud_data_adapter_push(param_3 + 0x3c,param_2);
    flix(0xe0b02b5,0xd002a41);
  }
  if (*(char *)(param_4 + 0xd9) != '\0') {
    iVar4 = *(int *)PTR_DAT_6009fa98;
    iVar2 = 10;
    if (param_3[0x44] == 0) {
      iVar2 = 9;
    }
    if (iVar4 == 0) {
      flix(0x90b003c,0xd7fd841);
    }
    else if (*(char *)(iVar4 + 0x32) != iVar2) {
      flix(0x90b0282,0xd002a41);
      aud_module_chain_locked(iVar4,iVar2);
    }
  }
  uVar1 = param_3[0x41];
  uVar7 = *(uint *)(param_4 + 0xd0);
  uVar8 = *(undefined4 *)(param_4 + 0x34);
  flix(0x80b0ebd,0xd002541);
  while( true ) {
    flix(0xd0b0eb8,0xd000a41);
    cVar6 = *(char *)(param_4 + 0x8d);
    uVar3 = (undefined1)param_6;
    if ((*(char *)(param_4 + 0x8c) == '\0') && (cVar6 == '\0')) {
      cVar5 = '\0';
    }
    else {
      if ((*param_5 & 8) != 0) {
        puVar9 = PTR_s_offset_600a08bc;
        if (*(char *)(param_4 + 0x8c) == '\0') {
          puVar9 = PTR_s_state_600a08b8;
        }
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a14,
                     PTR_s__s_is_changed__600a08c4,puVar9);
      }
      uVar1 = *(undefined4 *)(param_4 + 0x40);
      *(undefined1 *)(param_4 + 0x8d) = uVar3;
      *(undefined1 *)(param_4 + 0x8c) = uVar3;
      cVar5 = *(char *)(param_4 + 0xd9);
      param_3[0x55] = param_6;
      param_3[0x50] = uVar1;
      if ((cVar5 == '\0') || (cVar6 = *(char *)(param_4 + 0x8e), cVar6 == '\0')) {
        param_3[0x57] = 6;
      }
      else {
        *(undefined1 *)(param_4 + 0x8e) = uVar3;
        uVar7 = *param_5;
        if ((uVar7 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_process_600a0a10,DAT_600a0a18,
                       PTR_s_lipsync_state_keep_stay__prevAct_600a08cc,param_3[0x57]);
        }
      }
    }
    iVar2 = _lipsync_core(param_3,cVar5,uVar1,cVar6,uVar7,uVar8);
    if (iVar2 != 0) goto LAB_600a405c;
    if ((param_3[0x57] == 4) && (flix(0x60b02e1,0xd002741), *(char *)(param_4 + 0xd8) != '\0'))
    break;
    uVar1 = param_3[0x41];
    uVar8 = *(undefined4 *)(param_4 + 0x34);
    uVar7 = *(uint *)(param_4 + 0xd0);
  }
  if ((*param_5 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a8c,
                 PTR_s_flush_async__event_received__exi_600a0a90);
  }
  *(undefined1 *)(param_4 + 0xd8) = uVar3;
LAB_600a405c:
  flix(0x50b0180,0xd401841);
  if ((param_3[0x5f] != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
    msf_flush_metadata(param_3 + 0x45);
  }
  if (iVar2 != 0) {
    return iVar2;
  }
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a20,
                 PTR_s___s__bytes_done__used__d__total__600a0a24,*param_3);
  }
  return 0;
}


/* @600a419f _lipsync_chain__blk_600a419f */

int _lipsync_chain__blk_600a419f
              (undefined4 param_1,int param_2,undefined4 *param_3,int param_4,uint *param_5,
              undefined4 param_6)

{
  int iVar1;
  undefined1 uVar2;
  int iVar3;
  char cVar4;
  undefined4 uVar5;
  char cVar6;
  uint uVar7;
  undefined4 uVar8;
  undefined *puVar9;
  
  if ((param_3[0x75] != 1) &&
     (((*(char *)(param_4 + 0xd9) != '\0' || ((2 < (uint)param_3[0x76] && (param_3[0x5f] != 1)))) &&
      (iVar1 = msf_process_metadata
                         (param_3 + 0x45,*(undefined4 *)(param_2 + 0x38),
                          *(undefined4 *)(param_2 + 0x34)), iVar1 != 0)))) {
    flix(0xd0b02b0,0xd000d41);
  }
  aud_object_put(param_2);
  if (*(char *)(param_4 + 0xd9) != '\0') {
    iVar3 = *(int *)PTR_DAT_6009fa98;
    iVar1 = 10;
    if (param_3[0x44] == 0) {
      iVar1 = 9;
    }
    if (iVar3 == 0) {
      flix(0x90b003c,0xd7fd841);
    }
    else if (*(char *)(iVar3 + 0x32) != iVar1) {
      flix(0x90b0282,0xd002a41);
      aud_module_chain_locked(iVar3,iVar1);
    }
  }
  uVar5 = param_3[0x41];
  uVar7 = *(uint *)(param_4 + 0xd0);
  uVar8 = *(undefined4 *)(param_4 + 0x34);
  flix(0x80b0ebd,0xd002541);
  while( true ) {
    flix(0xd0b0eb8,0xd000a41);
    cVar6 = *(char *)(param_4 + 0x8d);
    uVar2 = (undefined1)param_6;
    if ((*(char *)(param_4 + 0x8c) == '\0') && (cVar6 == '\0')) {
      cVar4 = '\0';
    }
    else {
      if ((*param_5 & 8) != 0) {
        puVar9 = PTR_s_offset_600a08bc;
        if (*(char *)(param_4 + 0x8c) == '\0') {
          puVar9 = PTR_s_state_600a08b8;
        }
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a14,
                     PTR_s__s_is_changed__600a08c4,puVar9);
      }
      uVar5 = *(undefined4 *)(param_4 + 0x40);
      *(undefined1 *)(param_4 + 0x8d) = uVar2;
      *(undefined1 *)(param_4 + 0x8c) = uVar2;
      cVar4 = *(char *)(param_4 + 0xd9);
      param_3[0x55] = param_6;
      param_3[0x50] = uVar5;
      if ((cVar4 == '\0') || (cVar6 = *(char *)(param_4 + 0x8e), cVar6 == '\0')) {
        param_3[0x57] = 6;
      }
      else {
        *(undefined1 *)(param_4 + 0x8e) = uVar2;
        uVar7 = *param_5;
        if ((uVar7 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_process_600a0a10,DAT_600a0a18,
                       PTR_s_lipsync_state_keep_stay__prevAct_600a08cc,param_3[0x57]);
        }
      }
    }
    iVar1 = _lipsync_core(param_3,cVar4,uVar5,cVar6,uVar7,uVar8);
    if (iVar1 != 0) goto LAB_600a405c;
    if ((param_3[0x57] == 4) && (flix(0x60b02e1,0xd002741), *(char *)(param_4 + 0xd8) != '\0'))
    break;
    uVar5 = param_3[0x41];
    uVar8 = *(undefined4 *)(param_4 + 0x34);
    uVar7 = *(uint *)(param_4 + 0xd0);
  }
  if ((*param_5 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a8c,
                 PTR_s_flush_async__event_received__exi_600a0a90);
  }
  *(undefined1 *)(param_4 + 0xd8) = uVar2;
LAB_600a405c:
  flix(0x50b0180,0xd401841);
  if ((param_3[0x5f] != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
    msf_flush_metadata(param_3 + 0x45);
  }
  if (iVar1 != 0) {
    return iVar1;
  }
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a20,
                 PTR_s___s__bytes_done__used__d__total__600a0a24,*param_3);
  }
  return 0;
}


/* @600a41ea _lipsync_chain__blk_600a41ea */

/* WARNING: Removing unreachable block (ram,0x600a4082) */
/* WARNING: Removing unreachable block (ram,0x600a4088) */
/* WARNING: Removing unreachable block (ram,0x600a40d2) */

undefined4
_lipsync_chain__blk_600a41ea
          (undefined4 param_1,undefined4 param_2,int param_3,int param_4,undefined4 param_5,
          undefined4 param_6)

{
  int unaff_a9;
  
  *(undefined4 *)(param_3 + 0x1c8) = param_6;
  if ((unaff_a9 != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
    msf_flush_metadata(param_3 + 0x114);
  }
  return 0xd;
}


/* @600a41f2 _lipsync_chain__blk_600a41f2 */

undefined4 _lipsync_chain__blk_600a41f2(void)

{
  return 8;
}


/* @600a41f6 _lipsync_chain__blk_600a41f6 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_chain__blk_600a41f6(undefined4 param_1,int param_2,int param_3)

{
  int iStack00000000;
  
  iStack00000000 = *(int *)(param_3 + 0x100) - *(int *)(param_3 + 0x104);
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,5,PTR_s__lipsync_chain_600a0a1c,_LAB_600a0a45_3,_LAB_600a0a45_7,
               *(undefined4 *)(param_2 + 0x34));
}


/* @600a4215 _lipsync_chain__blk_600a4215 */

int _lipsync_chain__blk_600a4215
              (undefined4 param_1,undefined4 param_2,undefined4 *param_3,int param_4,uint *param_5,
              undefined4 param_6)

{
  int iVar1;
  undefined1 uVar2;
  int iVar3;
  char cVar4;
  undefined4 uVar5;
  char cVar6;
  uint uVar7;
  undefined4 uVar8;
  undefined *puVar9;
  
  if (*(char *)(param_4 + 0xd9) != '\0') {
    iVar3 = *(int *)PTR_DAT_6009fa98;
    iVar1 = 10;
    if (param_3[0x44] == 0) {
      iVar1 = 9;
    }
    if (iVar3 == 0) {
      flix(0x90b003c,0xd7fd841);
    }
    else if (*(char *)(iVar3 + 0x32) != iVar1) {
      flix(0x90b0282,0xd002a41);
      aud_module_chain_locked(iVar3,iVar1);
    }
  }
  uVar5 = param_3[0x41];
  uVar7 = *(uint *)(param_4 + 0xd0);
  uVar8 = *(undefined4 *)(param_4 + 0x34);
  flix(0x80b0ebd,0xd002541);
  while( true ) {
    flix(0xd0b0eb8,0xd000a41);
    cVar6 = *(char *)(param_4 + 0x8d);
    uVar2 = (undefined1)param_6;
    if ((*(char *)(param_4 + 0x8c) == '\0') && (cVar6 == '\0')) {
      cVar4 = '\0';
    }
    else {
      if ((*param_5 & 8) != 0) {
        puVar9 = PTR_s_offset_600a08bc;
        if (*(char *)(param_4 + 0x8c) == '\0') {
          puVar9 = PTR_s_state_600a08b8;
        }
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a14,
                     PTR_s__s_is_changed__600a08c4,puVar9);
      }
      uVar5 = *(undefined4 *)(param_4 + 0x40);
      *(undefined1 *)(param_4 + 0x8d) = uVar2;
      *(undefined1 *)(param_4 + 0x8c) = uVar2;
      cVar4 = *(char *)(param_4 + 0xd9);
      param_3[0x55] = param_6;
      param_3[0x50] = uVar5;
      if ((cVar4 == '\0') || (cVar6 = *(char *)(param_4 + 0x8e), cVar6 == '\0')) {
        param_3[0x57] = 6;
      }
      else {
        *(undefined1 *)(param_4 + 0x8e) = uVar2;
        uVar7 = *param_5;
        if ((uVar7 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_process_600a0a10,DAT_600a0a18,
                       PTR_s_lipsync_state_keep_stay__prevAct_600a08cc,param_3[0x57]);
        }
      }
    }
    iVar1 = _lipsync_core(param_3,cVar4,uVar5,cVar6,uVar7,uVar8);
    if (iVar1 != 0) goto LAB_600a405c;
    if ((param_3[0x57] == 4) && (flix(0x60b02e1,0xd002741), *(char *)(param_4 + 0xd8) != '\0'))
    break;
    uVar5 = param_3[0x41];
    uVar8 = *(undefined4 *)(param_4 + 0x34);
    uVar7 = *(uint *)(param_4 + 0xd0);
  }
  if ((*param_5 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a8c,
                 PTR_s_flush_async__event_received__exi_600a0a90);
  }
  *(undefined1 *)(param_4 + 0xd8) = uVar2;
LAB_600a405c:
  flix(0x50b0180,0xd401841);
  if ((param_3[0x5f] != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
    msf_flush_metadata(param_3 + 0x45);
  }
  if (iVar1 != 0) {
    return iVar1;
  }
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a20,
                 PTR_s___s__bytes_done__used__d__total__600a0a24,*param_3);
  }
  return 0;
}


/* @600a421b _lipsync_chain__blk_600a421b */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_chain__blk_600a421b(undefined4 param_1,undefined4 param_2)

{
  undefined4 uStack00000000;
  
  uStack00000000 = param_2;
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_chain_600a0a1c,_LAB_600a0a4d_3,_LAB_600a0a4d_7);
}


/* @600a4230 _lipsync_chain__blk_600a4230 */

int _lipsync_chain__blk_600a4230
              (undefined4 param_1,undefined4 param_2,undefined4 *param_3,int param_4,uint *param_5,
              undefined4 param_6)

{
  int iVar1;
  undefined1 uVar2;
  char cVar3;
  undefined4 uVar4;
  char cVar5;
  uint uVar6;
  undefined4 uVar7;
  undefined *puVar8;
  
  aud_module_chain_locked(*(undefined4 *)PTR_DAT_6009fa98,param_2);
  uVar4 = param_3[0x41];
  uVar6 = *(uint *)(param_4 + 0xd0);
  uVar7 = *(undefined4 *)(param_4 + 0x34);
  flix(0x80b0ebd,0xd002541);
  while( true ) {
    flix(0xd0b0eb8,0xd000a41);
    cVar5 = *(char *)(param_4 + 0x8d);
    uVar2 = (undefined1)param_6;
    if ((*(char *)(param_4 + 0x8c) == '\0') && (cVar5 == '\0')) {
      cVar3 = '\0';
    }
    else {
      if ((*param_5 & 8) != 0) {
        puVar8 = PTR_s_offset_600a08bc;
        if (*(char *)(param_4 + 0x8c) == '\0') {
          puVar8 = PTR_s_state_600a08b8;
        }
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a14,
                     PTR_s__s_is_changed__600a08c4,puVar8);
      }
      uVar4 = *(undefined4 *)(param_4 + 0x40);
      *(undefined1 *)(param_4 + 0x8d) = uVar2;
      *(undefined1 *)(param_4 + 0x8c) = uVar2;
      cVar3 = *(char *)(param_4 + 0xd9);
      param_3[0x55] = param_6;
      param_3[0x50] = uVar4;
      if ((cVar3 == '\0') || (cVar5 = *(char *)(param_4 + 0x8e), cVar5 == '\0')) {
        param_3[0x57] = 6;
      }
      else {
        *(undefined1 *)(param_4 + 0x8e) = uVar2;
        uVar6 = *param_5;
        if ((uVar6 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_process_600a0a10,DAT_600a0a18,
                       PTR_s_lipsync_state_keep_stay__prevAct_600a08cc,param_3[0x57]);
        }
      }
    }
    iVar1 = _lipsync_core(param_3,cVar3,uVar4,cVar5,uVar6,uVar7);
    if (iVar1 != 0) goto LAB_600a405c;
    if ((param_3[0x57] == 4) && (flix(0x60b02e1,0xd002741), *(char *)(param_4 + 0xd8) != '\0'))
    break;
    uVar4 = param_3[0x41];
    uVar7 = *(undefined4 *)(param_4 + 0x34);
    uVar6 = *(uint *)(param_4 + 0xd0);
  }
  if ((*param_5 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a8c,
                 PTR_s_flush_async__event_received__exi_600a0a90);
  }
  *(undefined1 *)(param_4 + 0xd8) = uVar2;
LAB_600a405c:
  flix(0x50b0180,0xd401841);
  if ((param_3[0x5f] != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
    msf_flush_metadata(param_3 + 0x45);
  }
  if (iVar1 != 0) {
    return iVar1;
  }
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a20,
                 PTR_s___s__bytes_done__used__d__total__600a0a24,*param_3);
  }
  return 0;
}


/* @600a4239 _lipsync_chain__blk_600a4239 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_chain__blk_600a4239(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,
               lgsrcSinctable_makeTable__blk_600a0a56,_LAB_600a0a5c);
}


/* @600a424a _lipsync_chain__blk_600a424a */

int _lipsync_chain__blk_600a424a
              (undefined4 param_1,undefined4 param_2,undefined4 *param_3,int param_4,uint *param_5,
              undefined4 param_6)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 uVar3;
  int iVar4;
  char cVar5;
  char cVar6;
  uint uVar7;
  undefined4 uVar8;
  undefined *puVar9;
  
  if (param_3[0xa0] != 0) {
    flix(0xc0b02e3,0xd003341);
    aud_object_put();
  }
  uVar1 = aud_object_get(param_2);
  param_3[0xa0] = uVar1;
  aud_object_put(param_2);
  if (*(char *)(param_4 + 0xd9) != '\0') {
    iVar4 = *(int *)PTR_DAT_6009fa98;
    iVar2 = 10;
    if (param_3[0x44] == 0) {
      iVar2 = 9;
    }
    if (iVar4 == 0) {
      flix(0x90b003c,0xd7fd841);
    }
    else if (*(char *)(iVar4 + 0x32) != iVar2) {
      flix(0x90b0282,0xd002a41);
      aud_module_chain_locked(iVar4,iVar2);
    }
  }
  uVar1 = param_3[0x41];
  uVar7 = *(uint *)(param_4 + 0xd0);
  uVar8 = *(undefined4 *)(param_4 + 0x34);
  flix(0x80b0ebd,0xd002541);
  while( true ) {
    flix(0xd0b0eb8,0xd000a41);
    cVar6 = *(char *)(param_4 + 0x8d);
    uVar3 = (undefined1)param_6;
    if ((*(char *)(param_4 + 0x8c) == '\0') && (cVar6 == '\0')) {
      cVar5 = '\0';
    }
    else {
      if ((*param_5 & 8) != 0) {
        puVar9 = PTR_s_offset_600a08bc;
        if (*(char *)(param_4 + 0x8c) == '\0') {
          puVar9 = PTR_s_state_600a08b8;
        }
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a14,
                     PTR_s__s_is_changed__600a08c4,puVar9);
      }
      uVar1 = *(undefined4 *)(param_4 + 0x40);
      *(undefined1 *)(param_4 + 0x8d) = uVar3;
      *(undefined1 *)(param_4 + 0x8c) = uVar3;
      cVar5 = *(char *)(param_4 + 0xd9);
      param_3[0x55] = param_6;
      param_3[0x50] = uVar1;
      if ((cVar5 == '\0') || (cVar6 = *(char *)(param_4 + 0x8e), cVar6 == '\0')) {
        param_3[0x57] = 6;
      }
      else {
        *(undefined1 *)(param_4 + 0x8e) = uVar3;
        uVar7 = *param_5;
        if ((uVar7 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_process_600a0a10,DAT_600a0a18,
                       PTR_s_lipsync_state_keep_stay__prevAct_600a08cc,param_3[0x57]);
        }
      }
    }
    iVar2 = _lipsync_core(param_3,cVar5,uVar1,cVar6,uVar7,uVar8);
    if (iVar2 != 0) goto LAB_600a405c;
    if ((param_3[0x57] == 4) && (flix(0x60b02e1,0xd002741), *(char *)(param_4 + 0xd8) != '\0'))
    break;
    uVar1 = param_3[0x41];
    uVar8 = *(undefined4 *)(param_4 + 0x34);
    uVar7 = *(uint *)(param_4 + 0xd0);
  }
  if ((*param_5 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a8c,
                 PTR_s_flush_async__event_received__exi_600a0a90);
  }
  *(undefined1 *)(param_4 + 0xd8) = uVar3;
LAB_600a405c:
  flix(0x50b0180,0xd401841);
  if ((param_3[0x5f] != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
    msf_flush_metadata(param_3 + 0x45);
  }
  if (iVar2 != 0) {
    return iVar2;
  }
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a20,
                 PTR_s___s__bytes_done__used__d__total__600a0a24,*param_3);
  }
  return 0;
}


/* @600a4250 _lipsync_chain__blk_600a4250 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_chain__blk_600a4250(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_chain_600a0a1c,_LAB_600a0a5e_2,_LAB_600a0a64);
}


/* @600a4261 _lipsync_chain__blk_600a4261 */

int _lipsync_chain__blk_600a4261
              (undefined4 param_1,undefined4 param_2,undefined4 *param_3,int param_4,uint *param_5,
              undefined4 param_6)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 uVar3;
  int iVar4;
  char cVar5;
  char cVar6;
  uint uVar7;
  undefined4 uVar8;
  undefined *puVar9;
  
  aud_object_put(param_3[0xa0]);
  uVar1 = aud_object_get(param_2);
  param_3[0xa0] = uVar1;
  aud_object_put(param_2);
  if (*(char *)(param_4 + 0xd9) != '\0') {
    iVar4 = *(int *)PTR_DAT_6009fa98;
    iVar2 = 10;
    if (param_3[0x44] == 0) {
      iVar2 = 9;
    }
    if (iVar4 == 0) {
      flix(0x90b003c,0xd7fd841);
    }
    else if (*(char *)(iVar4 + 0x32) != iVar2) {
      flix(0x90b0282,0xd002a41);
      aud_module_chain_locked(iVar4,iVar2);
    }
  }
  uVar1 = param_3[0x41];
  uVar7 = *(uint *)(param_4 + 0xd0);
  uVar8 = *(undefined4 *)(param_4 + 0x34);
  flix(0x80b0ebd,0xd002541);
  while( true ) {
    flix(0xd0b0eb8,0xd000a41);
    cVar6 = *(char *)(param_4 + 0x8d);
    uVar3 = (undefined1)param_6;
    if ((*(char *)(param_4 + 0x8c) == '\0') && (cVar6 == '\0')) {
      cVar5 = '\0';
    }
    else {
      if ((*param_5 & 8) != 0) {
        puVar9 = PTR_s_offset_600a08bc;
        if (*(char *)(param_4 + 0x8c) == '\0') {
          puVar9 = PTR_s_state_600a08b8;
        }
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a14,
                     PTR_s__s_is_changed__600a08c4,puVar9);
      }
      uVar1 = *(undefined4 *)(param_4 + 0x40);
      *(undefined1 *)(param_4 + 0x8d) = uVar3;
      *(undefined1 *)(param_4 + 0x8c) = uVar3;
      cVar5 = *(char *)(param_4 + 0xd9);
      param_3[0x55] = param_6;
      param_3[0x50] = uVar1;
      if ((cVar5 == '\0') || (cVar6 = *(char *)(param_4 + 0x8e), cVar6 == '\0')) {
        param_3[0x57] = 6;
      }
      else {
        *(undefined1 *)(param_4 + 0x8e) = uVar3;
        uVar7 = *param_5;
        if ((uVar7 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_process_600a0a10,DAT_600a0a18,
                       PTR_s_lipsync_state_keep_stay__prevAct_600a08cc,param_3[0x57]);
        }
      }
    }
    iVar2 = _lipsync_core(param_3,cVar5,uVar1,cVar6,uVar7,uVar8);
    if (iVar2 != 0) goto LAB_600a405c;
    if ((param_3[0x57] == 4) && (flix(0x60b02e1,0xd002741), *(char *)(param_4 + 0xd8) != '\0'))
    break;
    uVar1 = param_3[0x41];
    uVar8 = *(undefined4 *)(param_4 + 0x34);
    uVar7 = *(uint *)(param_4 + 0xd0);
  }
  if ((*param_5 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a8c,
                 PTR_s_flush_async__event_received__exi_600a0a90);
  }
  *(undefined1 *)(param_4 + 0xd8) = uVar3;
LAB_600a405c:
  flix(0x50b0180,0xd401841);
  if ((param_3[0x5f] != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
    msf_flush_metadata(param_3 + 0x45);
  }
  if (iVar2 != 0) {
    return iVar2;
  }
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a20,
                 PTR_s___s__bytes_done__used__d__total__600a0a24,*param_3);
  }
  return 0;
}


/* @600a426a _lipsync_chain__blk_600a426a */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_chain__blk_600a426a(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl();
}


/* @600a42a5 _lipsync_chain__blk_600a42a5 */

int _lipsync_chain__blk_600a42a5
              (undefined4 param_1,int param_2,undefined4 *param_3,int param_4,uint *param_5,
              undefined4 param_6)

{
  undefined4 uVar1;
  int iVar2;
  undefined1 uVar3;
  int iVar4;
  char cVar5;
  char cVar6;
  uint uVar7;
  undefined4 uVar8;
  undefined *puVar9;
  
  flix(0x70b02b0,0xd402a41);
  if (*(int *)(param_2 + 0x10) == 3) {
    flix(0x40b02d4,0xd003341);
    if (param_3[0xa0] != 0) {
      flix(0xc0b02e3,0xd003341);
      aud_object_put();
    }
    uVar1 = aud_object_get(param_2);
    param_3[0xa0] = uVar1;
    aud_object_put(param_2);
  }
  else {
    aud_data_adapter_push(param_3 + 0x3c,param_2);
    flix(0xe0b02b5,0xd002a41);
  }
  if (*(char *)(param_4 + 0xd9) != '\0') {
    iVar4 = *(int *)PTR_DAT_6009fa98;
    iVar2 = 10;
    if (param_3[0x44] == 0) {
      iVar2 = 9;
    }
    if (iVar4 == 0) {
      flix(0x90b003c,0xd7fd841);
    }
    else if (*(char *)(iVar4 + 0x32) != iVar2) {
      flix(0x90b0282,0xd002a41);
      aud_module_chain_locked(iVar4,iVar2);
    }
  }
  uVar1 = param_3[0x41];
  uVar7 = *(uint *)(param_4 + 0xd0);
  uVar8 = *(undefined4 *)(param_4 + 0x34);
  flix(0x80b0ebd,0xd002541);
  while( true ) {
    flix(0xd0b0eb8,0xd000a41);
    cVar6 = *(char *)(param_4 + 0x8d);
    uVar3 = (undefined1)param_6;
    if ((*(char *)(param_4 + 0x8c) == '\0') && (cVar6 == '\0')) {
      cVar5 = '\0';
    }
    else {
      if ((*param_5 & 8) != 0) {
        puVar9 = PTR_s_offset_600a08bc;
        if (*(char *)(param_4 + 0x8c) == '\0') {
          puVar9 = PTR_s_state_600a08b8;
        }
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a14,
                     PTR_s__s_is_changed__600a08c4,puVar9);
      }
      uVar1 = *(undefined4 *)(param_4 + 0x40);
      *(undefined1 *)(param_4 + 0x8d) = uVar3;
      *(undefined1 *)(param_4 + 0x8c) = uVar3;
      cVar5 = *(char *)(param_4 + 0xd9);
      param_3[0x55] = param_6;
      param_3[0x50] = uVar1;
      if ((cVar5 == '\0') || (cVar6 = *(char *)(param_4 + 0x8e), cVar6 == '\0')) {
        param_3[0x57] = 6;
      }
      else {
        *(undefined1 *)(param_4 + 0x8e) = uVar3;
        uVar7 = *param_5;
        if ((uVar7 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_process_600a0a10,DAT_600a0a18,
                       PTR_s_lipsync_state_keep_stay__prevAct_600a08cc,param_3[0x57]);
        }
      }
    }
    iVar2 = _lipsync_core(param_3,cVar5,uVar1,cVar6,uVar7,uVar8);
    if (iVar2 != 0) goto LAB_600a405c;
    if ((param_3[0x57] == 4) && (flix(0x60b02e1,0xd002741), *(char *)(param_4 + 0xd8) != '\0'))
    break;
    uVar1 = param_3[0x41];
    uVar8 = *(undefined4 *)(param_4 + 0x34);
    uVar7 = *(uint *)(param_4 + 0xd0);
  }
  if ((*param_5 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a8c,
                 PTR_s_flush_async__event_received__exi_600a0a90);
  }
  *(undefined1 *)(param_4 + 0xd8) = uVar3;
LAB_600a405c:
  flix(0x50b0180,0xd401841);
  if ((param_3[0x5f] != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
    msf_flush_metadata(param_3 + 0x45);
  }
  if (iVar2 != 0) {
    return iVar2;
  }
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a20,
                 PTR_s___s__bytes_done__used__d__total__600a0a24,*param_3);
  }
  return 0;
}


/* @600a42ab _lipsync_chain__blk_600a42ab */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void _lipsync_chain__blk_600a42ab(void)

{
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,0,PTR_s__lipsync_chain_600a0a1c,_LAB_600a0a78,
               PTR_s_msf_process_metadata___Err_600a0a7c);
}


/* @600a42bd _lipsync_chain__blk_600a42bd */

int _lipsync_chain__blk_600a42bd
              (undefined4 param_1,undefined4 param_2,undefined4 *param_3,int param_4,uint *param_5,
              undefined4 param_6)

{
  int iVar1;
  undefined1 uVar2;
  int iVar3;
  char cVar4;
  undefined4 uVar5;
  char cVar6;
  uint uVar7;
  undefined4 uVar8;
  undefined *puVar9;
  
  aud_object_put(param_2);
  if (*(char *)(param_4 + 0xd9) != '\0') {
    iVar3 = *(int *)PTR_DAT_6009fa98;
    iVar1 = 10;
    if (param_3[0x44] == 0) {
      iVar1 = 9;
    }
    if (iVar3 == 0) {
      flix(0x90b003c,0xd7fd841);
    }
    else if (*(char *)(iVar3 + 0x32) != iVar1) {
      flix(0x90b0282,0xd002a41);
      aud_module_chain_locked(iVar3,iVar1);
    }
  }
  uVar5 = param_3[0x41];
  uVar7 = *(uint *)(param_4 + 0xd0);
  uVar8 = *(undefined4 *)(param_4 + 0x34);
  flix(0x80b0ebd,0xd002541);
  while( true ) {
    flix(0xd0b0eb8,0xd000a41);
    cVar6 = *(char *)(param_4 + 0x8d);
    uVar2 = (undefined1)param_6;
    if ((*(char *)(param_4 + 0x8c) == '\0') && (cVar6 == '\0')) {
      cVar4 = '\0';
    }
    else {
      if ((*param_5 & 8) != 0) {
        puVar9 = PTR_s_offset_600a08bc;
        if (*(char *)(param_4 + 0x8c) == '\0') {
          puVar9 = PTR_s_state_600a08b8;
        }
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a14,
                     PTR_s__s_is_changed__600a08c4,puVar9);
      }
      uVar5 = *(undefined4 *)(param_4 + 0x40);
      *(undefined1 *)(param_4 + 0x8d) = uVar2;
      *(undefined1 *)(param_4 + 0x8c) = uVar2;
      cVar4 = *(char *)(param_4 + 0xd9);
      param_3[0x55] = param_6;
      param_3[0x50] = uVar5;
      if ((cVar4 == '\0') || (cVar6 = *(char *)(param_4 + 0x8e), cVar6 == '\0')) {
        param_3[0x57] = 6;
      }
      else {
        *(undefined1 *)(param_4 + 0x8e) = uVar2;
        uVar7 = *param_5;
        if ((uVar7 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_process_600a0a10,DAT_600a0a18,
                       PTR_s_lipsync_state_keep_stay__prevAct_600a08cc,param_3[0x57]);
        }
      }
    }
    iVar1 = _lipsync_core(param_3,cVar4,uVar5,cVar6,uVar7,uVar8);
    if (iVar1 != 0) goto LAB_600a405c;
    if ((param_3[0x57] == 4) && (flix(0x60b02e1,0xd002741), *(char *)(param_4 + 0xd8) != '\0'))
    break;
    uVar5 = param_3[0x41];
    uVar8 = *(undefined4 *)(param_4 + 0x34);
    uVar7 = *(uint *)(param_4 + 0xd0);
  }
  if ((*param_5 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_process_600a0a10,DAT_600a0a8c,
                 PTR_s_flush_async__event_received__exi_600a0a90);
  }
  *(undefined1 *)(param_4 + 0xd8) = uVar2;
LAB_600a405c:
  flix(0x50b0180,0xd401841);
  if ((param_3[0x5f] != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
    msf_flush_metadata(param_3 + 0x45);
  }
  if (iVar1 != 0) {
    return iVar1;
  }
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a20,
                 PTR_s___s__bytes_done__used__d__total__600a0a24,*param_3);
  }
  return 0;
}


/* @600a42c3 _lipsync_chain__blk_600a42c3 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int _lipsync_chain__blk_600a42c3
              (undefined4 param_1,int param_2,undefined4 *param_3,int param_4,uint *param_5)

{
  if ((*param_5 & 4) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,2,PTR_s__lipsync_process_600a0a10,_LAB_600a0aa0,_LAB_600a0aa4);
  }
  if (param_3[0x40] != param_3[0x41]) {
    aud_data_adapter_flush(param_3 + 0x3c);
  }
  flix(0x50b0180,0xd401841);
  if ((param_3[0x5f] != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
    msf_flush_metadata(param_3 + 0x45);
  }
  if (param_2 != 0) {
    return param_2;
  }
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a20,
                 PTR_s___s__bytes_done__used__d__total__600a0a24,*param_3);
  }
  return 0;
}


/* @600a42e3 _lipsync_chain__blk_600a42e3 */

void _lipsync_chain__blk_600a42e3(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  undefined4 uStack00000008;
  undefined4 uStack0000000c;
  undefined *puStack00000010;
  undefined *puStack00000014;
  undefined *puStack00000018;
  
  puStack00000010 = PTR_s_PCR_MAIN_600a0a80;
  uStack00000000 = *(undefined4 *)(param_3 + 0x1f0);
  uStack00000004 = *(undefined4 *)(param_3 + 0x1f8);
  uStack00000008 = *(undefined4 *)(param_3 + 0x1fc);
  uStack0000000c = *(undefined4 *)(param_3 + 500);
  puStack00000014 = PTR_s___no_delay_600a0a30;
  if (*(int *)(param_3 + 0x214) == 0) {
    puStack00000014 = PTR_s__6004874c_0x14_600a0a2c;
  }
  puStack00000018 = PTR_s___overflow_protection_600a0a34;
  if (*(int *)(param_3 + 0x218) == 0) {
    puStack00000018 = PTR_s__6004874c_0x14_600a0a2c;
  }
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a38,
               PTR_s_lipsync_on__bound__d__d__freerun_600a0a3c,*(undefined4 *)(param_3 + 0x1ec));
}


/* @600a42e9 _lipsync_chain__blk_600a42e9 */

void _lipsync_chain__blk_600a42e9(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  undefined4 uStack00000008;
  undefined4 uStack0000000c;
  undefined *puStack00000010;
  undefined *puStack00000014;
  undefined *puStack00000018;
  
  puStack00000010 = PTR_s_PCM_ASSOCIATE_600a0a84;
  uStack00000000 = *(undefined4 *)(param_3 + 0x1f0);
  uStack00000004 = *(undefined4 *)(param_3 + 0x1f8);
  uStack00000008 = *(undefined4 *)(param_3 + 0x1fc);
  uStack0000000c = *(undefined4 *)(param_3 + 500);
  puStack00000014 = PTR_s___no_delay_600a0a30;
  if (*(int *)(param_3 + 0x214) == 0) {
    puStack00000014 = PTR_s__6004874c_0x14_600a0a2c;
  }
  puStack00000018 = PTR_s___overflow_protection_600a0a34;
  if (*(int *)(param_3 + 0x218) == 0) {
    puStack00000018 = PTR_s__6004874c_0x14_600a0a2c;
  }
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a38,
               PTR_s_lipsync_on__bound__d__d__freerun_600a0a3c,*(undefined4 *)(param_3 + 0x1ec));
}


/* @600a42ef _lipsync_chain__blk_600a42ef */

void _lipsync_chain__blk_600a42ef(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  undefined4 uStack00000008;
  undefined4 uStack0000000c;
  undefined *puStack00000010;
  undefined *puStack00000014;
  undefined *puStack00000018;
  
  puStack00000010 = PTR_s_NON_PCR_600a0a88;
  uStack00000000 = *(undefined4 *)(param_3 + 0x1f0);
  uStack00000004 = *(undefined4 *)(param_3 + 0x1f8);
  uStack00000008 = *(undefined4 *)(param_3 + 0x1fc);
  uStack0000000c = *(undefined4 *)(param_3 + 500);
  puStack00000014 = PTR_s___no_delay_600a0a30;
  if (*(int *)(param_3 + 0x214) == 0) {
    puStack00000014 = PTR_s__6004874c_0x14_600a0a2c;
  }
  puStack00000018 = PTR_s___overflow_protection_600a0a34;
  if (*(int *)(param_3 + 0x218) == 0) {
    puStack00000018 = PTR_s__6004874c_0x14_600a0a2c;
  }
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a38,
               PTR_s_lipsync_on__bound__d__d__freerun_600a0a3c,*(undefined4 *)(param_3 + 0x1ec));
}


/* @600a4306 _lipsync_chain__blk_600a4306 */

int _lipsync_chain__blk_600a4306
              (undefined4 param_1,int param_2,undefined4 *param_3,int param_4,uint *param_5,
              undefined1 param_6)

{
  *(undefined1 *)(param_4 + 0xd8) = param_6;
  flix(0x50b0180,0xd401841);
  if ((param_3[0x5f] != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
    msf_flush_metadata(param_3 + 0x45);
  }
  if (param_2 != 0) {
    return param_2;
  }
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a20,
                 PTR_s___s__bytes_done__used__d__total__600a0a24,*param_3);
  }
  return 0;
}


/* @600a430c _lipsync_chain__blk_600a430c */

void _lipsync_chain__blk_600a430c(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  undefined4 uStack00000008;
  undefined4 uStack0000000c;
  undefined *puStack00000010;
  undefined *puStack00000014;
  undefined *puStack00000018;
  
  puStack00000010 = PTR_DAT_600a0a94;
  uStack00000000 = *(undefined4 *)(param_3 + 0x1f0);
  uStack00000004 = *(undefined4 *)(param_3 + 0x1f8);
  uStack00000008 = *(undefined4 *)(param_3 + 0x1fc);
  uStack0000000c = *(undefined4 *)(param_3 + 500);
  puStack00000014 = PTR_s___no_delay_600a0a30;
  if (*(int *)(param_3 + 0x214) == 0) {
    puStack00000014 = PTR_s__6004874c_0x14_600a0a2c;
  }
  puStack00000018 = PTR_s___overflow_protection_600a0a34;
  if (*(int *)(param_3 + 0x218) == 0) {
    puStack00000018 = PTR_s__6004874c_0x14_600a0a2c;
  }
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a38,
               PTR_s_lipsync_on__bound__d__d__freerun_600a0a3c,*(undefined4 *)(param_3 + 0x1ec));
}


/* @600a4312 _lipsync_chain__blk_600a4312 */

void _lipsync_chain__blk_600a4312(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  undefined4 uStack00000008;
  undefined4 uStack0000000c;
  undefined *puStack00000010;
  undefined *puStack00000014;
  undefined *puStack00000018;
  
  puStack00000010 = PTR_s_GSTC1_600a0a98;
  uStack00000000 = *(undefined4 *)(param_3 + 0x1f0);
  uStack00000004 = *(undefined4 *)(param_3 + 0x1f8);
  uStack00000008 = *(undefined4 *)(param_3 + 0x1fc);
  uStack0000000c = *(undefined4 *)(param_3 + 500);
  puStack00000014 = PTR_s___no_delay_600a0a30;
  if (*(int *)(param_3 + 0x214) == 0) {
    puStack00000014 = PTR_s__6004874c_0x14_600a0a2c;
  }
  puStack00000018 = PTR_s___overflow_protection_600a0a34;
  if (*(int *)(param_3 + 0x218) == 0) {
    puStack00000018 = PTR_s__6004874c_0x14_600a0a2c;
  }
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a38,
               PTR_s_lipsync_on__bound__d__d__freerun_600a0a3c,*(undefined4 *)(param_3 + 0x1ec));
}


/* @600a4318 _lipsync_chain__blk_600a4318 */

void _lipsync_chain__blk_600a4318(undefined4 param_1,undefined4 param_2,int param_3)

{
  undefined4 uStack00000000;
  undefined4 uStack00000004;
  undefined4 uStack00000008;
  undefined4 uStack0000000c;
  undefined *puStack00000010;
  undefined *puStack00000014;
  undefined *puStack00000018;
  
  puStack00000010 = PTR_s_AUDIO_MASTER_600a0a9c;
  uStack00000000 = *(undefined4 *)(param_3 + 0x1f0);
  uStack00000004 = *(undefined4 *)(param_3 + 0x1f8);
  uStack00000008 = *(undefined4 *)(param_3 + 0x1fc);
  uStack0000000c = *(undefined4 *)(param_3 + 500);
  puStack00000014 = PTR_s___no_delay_600a0a30;
  if (*(int *)(param_3 + 0x214) == 0) {
    puStack00000014 = PTR_s__6004874c_0x14_600a0a2c;
  }
  puStack00000018 = PTR_s___overflow_protection_600a0a34;
  if (*(int *)(param_3 + 0x218) == 0) {
    puStack00000018 = PTR_s__6004874c_0x14_600a0a2c;
  }
                    /* WARNING: Subroutine does not return */
  logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a38,
               PTR_s_lipsync_on__bound__d__d__freerun_600a0a3c,*(undefined4 *)(param_3 + 0x1ec));
}


/* @600a433d _lipsync_chain__blk_600a433d */

int _lipsync_chain__blk_600a433d
              (undefined4 param_1,int param_2,undefined4 *param_3,int param_4,uint *param_5)

{
  if (param_3[0x40] != param_3[0x41]) {
    aud_data_adapter_flush(param_3 + 0x3c);
  }
  flix(0x50b0180,0xd401841);
  if ((param_3[0x5f] != 0) && (*(char *)(param_4 + 0xd9) != '\0')) {
    msf_flush_metadata(param_3 + 0x45);
  }
  if (param_2 != 0) {
    return param_2;
  }
  if ((*param_5 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,4,PTR_s__lipsync_chain_600a0a1c,DAT_600a0a20,
                 PTR_s___s__bytes_done__used__d__total__600a0a24,*param_3);
  }
  return 0;
}


/* @600a4344 _lipsync_set_prop_offset */

undefined4 _lipsync_set_prop_offset(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = _Stoul(param_3,0,0);
  if ((*SUB_600a062c & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_set_prop_offset_600a0aa8,DAT_600a0aac,
                 PTR_s__s__offset_setting__u__>__u_600a0ab0,*param_1,param_1[0x7d],iVar1);
  }
  if (param_1[0x7d] == iVar1) {
    param_1[0x7d] = iVar1;
    return 0;
  }
  *(undefined1 *)(param_1 + 0x8c) = 1;
  param_1[0x7d] = iVar1;
  return 0;
}


/* @600a4395 _lipsync_set_prop_offset__blk_600a4395 */

undefined4 _lipsync_set_prop_offset__blk_600a4395(int param_1,int param_2)

{
  if (*(int *)(param_1 + 500) == param_2) {
    *(int *)(param_1 + 500) = param_2;
    return 0;
  }
  *(undefined1 *)(param_1 + 0x230) = 1;
  *(int *)(param_1 + 500) = param_2;
  return 0;
}


/* @600a439c _lipsync_set_prop_speed */

undefined4 _lipsync_set_prop_speed(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = _Stoul(param_3,0,0);
  if ((*SUB_600a062c & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_set_prop_speed_600a0ab4,DAT_600a0ab8,
                 PTR_s__s__speed_setting__d__>__d_600a0abc,*param_1,param_1[0x60],iVar1);
  }
  if (param_1[0x60] == iVar1) {
    param_1[0x60] = iVar1;
    param_1[0x5f] = (uint)(iVar1 != 1000);
    return 0;
  }
  *(undefined1 *)((int)param_1 + 0x232) = 1;
  param_1[0x60] = iVar1;
  param_1[0x5f] = (uint)(iVar1 != 1000);
  return 0;
}


/* @600a4408 _lipsync_set_prop_speed__blk_600a4408 */

undefined4 _lipsync_set_prop_speed__blk_600a4408(int param_1,int param_2)

{
  if (*(int *)(param_1 + 0x180) == param_2) {
    *(int *)(param_1 + 0x180) = param_2;
    *(uint *)(param_1 + 0x17c) = (uint)(param_2 != 1000);
    return 0;
  }
  *(undefined1 *)(param_1 + 0x232) = 1;
  *(int *)(param_1 + 0x180) = param_2;
  *(uint *)(param_1 + 0x17c) = (uint)(param_2 != 1000);
  return 0;
}


/* @600a440c _lipsync_set_prop_channel */

undefined4 _lipsync_set_prop_channel(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = _Stoul(param_3,0,0);
  if ((*SUB_600a062c & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_set_prop_channel_600a0ac0,DAT_600a0ac4,
                 PTR_s_Channels____d_600a07bc,iVar1);
  }
  *(int *)(param_1 + 0x160) = iVar1 << 0xb;
  *(int *)(param_1 + 0x164) = iVar1 << 0xb;
  if (*(int *)(param_1 + 0x1d8) == 0) {
    *(int *)(param_1 + 0x1d8) = iVar1;
    return 0;
  }
  return 0;
}


/* @600a444a _lipsync_set_prop_channel__blk_600a444a */

undefined4 _lipsync_set_prop_channel__blk_600a444a(int param_1,int param_2)

{
  *(int *)(param_1 + 0x160) = param_2 << 0xb;
  *(int *)(param_1 + 0x164) = param_2 << 0xb;
  if (*(int *)(param_1 + 0x1d8) == 0) {
    *(int *)(param_1 + 0x1d8) = param_2;
    return 0;
  }
  return 0;
}


/* @600a4458 _lipsync_set_prop_datatype */

undefined4 _lipsync_set_prop_datatype(int param_1,undefined4 param_2,undefined4 param_3)

{
  int iVar1;
  
  iVar1 = _Stoul(param_3,0,0);
  if ((*SUB_600a062c & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_set_prop_datatype_600a0ac8,DAT_600a0acc,
                 PTR_s_Type_0_pcm__1_es_____d_600a078c,iVar1);
  }
  *(int *)(param_1 + 0x1d4) = iVar1;
  if (iVar1 == 1) {
    *(undefined4 *)(param_1 + 0x1e0) = 2;
    *(undefined4 *)(param_1 + 0x184) = 0;
    return 0;
  }
  *(undefined4 *)(param_1 + 0x1e0) = 4;
  *(undefined4 *)(param_1 + 0x184) = 1;
  return 0;
}


/* @600a4499 _lipsync_set_prop_datatype__blk_600a4499 */

undefined4 _lipsync_set_prop_datatype__blk_600a4499(int param_1,int param_2)

{
  *(int *)(param_1 + 0x1d4) = param_2;
  if (param_2 == 1) {
    *(undefined4 *)(param_1 + 0x1e0) = 2;
    *(undefined4 *)(param_1 + 0x184) = 0;
    return 0;
  }
  *(undefined4 *)(param_1 + 0x1e0) = 4;
  *(undefined4 *)(param_1 + 0x184) = 1;
  return 0;
}


/* @600a44b0 _lipsync_set_prop_idletime */

undefined4 _lipsync_set_prop_idletime(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  longlong lVar2;
  undefined8 uVar3;
  
  uVar1 = _Stoul(param_3,0,0);
  if ((*SUB_600a062c & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_set_prop_idletime_600a0ad0,DAT_600a0ad4,
                 PTR_s_Escape_idle_time____u_msec_600a06f8,uVar1);
  }
  lVar2 = (ulonglong)DAT_600a073c << 0x20;
  if (*(int *)(param_1 + 0x1dc) != 0) {
    lVar2 = __floatunsidf();
  }
  uVar3 = __floatunsidf(uVar1);
  uVar3 = __muldf3((int)uVar3,(int)((ulonglong)uVar3 >> 0x20),(int)lVar2,
                   (int)((ulonglong)lVar2 >> 0x20));
  __muldf3((int)uVar3,(int)((ulonglong)uVar3 >> 0x20),0,DAT_600a0740);
  uVar1 = __fixdfsi();
  *(undefined4 *)(param_1 + 0x188) = uVar1;
  return 0;
}


/* @600a4518 _lipsync_set_prop_idletime__blk_600a4518 */

undefined4 _lipsync_set_prop_idletime__blk_600a4518(int param_1,undefined4 param_2)

{
  undefined4 uVar1;
  undefined4 uVar2;
  undefined8 uVar3;
  
  uVar2 = 0;
  uVar1 = DAT_600a073c;
  if (*(int *)(param_1 + 0x1dc) != 0) {
    uVar3 = __floatunsidf();
    uVar1 = (undefined4)((ulonglong)uVar3 >> 0x20);
    uVar2 = (undefined4)uVar3;
  }
  uVar3 = __floatunsidf(param_2);
  uVar3 = __muldf3((int)uVar3,(int)((ulonglong)uVar3 >> 0x20),uVar2,uVar1);
  __muldf3((int)uVar3,(int)((ulonglong)uVar3 >> 0x20),0,DAT_600a0740);
  uVar1 = __fixdfsi();
  *(undefined4 *)(param_1 + 0x188) = uVar1;
  return 0;
}


/* @600a4520 _lipsync_set_baseCT */

undefined4 _lipsync_set_baseCT(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  uVar1 = _Stoul(param_3,0,0);
  if ((*SUB_600a062c & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_set_baseCT_600a0ad8,DAT_600a0adc,
                 PTR_s__s__BaseCT__d__>__d_600a0ae0,*param_1,param_1[0x81],uVar1);
  }
  *(undefined1 *)((int)param_1 + 0x231) = 1;
  param_1[0x81] = uVar1;
  param_1[0x83] = uVar1;
  return 0;
}


/* @600a4569 _lipsync_set_baseCT__blk_600a4569 */

undefined4 _lipsync_set_baseCT__blk_600a4569(int param_1,undefined4 param_2)

{
  *(undefined1 *)(param_1 + 0x231) = 1;
  *(undefined4 *)(param_1 + 0x204) = param_2;
  *(undefined4 *)(param_1 + 0x20c) = param_2;
  return 0;
}


/* @600a457c _lipsync_set_baseST */

undefined4 _lipsync_set_baseST(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  uVar1 = _Stoul(param_3,0,0);
  if ((*SUB_600a062c & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_set_baseST_600a0ae4,DAT_600a0ae8,
                 PTR_s__s__BaseST__d__>__d_600a0aec,*param_1,param_1[0x82],uVar1);
  }
  *(undefined1 *)((int)param_1 + 0x231) = 1;
  param_1[0x82] = uVar1;
  param_1[0x84] = uVar1;
  return 0;
}


/* @600a45c4 _lipsync_set_baseST__blk_600a45c4 */

undefined4 _lipsync_set_baseST__blk_600a45c4(int param_1,undefined4 param_2)

{
  *(undefined1 *)(param_1 + 0x231) = 1;
  *(undefined4 *)(param_1 + 0x208) = param_2;
  *(undefined4 *)(param_1 + 0x210) = param_2;
  return 0;
}


/* @600a45d8 _lipsync_set_state */

void _lipsync_set_state(undefined4 *param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  uVar1 = _Stoul(param_3,0,0);
  if ((*SUB_600a062c & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_set_state_600a0af0,DAT_600a0af4,
                 PTR_s__s__change_state_to__d_600a0af8,*param_1,uVar1);
  }
  param_1[0x9e] = uVar1;
  *(undefined1 *)((int)param_1 + 0x231) = 1;
  *(undefined1 *)((int)param_1 + 0x269) = 0;
  return;
}


/* @600a461c _lipsync_set_state__blk_600a461c */

void _lipsync_set_state__blk_600a461c(int param_1,undefined4 param_2)

{
  *(undefined4 *)(param_1 + 0x278) = param_2;
  *(undefined1 *)(param_1 + 0x231) = 1;
  *(undefined1 *)(param_1 + 0x269) = 0;
  return;
}


/* @600a4630 _lipsync_set_seamless_mode */

undefined4 _lipsync_set_seamless_mode(void)

{
  if ((*SUB_600a062c & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_set_seamless_mode_600a0afc,DAT_600a0b00,
                 PTR_s_seamless_mode_is_set_only_by_the_600a0b04);
  }
  return 0;
}


/* @600a4651 _lipsync_set_seamless_mode__blk_600a4651 */

undefined4 _lipsync_set_seamless_mode__blk_600a4651(void)

{
  return 0;
}


/* @600a4658 _lipsync_disconnect_port */

undefined4 _lipsync_disconnect_port(int param_1)

{
  if ((*SUB_600a062c & 8) != 0) {
                    /* WARNING: Subroutine does not return */
    logmm_printl(PTR_DAT_600a0634,3,PTR_s__lipsync_disconnect_port_600a0b08,DAT_600a0b0c,
                 PTR_s_mark_disconnecting_600a0b10);
  }
  *(undefined1 *)(param_1 + 0x269) = 1;
  aud_mutex_lock(param_1 + 0x23c);
  aud_cond_signal(param_1 + 600);
  aud_mutex_unlock(param_1 + 0x23c);
  return 0xffffffff;
}


/* @600a4699 _lipsync_disconnect_port__blk_600a4699 */

undefined4 _lipsync_disconnect_port__blk_600a4699(int param_1)

{
  *(undefined1 *)(param_1 + 0x269) = 1;
  aud_mutex_lock(param_1 + 0x23c);
  aud_cond_signal(param_1 + 600);
  aud_mutex_unlock(param_1 + 0x23c);
  return 0xffffffff;
}


/* @600a46a0 initializer */

void initializer(void)

{
  ModuleRegistry_Register(PTR_DAT_600a0b14);
  return;
}


/* @600a46b0 SOLA_Create */

undefined4 SOLA_Create(int *param_1,int param_2,int param_3)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = (int *)_BufMonitor_RemoveBufferIdxLocal(0x18);
  *param_1 = (int)piVar1;
  *piVar1 = param_3;
  if ((param_2 < 1) || (10 < param_2)) {
    param_2 = 2;
  }
  piVar1[2] = param_2;
  if (param_3 == 2) {
    iVar2 = _BufMonitor_RemoveBufferIdxLocal(0x78);
    piVar1[1] = iVar2;
    WSOLA2_create();
    WSOLA2_SetChannel(piVar1[1],piVar1[2]);
    piVar1[5] = 0xff;
    return 0;
  }
  piVar1[5] = 0xff;
  return 0xfffffff2;
}


