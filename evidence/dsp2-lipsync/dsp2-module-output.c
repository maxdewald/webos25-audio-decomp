/* Ghidra pseudocode, not original source. FLIX operations remain opaque;
 * __blk_ routines are synthetic exports, not proven independent functions. */

/* @600724d8 aud_module_push_data */

undefined4 aud_module_push_data(undefined4 *param_1,undefined4 param_2,int param_3)

{
  uint *puVar1;
  undefined *puVar2;
  undefined *puVar3;
  int iVar4;
  
  puVar3 = PTR_s_aud_module_push_data_6005d9ac;
  puVar2 = PTR_DAT_6005d960;
  puVar1 = DAT_6005d958;
  if (param_3 == 0) {
    if ((*DAT_6005d958 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_6005d960,4,PTR_s_aud_module_push_data_6005d9ac,0xd6,
                   PTR_s__s__no_data_6005d9b4,*param_1);
    }
  }
  else {
    if (*(char *)(param_1 + 0x25) != '\0') {
      if ((*DAT_6005d958 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_6005d960,4,PTR_s_aud_module_push_data_6005d9ac,0xdc,
                     PTR_s__s__drop_output_data__6005d9b0,*param_1);
      }
      aud_object_put(param_3);
      return 0;
    }
    if (*(char *)((int)param_1 + 0x95) != '\0') {
      if (*(int *)(param_3 + 0x10) == 2) {
        if ((*DAT_6005d958 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_6005d960,4,PTR_s_aud_module_push_data_6005d9ac,0xe3,
                       PTR_s__s__drop_output_data__6005d9b0,*param_1);
        }
        aud_object_put(param_3);
        return 0;
      }
    }
    iVar4 = aud_module_get_output(param_1,param_2);
    if (iVar4 == 0) {
      if ((*puVar1 & 0x10) != 0) {
                    /* WARNING: Subroutine does not return */
        logmm_printl(puVar2,4,puVar3,0xeb,PTR_s__s__no_output_port__d_6005d9bc,*param_1,param_2);
      }
      aud_object_put(param_3);
      return 0;
    }
    aud_mutex_lock(iVar4 + 0xc);
    aud_mutex_unlock(iVar4 + 0x68);
    add_pending_obj_locked(iVar4,param_3);
    aud_mutex_unlock(iVar4 + 0xc);
    if (*DAT_6005d9b8 != '\0') {
      aud_module_sync(param_1,param_2);
      return 0;
    }
  }
  return 0;
}


/* @60072568 aud_module_push_data__blk_60072568 */

undefined4 aud_module_push_data__blk_60072568(void)

{
  return 0;
}


/* @600725b0 aud_module_push_data__blk_600725b0 */

undefined4
aud_module_push_data__blk_600725b0(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  aud_object_put(param_3);
  return 0;
}


/* @600725c9 aud_module_push_data__blk_600725c9 */

undefined4
aud_module_push_data__blk_600725c9(undefined4 param_1,undefined4 param_2,undefined4 param_3)

{
  aud_object_put(param_3);
  return 0;
}


/* @600759cc aud_module_set_prop */

undefined4 aud_module_set_prop(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined *__delim;
  int iVar1;
  char *__s;
  char *__s2;
  char *pcVar2;
  int iVar3;
  undefined4 *puVar4;
  char *pcVar5;
  char *apcStack_30 [12];
  
  iVar1 = (*(code *)PTR_aud_module_get_prop_6005dc20)(param_2,param_3);
  __s = (char *)_BufMonitor_RemoveBufferIdxLocal(iVar1 + 1);
  if (__s == (char *)0x0) {
    flix(0xb0b0280,0xd001041);
    return 0;
  }
  BufMonitor_AddBufferRef(__s,param_2,iVar1 + 1);
  __delim = PTR_DAT_6005dc0c;
  __s[iVar1] = '\0';
  __s2 = strtok_r(__s,__delim,apcStack_30);
  do {
    if (__s2 == (char *)0x0) {
LAB_60075aa4:
      _BufMonitor_RemoveBufferIndex(__s);
      return 0;
    }
    pcVar2 = strtok_r((char *)0x0,PTR_DAT_6005dc0c,apcStack_30);
    if (pcVar2 == (char *)0x0) {
      if ((*DAT_6005d958 & 1) != 0) {
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_6005d960,0,PTR_s_aud_module_set_prop_6005dc08,DAT_6005dc10,
                     PTR_s_no_data_for_data__str__s_6005dc14,__s);
      }
      goto LAB_60075aa4;
    }
    puVar4 = *(undefined4 **)(*(int *)(param_1 + 0xc) + 0x30);
    if (puVar4 != (undefined4 *)0x0) {
      pcVar5 = (char *)*puVar4;
      while (pcVar5 != (char *)0x0) {
        iVar1 = strcmp(pcVar5,__s2);
        if (iVar1 == 0) {
          if (((puVar4[2] != 0) || (puVar4[4] != 0)) && ((puVar4[3] & 1) == 0)) {
            iVar1 = set_prop(param_1,puVar4,pcVar2,0);
            goto LAB_60075a27;
          }
        }
        puVar4 = puVar4 + 10;
        if (puVar4 == (undefined4 *)0x0) break;
        pcVar5 = (char *)*puVar4;
      }
    }
    iVar1 = -1;
LAB_60075a27:
    if (iVar1 < 0) {
      pcVar5 = *(char **)PTR_PTR_6005dc18;
      puVar4 = (undefined4 *)PTR_PTR_6005dc18;
      while (pcVar5 != (char *)0x0) {
        iVar1 = strcmp(pcVar5,__s2);
        if (iVar1 == 0) {
          if (((puVar4[2] != 0) || (puVar4[4] != 0)) && ((puVar4[3] & 1) == 0)) {
            iVar1 = set_prop(param_1,puVar4,pcVar2,0);
            goto LAB_60075a2a;
          }
        }
        puVar4 = puVar4 + 10;
        if (puVar4 == (undefined4 *)0x0) break;
        pcVar5 = (char *)*puVar4;
      }
      iVar1 = -1;
    }
LAB_60075a2a:
    if ((iVar1 < 0) && (iVar1 = *(int *)(*(int *)(param_1 + 0xc) + 0x38), iVar1 != 0)) {
      flix(0xe0b087b,0xd3ff541);
      while( true ) {
        iVar3 = strcmp(*(char **)(iVar1 + -0x20),__s2);
        if (iVar3 == 0) break;
        iVar1 = *(int *)(iVar1 + 4);
        flix(0xb0b08e7,0xd3ff041);
      }
      if ((*(int *)(iVar1 + -0x18) != 0) || (*(int *)(iVar1 + -0x10) != 0)) {
        flix(0xb0280,0xd3fef41);
        set_prop(param_1,(undefined4 *)(iVar1 + -0x20),pcVar2,0);
      }
    }
    __s2 = strtok_r((char *)0x0,PTR_DAT_6005dc0c,apcStack_30);
  } while( true );
}


/* @60075b03 aud_module_set_prop__blk_60075b03 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void aud_module_set_prop__blk_60075b03(void)

{
  undefined4 in_a7;
  
                    /* WARNING: Subroutine does not return */
  logmm_printl(in_a7,0);
}


/* @60075b15 aud_module_set_prop__blk_60075b15 */

undefined4 aud_module_set_prop__blk_60075b15(void)

{
  return 0;
}


/* @60075b74 aud_module_get_prop */

undefined8 aud_module_get_prop(undefined4 *param_1,char *param_2)

{
  undefined4 *puVar1;
  uint *puVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  undefined4 *puVar7;
  int iVar8;
  char *pcVar9;
  uint *puVar10;
  
  iVar8 = param_1[3];
  puVar7 = *(undefined4 **)(iVar8 + 0x30);
  if (puVar7 != (undefined4 *)0x0) {
    pcVar9 = (char *)*puVar7;
    while (pcVar9 != (char *)0x0) {
      iVar3 = strcmp(pcVar9,param_2);
      if (iVar3 == 0) goto LAB_60075b95;
      puVar1 = puVar7 + 10;
      puVar7 = puVar7 + 10;
      pcVar9 = (char *)*puVar1;
    }
  }
  puVar7 = (undefined4 *)0x0;
LAB_60075b95:
  puVar2 = DAT_6005d958;
  if (puVar7 == (undefined4 *)0x0) {
    pcVar9 = *(char **)PTR_PTR_6005dc18;
    puVar7 = (undefined4 *)PTR_PTR_6005dc18;
    while (pcVar9 != (char *)0x0) {
      iVar3 = strcmp(pcVar9,param_2);
      if (iVar3 == 0) goto LAB_60075bb8;
      puVar1 = puVar7 + 10;
      puVar7 = puVar7 + 10;
      pcVar9 = (char *)*puVar1;
    }
    puVar7 = (undefined4 *)0x0;
LAB_60075bb8:
    if (puVar7 == (undefined4 *)0x0) {
      iVar3 = *(int *)(iVar8 + 0x38);
      if (iVar3 == 0) {
        return 0xffffffffffffffff;
      }
      do {
        if (iVar8 + 0x34 == iVar3) {
LAB_60075be0:
          flix(0x60b00a0,0xd000a41);
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_6005d960,0,PTR_s_aud_module_get_prop_6005dc24,DAT_6005dc28,
                       PTR_s_unknown_prop___s_6005dc2c,param_2);
        }
        puVar7 = (undefined4 *)(iVar3 + -0x20);
        iVar4 = strcmp((char *)*puVar7,param_2);
        if (iVar4 == 0) {
          if (puVar7 != (undefined4 *)0x0) break;
          goto LAB_60075be0;
        }
        iVar3 = *(int *)(iVar3 + 4);
      } while( true );
    }
  }
  if ((code *)puVar7[6] == (code *)0x0) {
    if (puVar7[5] != 0) {
      if ((*puVar2 & 1) != 0) {
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_6005d960,0,PTR_s_aud_module_get_prop_6005dc24,DAT_6005dc38,
                     PTR_s_gets___not_supported___s_at__s_6005dc3c,param_2,*param_1);
      }
      return 0xffffffffffffffff;
    }
    iVar3 = puVar7[2];
    iVar8 = puVar7[1];
    puVar10 = (uint *)(iVar3 + (int)param_1);
    if (iVar8 == 3) {
      uVar6 = puVar10[1];
      uVar5 = *puVar10;
      if ((*puVar2 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_6005d960,3,PTR_s_aud_module_get_prop_6005dc24,DAT_6005dc40,
                     PTR_s__s_offset__d____lld_6005dc44,*puVar7,iVar3);
      }
    }
    else if (iVar8 == 2) {
      uVar5 = *puVar10;
      uVar6 = (int)uVar5 >> 0x1f;
      if ((*puVar2 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_6005d960,3,PTR_s_aud_module_get_prop_6005dc24,DAT_6005dc48,
                     PTR_s__s_offset__d____lld_6005dc44,*puVar7,iVar3);
      }
    }
    else {
      if (iVar8 != 1) {
        if ((iVar8 == 4) && ((*puVar2 & 8) != 0)) {
                    /* WARNING: Subroutine does not return */
          logmm_printl(PTR_DAT_6005d960,3,PTR_s_aud_module_get_prop_6005dc24,DAT_6005dc50,
                       PTR_s__s_offset__d____s_6005dc54,*puVar7,iVar3,*puVar10);
        }
        return 0;
      }
      uVar5 = (uint)(byte)*puVar10;
      uVar6 = 0;
      if ((*puVar2 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
        logmm_printl(PTR_DAT_6005d960,3,PTR_s_aud_module_get_prop_6005dc24,DAT_6005dc4c,
                     PTR_s__s_offset__d____lld_6005dc44,*puVar7,iVar3);
      }
    }
  }
  else {
    uVar5 = (*(code *)puVar7[6])(param_1,puVar7);
    uVar6 = (int)uVar5 >> 0x1f;
    if ((*puVar2 & 8) != 0) {
                    /* WARNING: Subroutine does not return */
      logmm_printl(PTR_DAT_6005d960,3,PTR_s_aud_module_get_prop_6005dc24,DAT_6005dc30,
                   PTR_s__s___lld_60043b70_0x20_6005dc34,*puVar7,uVar5,uVar6);
    }
  }
  return CONCAT44(uVar6,uVar5);
}


/* @60075c00 aud_module_get_prop__blk_60075c00 */

undefined8 aud_module_get_prop__blk_60075c00(void)

{
  return 0xffffffffffffffff;
}


/* @60075c3c aud_module_get_prop__blk_60075c3c */

void aud_module_get_prop__blk_60075c3c(void)

{
  return;
}


/* @60075c60 aud_module_get_prop__blk_60075c60 */

undefined8 aud_module_get_prop__blk_60075c60(void)

{
  return 0xffffffffffffffff;
}


/* @60075cd1 aud_module_get_prop__blk_60075cd1 */

void aud_module_get_prop__blk_60075cd1(void)

{
  return;
}


/* @60075cf9 aud_module_get_prop__blk_60075cf9 */

void aud_module_get_prop__blk_60075cf9(void)

{
  return;
}


/* @60075d1c aud_module_get_prop__blk_60075d1c */

undefined8 aud_module_get_prop__blk_60075d1c(void)

{
  return 0;
}


/* @600fba98 aud_module_get_prop */

int aud_module_get_prop(char *param_1,int param_2)

{
  char cVar1;
  char *pcVar2;
  
  param_2 = param_2 + -1;
  pcVar2 = param_1;
  if (param_2 != -1) {
    cVar1 = *param_1;
    while (cVar1 != '\0') {
      pcVar2 = pcVar2 + 1;
      param_2 = param_2 + -1;
      if (param_2 == -1) break;
      cVar1 = *pcVar2;
    }
  }
  return (int)pcVar2 - (int)param_1;
}


