/* LOCAL STATIC EVIDENCE: Ghidra pseudocode, not original source. SHA256 170d468085255d381245693260840771b00d39c718473ec024566fb1df1ca3af image base 00010000 */

/* @000d15d8 _ZN13umiaudiomixer11setSettingsENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueEPFbP8LSHandleP9LSMessagePvEP8envelope */

int _ZN13umiaudiomixer11setSettingsENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueEPFbP8LSHandleP9LSMessagePvEP8envelope
              (undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
              undefined4 param_5)

{
  undefined4 uVar1;
  int iVar2;
  int *piVar3;
  undefined4 uVar4;
  undefined1 *puVar5;
  undefined1 *puVar6;
  bool bVar7;
  undefined1 auStack_74 [4];
  undefined4 uStack_70;
  int iStack_6c;
  int *piStack_68;
  undefined4 uStack_64;
  int aiStack_60 [4];
  undefined1 auStack_50 [12];
  undefined1 auStack_44 [12];
  undefined1 auStack_38 [12];
  undefined1 auStack_2c [12];
  
  bVar7 = 0xfffffffe < uRam0055a628;
  uRam0055a628 = uRam0055a628 + 1;
  iRam0055a62c = iRam0055a62c + (uint)bVar7;
  _ZN13umiaudiomixer19mapSoundOutputToUmiERN7pbnjson6JValueE(param_1,param_3);
  bVar7 = 0xfffffffe < uRam0055a630;
  uRam0055a630 = uRam0055a630 + 1;
  iRam0055a634 = iRam0055a634 + (uint)bVar7;
  bVar7 = 0xfffffffe < uRam00559190;
  uRam00559190 = uRam00559190 + 1;
  iRam00559194 = iRam00559194 + (uint)bVar7;
  func_0x0009183c(auStack_50,&UNK_004a201c);
  bVar7 = 0xfffffffe < uRam00559198;
  uRam00559198 = uRam00559198 + 1;
  iRam0055919c = iRam0055919c + (uint)bVar7;
  func_0x000921c0(auStack_44,param_2);
  bVar7 = 0xfffffffe < uRam005591a8;
  uRam005591a8 = uRam005591a8 + 1;
  iRam005591ac = iRam005591ac + (uint)bVar7;
  bVar7 = 0xfffffffe < uRam0055a638;
  uRam0055a638 = uRam0055a638 + 1;
  iRam0055a63c = iRam0055a63c + (uint)bVar7;
  bVar7 = 0xfffffffe < uRam0055a790;
  uRam0055a790 = uRam0055a790 + 1;
  iRam0055a794 = iRam0055a794 + (uint)bVar7;
  func_0x0009183c(auStack_38,&UNK_004b5090);
  bVar7 = 0xfffffffe < uRam0055a798;
  uRam0055a798 = uRam0055a798 + 1;
  iRam0055a79c = iRam0055a79c + (uint)bVar7;
  func_0x000923ac(auStack_2c,param_3);
  bVar7 = 0xfffffffe < uRam0055a7a8;
  uRam0055a7a8 = uRam0055a7a8 + 1;
  iRam0055a7ac = iRam0055a7ac + (uint)bVar7;
  bVar7 = 0xfffffffe < uRam0055a640;
  uRam0055a640 = uRam0055a640 + 1;
  iRam0055a644 = iRam0055a644 + (uint)bVar7;
  func_0x000b0db4(&piStack_68,auStack_50,2);
  bVar7 = 0xfffffffe < uRam0055a648;
  uRam0055a648 = uRam0055a648 + 1;
  iRam0055a64c = iRam0055a64c + (uint)bVar7;
  piStack_68 = (int *)(_ZTVN7pbnjson7JObjectE + 8);
  func_0x0009156c(auStack_74);
  iVar2 = aiStack_60[0];
  uVar1 = uStack_70;
  piStack_68 = (int *)(_ZTVN7pbnjson7JObjectE + 8);
  uStack_70 = uStack_64;
  uStack_64 = uVar1;
  bVar7 = 0xfffffffe < uRam0055a650;
  uRam0055a650 = uRam0055a650 + 1;
  iRam0055a654 = iRam0055a654 + (uint)bVar7;
  aiStack_60[0] = iStack_6c;
  iStack_6c = iVar2;
  func_0x00091434(&piStack_68);
  bVar7 = 0xfffffffe < uRam0055a660;
  uRam0055a660 = uRam0055a660 + 1;
  iRam0055a664 = iRam0055a664 + (uint)bVar7;
  puVar5 = auStack_2c + 0xc;
  do {
    puVar6 = puVar5 + -0x18;
    bVar7 = 0xfffffffe < uRam0055a670;
    uRam0055a670 = uRam0055a670 + 1;
    iRam0055a674 = iRam0055a674 + (uint)bVar7;
    func_0x00091ba8(puVar5 + -0xc);
    bVar7 = 0xfffffffe < uRam0055a678;
    uRam0055a678 = uRam0055a678 + 1;
    iRam0055a67c = iRam0055a67c + (uint)bVar7;
    func_0x00091ba8(puVar6);
    puVar5 = puVar6;
  } while (puVar6 != auStack_50);
  bVar7 = 0xfffffffe < uRam0055a668;
  uRam0055a668 = uRam0055a668 + 1;
  iRam0055a66c = iRam0055a66c + (uint)bVar7;
  func_0x00091830(auStack_50);
  bVar7 = 0xfffffffe < uRam0055a680;
  uRam0055a680 = uRam0055a680 + 1;
  iRam0055a684 = iRam0055a684 + (uint)bVar7;
  uVar1 = _Z14GetPalmServicev();
  bVar7 = 0xfffffffe < uRam0055a698;
  uRam0055a698 = uRam0055a698 + 1;
  iRam0055a69c = iRam0055a69c + (uint)bVar7;
  iVar2 = _Z15getPmLogContextv();
  if (iVar2 == 0) {
    bVar7 = 0xfffffffe < uRam0055a6a8;
    uRam0055a6a8 = uRam0055a6a8 + 1;
    iRam0055a6ac = iRam0055a6ac + (uint)bVar7;
  }
  else {
    bVar7 = 0xfffffffe < uRam0055a6b0;
    uRam0055a6b0 = uRam0055a6b0 + 1;
    iRam0055a6b4 = iRam0055a6b4 + (uint)bVar7;
    piVar3 = (int *)_Z15getPmLogContextv();
    bVar7 = 0xfffffffe < uRam0055a6c0;
    uRam0055a6c0 = uRam0055a6c0 + 1;
    iRam0055a6c4 = iRam0055a6c4 + (uint)bVar7;
    if (*piVar3 < 6) goto LAB_000d1880;
    bVar7 = 0xfffffffe < uRam0055a6c8;
    uRam0055a6c8 = uRam0055a6c8 + 1;
    iRam0055a6cc = iRam0055a6cc + (uint)bVar7;
  }
  uVar4 = _Z15getPmLogContextv();
  bVar7 = 0xfffffffe < uRam0055a6d8;
  uRam0055a6d8 = uRam0055a6d8 + 1;
  iRam0055a6dc = iRam0055a6dc + (uint)bVar7;
  bVar7 = 0xfffffffe < uRam00558958;
  uRam00558958 = uRam00558958 + 1;
  iRam0055895c = iRam0055895c + (uint)bVar7;
  func_0x00091470(&piStack_68,auStack_74,0);
  bVar7 = 0xfffffffe < uRam0055a6e8;
  uRam0055a6e8 = uRam0055a6e8 + 1;
  iRam0055a6ec = iRam0055a6ec + (uint)bVar7;
  func_0x00091a40(uVar4,6,0,&UNK_004a1c04,0,0,0,&UNK_004a2590,&UNK_004a1c62,0x120,piStack_68);
  bVar7 = 0xfffffffe < uRam0055a6f8;
  uRam0055a6f8 = uRam0055a6f8 + 1;
  iRam0055a6fc = iRam0055a6fc + (uint)bVar7;
  if (piStack_68 != aiStack_60) {
    func_0x00091788(piStack_68,aiStack_60[0] + 1);
  }
  bVar7 = 0xfffffffe < uRam0055a700;
  uRam0055a700 = uRam0055a700 + 1;
  iRam0055a704 = iRam0055a704 + (uint)bVar7;
LAB_000d1880:
  func_0x00091470(&piStack_68,auStack_74,0);
  bVar7 = 0xfffffffe < uRam0055a710;
  uRam0055a710 = uRam0055a710 + 1;
  iRam0055a714 = iRam0055a714 + (uint)bVar7;
  iVar2 = func_0x00091b6c(uVar1,&UNK_004a25bc,piStack_68,param_4,param_5,0,auStack_50);
  bVar7 = 0xfffffffe < uRam0055a720;
  uRam0055a720 = uRam0055a720 + 1;
  iRam0055a724 = iRam0055a724 + (uint)bVar7;
  if (piStack_68 != aiStack_60) {
    func_0x00091788(piStack_68,aiStack_60[0] + 1);
  }
  bVar7 = 0xfffffffe < uRam0055a728;
  uRam0055a728 = uRam0055a728 + 1;
  iRam0055a72c = iRam0055a72c + (uint)bVar7;
  if (iVar2 == 0) {
    bVar7 = 0xfffffffe < uRam0055a730;
    uRam0055a730 = uRam0055a730 + 1;
    iRam0055a734 = iRam0055a734 + (uint)bVar7;
    _ZN8CLSError5PrintEPKci14GLogLevelFlags(auStack_50,&UNK_004a25f8,0x125,0x10);
    bVar7 = 0xfffffffe < uRam0055a740;
    uRam0055a740 = uRam0055a740 + 1;
    iRam0055a744 = iRam0055a744 + (uint)bVar7;
  }
  func_0x00091ba8(auStack_74);
  bVar7 = 0xfffffffe < uRam0055a748;
  uRam0055a748 = uRam0055a748 + 1;
  iRam0055a74c = iRam0055a74c + (uint)bVar7;
  return iVar2;
}



/* @001987b4 _ZN9TVManager18setSoundOutputImplERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEb */

void _ZN9TVManager18setSoundOutputImplERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEb
               (int param_1,undefined4 *param_2,int param_3)

{
  code ****ppppcVar1;
  int iVar2;
  int *piVar3;
  undefined4 uVar4;
  undefined4 uVar5;
  uint uVar6;
  int extraout_r3;
  code *****pppppcVar7;
  undefined1 *puVar8;
  int iVar9;
  code *****pppppcVar10;
  bool bVar11;
  undefined1 auStack_e8 [4];
  undefined4 uStack_e4;
  undefined4 uStack_e0;
  undefined4 uStack_dc;
  undefined4 uStack_d8;
  undefined4 uStack_d4;
  code *****pppppcStack_d0;
  undefined4 uStack_cc;
  undefined4 uStack_c8;
  code *****pppppcStack_c4;
  code *****pppppcStack_c0;
  code *****apppppcStack_bc [4];
  code ****ppppcStack_ac;
  undefined4 uStack_a8;
  code ****appppcStack_a4 [4];
  code *****pppppcStack_94;
  undefined4 uStack_90;
  undefined1 auStack_88 [12];
  code *****pppppcStack_7c;
  undefined4 uStack_78;
  code ****ppppcStack_74;
  undefined4 uStack_70;
  undefined4 uStack_6c;
  undefined2 uStack_68;
  undefined1 uStack_66;
  code *pcStack_60;
  code *****pppppcStack_58;
  code *****pppppcStack_54;
  code *****pppppcStack_50;
  undefined *apuStack_4c [2];
  undefined1 auStack_44 [4];
  undefined1 auStack_40 [4];
  code *pcStack_3c;
  undefined1 auStack_34 [4];
  undefined4 uStack_30;
  undefined4 uStack_2c;
  code ****ppppcStack_28;
  
  bVar11 = 0xfffffffe < uRam00681cb0;
  uRam00681cb0 = uRam00681cb0 + 1;
  iRam00681cb4 = iRam00681cb4 + (uint)bVar11;
  if (param_3 == 0) {
    bVar11 = 0xfffffffe < uRam00681cb8;
    uRam00681cb8 = uRam00681cb8 + 1;
    iRam00681cbc = iRam00681cbc + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam006826d8;
    uRam006826d8 = uRam006826d8 + 1;
    iRam006826dc = iRam006826dc + (uint)bVar11;
    func_0x0009183c(&pppppcStack_58,&UNK_004a236c);
    bVar11 = 0xfffffffe < uRam006826e0;
    uRam006826e0 = uRam006826e0 + 1;
    iRam006826e4 = iRam006826e4 + (uint)bVar11;
    func_0x000921c0(apuStack_4c,param_2);
    bVar11 = 0xfffffffe < uRam006826f0;
    uRam006826f0 = uRam006826f0 + 1;
    iRam006826f4 = iRam006826f4 + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam00681cc0;
    uRam00681cc0 = uRam00681cc0 + 1;
    iRam00681cc4 = iRam00681cc4 + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam00682700;
    uRam00682700 = uRam00682700 + 1;
    iRam00682704 = iRam00682704 + (uint)bVar11;
    func_0x0009183c(auStack_40,&UNK_004ae24c);
    bVar11 = 0xfffffffe < uRam00682708;
    uRam00682708 = uRam00682708 + 1;
    iRam0068270c = iRam0068270c + (uint)bVar11;
    func_0x000921c0(auStack_34,param_1 + 0x2d0);
    uStack_90 = 2;
    bVar11 = 0xfffffffe < uRam00682718;
    uRam00682718 = uRam00682718 + 1;
    iRam0068271c = iRam0068271c + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam00681cc8;
    uRam00681cc8 = uRam00681cc8 + 1;
    iRam00681ccc = iRam00681ccc + (uint)bVar11;
    pppppcStack_94 = (code *****)&pppppcStack_58;
    func_0x000b0db4(&pppppcStack_7c,&pppppcStack_58,2);
    bVar11 = 0xfffffffe < uRam00681cd0;
    uRam00681cd0 = uRam00681cd0 + 1;
    iRam00681cd4 = iRam00681cd4 + (uint)bVar11;
    pppppcStack_7c = (code *****)(_ZTVN7pbnjson7JObjectE + 8);
    func_0x0009156c(&ppppcStack_ac);
    ppppcVar1 = ppppcStack_74;
    uVar4 = uStack_78;
    pppppcStack_7c = (code *****)(_ZTVN7pbnjson7JObjectE + 8);
    uStack_78 = uStack_a8;
    uStack_a8 = uVar4;
    bVar11 = 0xfffffffe < uRam00681cd8;
    uRam00681cd8 = uRam00681cd8 + 1;
    iRam00681cdc = iRam00681cdc + (uint)bVar11;
    ppppcStack_74 = appppcStack_a4[0];
    appppcStack_a4[0] = ppppcVar1;
    func_0x00091434(&pppppcStack_7c);
    bVar11 = 0xfffffffe < uRam00681ce8;
    uRam00681ce8 = uRam00681ce8 + 1;
    iRam00681cec = iRam00681cec + (uint)bVar11;
    pppppcVar7 = &ppppcStack_28;
    do {
      pppppcVar10 = pppppcVar7 + -6;
      bVar11 = 0xfffffffe < uRam00681cf8;
      uRam00681cf8 = uRam00681cf8 + 1;
      iRam00681cfc = iRam00681cfc + (uint)bVar11;
      func_0x00091ba8(pppppcVar7 + -3);
      bVar11 = 0xfffffffe < uRam00681d00;
      uRam00681d00 = uRam00681d00 + 1;
      iRam00681d04 = iRam00681d04 + (uint)bVar11;
      func_0x00091ba8(pppppcVar10);
      pppppcVar7 = pppppcVar10;
    } while ((code ******)pppppcVar10 != &pppppcStack_58);
    bVar11 = 0xfffffffe < uRam00681cf0;
    uRam00681cf0 = uRam00681cf0 + 1;
    iRam00681cf4 = iRam00681cf4 + (uint)bVar11;
    func_0x000923ac(&pppppcStack_94,&ppppcStack_ac);
    bVar11 = 0xfffffffe < uRam00681d10;
    uRam00681d10 = uRam00681d10 + 1;
    iRam00681d14 = iRam00681d14 + (uint)bVar11;
    pppppcStack_58 = (code *****)&UNK_0019c175;
    pppppcStack_54 = (code *****)0x0;
    apuStack_4c[0] = &DAT_001de039;
    pppppcStack_50 = (code *****)&DAT_001de145;
    _ZN5utils19makeSoundOutputTaskEN7pbnjson6JValueESt8functionIFvS1_P8envelopeEES4_
              (&pppppcStack_7c,&pppppcStack_94,&pppppcStack_58);
    bVar11 = 0xfffffffe < uRam00681d20;
    uRam00681d20 = uRam00681d20 + 1;
    iRam00681d24 = iRam00681d24 + (uint)bVar11;
    if (pppppcStack_50 == (code *****)0x0) {
      bVar11 = 0xfffffffe < uRam00681168;
      uRam00681168 = uRam00681168 + 1;
      iRam0068116c = iRam0068116c + (uint)bVar11;
    }
    else {
      bVar11 = 0xfffffffe < uRam00681160;
      uRam00681160 = uRam00681160 + 1;
      iRam00681164 = iRam00681164 + (uint)bVar11;
      (*(code *)pppppcStack_50)(&pppppcStack_58,&pppppcStack_58,3);
      bVar11 = 0xfffffffe < uRam00681170;
      uRam00681170 = uRam00681170 + 1;
      iRam00681174 = iRam00681174 + (uint)bVar11;
    }
    bVar11 = 0xfffffffe < uRam00681d28;
    uRam00681d28 = uRam00681d28 + 1;
    iRam00681d2c = iRam00681d2c + (uint)bVar11;
    func_0x00091ba8(&pppppcStack_94);
    bVar11 = 0xfffffffe < uRam00681d30;
    uRam00681d30 = uRam00681d30 + 1;
    iRam00681d34 = iRam00681d34 + (uint)bVar11;
    iVar2 = _ZN5utils15SoundOutputTask7isEmptyEv(&pppppcStack_94);
    if (iVar2 == 0) {
      bVar11 = 0xfffffffe < uRam00681d48;
      uRam00681d48 = uRam00681d48 + 1;
      iRam00681d4c = iRam00681d4c + (uint)bVar11;
      func_0x000b476c(&pppppcStack_58,&pppppcStack_7c);
      bVar11 = 0xfffffffe < uRam00681dd8;
      uRam00681dd8 = uRam00681dd8 + 1;
      iRam00681ddc = iRam00681ddc + (uint)bVar11;
      _ZN5utils15SoundOutputTask7addTaskENS_4taskE(&pppppcStack_94,&pppppcStack_58);
      bVar11 = 0xfffffffe < uRam00681de8;
      uRam00681de8 = uRam00681de8 + 1;
      iRam00681dec = iRam00681dec + (uint)bVar11;
      if (pcStack_3c == (code *)0x0) {
        bVar11 = 0xfffffffe < uRam00681168;
        uRam00681168 = uRam00681168 + 1;
        iRam0068116c = iRam0068116c + (uint)bVar11;
      }
      else {
        bVar11 = 0xfffffffe < uRam00681160;
        uRam00681160 = uRam00681160 + 1;
        iRam00681164 = iRam00681164 + (uint)bVar11;
        (*pcStack_3c)(auStack_44,auStack_44,3);
        bVar11 = 0xfffffffe < uRam00681170;
        uRam00681170 = uRam00681170 + 1;
        iRam00681174 = iRam00681174 + (uint)bVar11;
      }
      bVar11 = 0xfffffffe < uRam00681df0;
      uRam00681df0 = uRam00681df0 + 1;
      iRam00681df4 = iRam00681df4 + (uint)bVar11;
      func_0x00091ba8(&pppppcStack_54);
      bVar11 = 0xfffffffe < uRam00681df8;
      uRam00681df8 = uRam00681df8 + 1;
      iRam00681dfc = iRam00681dfc + (uint)bVar11;
      iVar2 = _Z15getPmLogContextv();
      if (iVar2 == 0) {
        bVar11 = 0xfffffffe < uRam00681e08;
        uRam00681e08 = uRam00681e08 + 1;
        iRam00681e0c = iRam00681e0c + (uint)bVar11;
      }
      else {
        bVar11 = 0xfffffffe < uRam00681e10;
        uRam00681e10 = uRam00681e10 + 1;
        iRam00681e14 = iRam00681e14 + (uint)bVar11;
        piVar3 = (int *)_Z15getPmLogContextv();
        bVar11 = 0xfffffffe < uRam00681e20;
        uRam00681e20 = uRam00681e20 + 1;
        iRam00681e24 = iRam00681e24 + (uint)bVar11;
        if (*piVar3 < 6) goto LAB_0019943e;
        bVar11 = 0xfffffffe < uRam00681e28;
        uRam00681e28 = uRam00681e28 + 1;
        iRam00681e2c = iRam00681e2c + (uint)bVar11;
      }
      uVar4 = _Z15getPmLogContextv();
      bVar11 = 0xfffffffe < uRam00681e38;
      uRam00681e38 = uRam00681e38 + 1;
      iRam00681e3c = iRam00681e3c + (uint)bVar11;
      uVar5 = func_0x00193610();
      func_0x00091a40(uVar4,6,0,&UNK_0049ed3c,0,0,0,&UNK_004ae298,uVar5,0x7b3);
      bVar11 = 0xfffffffe < uRam00681e48;
      uRam00681e48 = uRam00681e48 + 1;
      iRam00681e4c = iRam00681e4c + (uint)bVar11;
LAB_0019943e:
      if (pcStack_60 == (code *)0x0) {
        bVar11 = 0xfffffffe < uRam00681168;
        uRam00681168 = uRam00681168 + 1;
        iRam0068116c = iRam0068116c + (uint)bVar11;
      }
      else {
        bVar11 = 0xfffffffe < uRam00681160;
        uRam00681160 = uRam00681160 + 1;
        iRam00681164 = iRam00681164 + (uint)bVar11;
        (*pcStack_60)(&uStack_68,&uStack_68,3);
        bVar11 = 0xfffffffe < uRam00681170;
        uRam00681170 = uRam00681170 + 1;
        iRam00681174 = iRam00681174 + (uint)bVar11;
      }
      bVar11 = 0xfffffffe < uRam00681e50;
      uRam00681e50 = uRam00681e50 + 1;
      iRam00681e54 = iRam00681e54 + (uint)bVar11;
      func_0x00091ba8(&uStack_78);
      bVar11 = 0xfffffffe < uRam00681e58;
      uRam00681e58 = uRam00681e58 + 1;
      iRam00681e5c = iRam00681e5c + (uint)bVar11;
      func_0x00091ba8(&ppppcStack_ac);
      bVar11 = 0xfffffffe < uRam00681e60;
      uRam00681e60 = uRam00681e60 + 1;
      iRam00681e64 = iRam00681e64 + (uint)bVar11;
      return;
    }
    bVar11 = 0xfffffffe < uRam00681d40;
    uRam00681d40 = uRam00681d40 + 1;
    iRam00681d44 = iRam00681d44 + (uint)bVar11;
    pppppcStack_7c = (code *****)0x1;
    func_0x000b476c(&pppppcStack_58,&pppppcStack_7c);
    bVar11 = 0xfffffffe < uRam00681d58;
    uRam00681d58 = uRam00681d58 + 1;
    iRam00681d5c = iRam00681d5c + (uint)bVar11;
    _ZN5utils15SoundOutputTask7addTaskENS_4taskE(&pppppcStack_94,&pppppcStack_58);
    bVar11 = 0xfffffffe < uRam00681d68;
    uRam00681d68 = uRam00681d68 + 1;
    iRam00681d6c = iRam00681d6c + (uint)bVar11;
    if (pcStack_3c == (code *)0x0) {
      bVar11 = 0xfffffffe < uRam00681168;
      uRam00681168 = uRam00681168 + 1;
      iRam0068116c = iRam0068116c + (uint)bVar11;
    }
    else {
      bVar11 = 0xfffffffe < uRam00681160;
      uRam00681160 = uRam00681160 + 1;
      iRam00681164 = iRam00681164 + (uint)bVar11;
      (*pcStack_3c)(auStack_44,auStack_44,3);
      bVar11 = 0xfffffffe < uRam00681170;
      uRam00681170 = uRam00681170 + 1;
      iRam00681174 = iRam00681174 + (uint)bVar11;
    }
    bVar11 = 0xfffffffe < uRam00681d70;
    uRam00681d70 = uRam00681d70 + 1;
    iRam00681d74 = iRam00681d74 + (uint)bVar11;
    func_0x00091ba8(&pppppcStack_54);
    bVar11 = 0xfffffffe < uRam00681d78;
    uRam00681d78 = uRam00681d78 + 1;
    iRam00681d7c = iRam00681d7c + (uint)bVar11;
    iVar2 = _Z15getPmLogContextv();
    if (iVar2 == 0) {
      bVar11 = 0xfffffffe < uRam00681d88;
      uRam00681d88 = uRam00681d88 + 1;
      iRam00681d8c = iRam00681d8c + (uint)bVar11;
LAB_00198c22:
      uVar4 = _Z15getPmLogContextv();
      bVar11 = 0xfffffffe < uRam00681db8;
      uRam00681db8 = uRam00681db8 + 1;
      iRam00681dbc = iRam00681dbc + (uint)bVar11;
      uVar5 = func_0x00193610();
      func_0x00091a40(uVar4,6,0,&UNK_0049ed3c,0,0,0,&UNK_004ae258,uVar5,0x7ad);
      bVar11 = 0xfffffffe < uRam00681dc8;
      uRam00681dc8 = uRam00681dc8 + 1;
      iRam00681dcc = iRam00681dcc + (uint)bVar11;
    }
    else {
      bVar11 = 0xfffffffe < uRam00681d90;
      uRam00681d90 = uRam00681d90 + 1;
      iRam00681d94 = iRam00681d94 + (uint)bVar11;
      piVar3 = (int *)_Z15getPmLogContextv();
      bVar11 = 0xfffffffe < uRam00681da0;
      uRam00681da0 = uRam00681da0 + 1;
      iRam00681da4 = iRam00681da4 + (uint)bVar11;
      if (5 < *piVar3) {
        bVar11 = 0xfffffffe < uRam00681da8;
        uRam00681da8 = uRam00681da8 + 1;
        iRam00681dac = iRam00681dac + (uint)bVar11;
        goto LAB_00198c22;
      }
    }
    if (pcStack_60 == (code *)0x0) {
      bVar11 = 0xfffffffe < uRam00681168;
      uRam00681168 = uRam00681168 + 1;
      iRam0068116c = iRam0068116c + (uint)bVar11;
    }
    else {
      bVar11 = 0xfffffffe < uRam00681160;
      uRam00681160 = uRam00681160 + 1;
      iRam00681164 = iRam00681164 + (uint)bVar11;
      (*pcStack_60)(&uStack_68,&uStack_68,3);
      bVar11 = 0xfffffffe < uRam00681170;
      uRam00681170 = uRam00681170 + 1;
      iRam00681174 = iRam00681174 + (uint)bVar11;
    }
    bVar11 = 0xfffffffe < uRam00681e68;
    uRam00681e68 = uRam00681e68 + 1;
    iRam00681e6c = iRam00681e6c + (uint)bVar11;
    func_0x00091ba8(&uStack_78);
    bVar11 = 0xfffffffe < uRam00681e70;
    uRam00681e70 = uRam00681e70 + 1;
    iRam00681e74 = iRam00681e74 + (uint)bVar11;
    func_0x00091ba8(&ppppcStack_ac);
    bVar11 = 0xfffffffe < uRam00681e78;
    uRam00681e78 = uRam00681e78 + 1;
    iRam00681e7c = iRam00681e7c + (uint)bVar11;
  }
  iVar2 = _Z15getPmLogContextv();
  if (iVar2 == 0) {
    bVar11 = 0xfffffffe < uRam00681e80;
    uRam00681e80 = uRam00681e80 + 1;
    iRam00681e84 = iRam00681e84 + (uint)bVar11;
LAB_00198d84:
    uVar4 = _Z15getPmLogContextv();
    bVar11 = 0xfffffffe < uRam00681ea0;
    uRam00681ea0 = uRam00681ea0 + 1;
    iRam00681ea4 = iRam00681ea4 + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam00680bb8;
    uRam00680bb8 = uRam00680bb8 + 1;
    iRam00680bbc = iRam00680bbc + (uint)bVar11;
    func_0x00091a40(uVar4,6,0,&UNK_0049ed3c,0,0,0,&UNK_004ae2f0,&UNK_004ade42,0x7b9,&UNK_004ae2dc,
                    *param_2,param_3);
    bVar11 = 0xfffffffe < uRam00681ea8;
    uRam00681ea8 = uRam00681ea8 + 1;
    iRam00681eac = iRam00681eac + (uint)bVar11;
  }
  else {
    bVar11 = 0xfffffffe < uRam00681e88;
    uRam00681e88 = uRam00681e88 + 1;
    iRam00681e8c = iRam00681e8c + (uint)bVar11;
    piVar3 = (int *)_Z15getPmLogContextv();
    bVar11 = 0xfffffffe < uRam00681e90;
    uRam00681e90 = uRam00681e90 + 1;
    iRam00681e94 = iRam00681e94 + (uint)bVar11;
    if (5 < *piVar3) {
      bVar11 = 0xfffffffe < uRam00681e98;
      uRam00681e98 = uRam00681e98 + 1;
      iRam00681e9c = iRam00681e9c + (uint)bVar11;
      goto LAB_00198d84;
    }
  }
  puVar8 = (undefined1 *)*param_2;
  pppppcVar7 = (code *****)param_2[1];
  bVar11 = 0xfffffffe < uRam00681410;
  uRam00681410 = uRam00681410 + 1;
  iRam00681414 = iRam00681414 + (uint)bVar11;
  pppppcStack_c4 = (code *****)apppppcStack_bc;
  if (puVar8 == (undefined1 *)0x0) {
    bVar11 = 0xfffffffe < uRam00681418;
    uRam00681418 = uRam00681418 + 1;
    iRam0068141c = iRam0068141c + (uint)bVar11;
    if (pppppcVar7 != (code *****)0x0) {
      bVar11 = 0xfffffffe < uRam00681420;
      uRam00681420 = uRam00681420 + 1;
      iRam00681424 = iRam00681424 + (uint)bVar11;
      func_0x000917ac(&UNK_0049d590);
      iVar2 = extraout_r3;
      goto LAB_0019af8a;
    }
  }
  else {
    pppppcStack_58 = pppppcVar7;
    if (pppppcVar7 < (code *****)0x10) {
      if (pppppcVar7 == (code *****)0x1) {
        apppppcStack_bc[0] = (code *****)CONCAT31(apppppcStack_bc[0]._1_3_,*puVar8);
      }
      else if (pppppcVar7 != (code *****)0x0) goto LAB_001992a4;
    }
    else {
      bVar11 = 0xfffffffe < uRam00681428;
      uRam00681428 = uRam00681428 + 1;
      iRam0068142c = iRam0068142c + (uint)bVar11;
      pppppcStack_c4 = (code *****)func_0x00092130(&pppppcStack_c4,&pppppcStack_58,0);
      bVar11 = 0xfffffffe < uRam00681430;
      uRam00681430 = uRam00681430 + 1;
      iRam00681434 = iRam00681434 + (uint)bVar11;
      apppppcStack_bc[0] = pppppcStack_58;
LAB_001992a4:
      func_0x00091ea8(pppppcStack_c4,puVar8,pppppcVar7);
      pppppcVar7 = pppppcStack_58;
    }
  }
  bVar11 = 0xfffffffe < uRam00681438;
  uRam00681438 = uRam00681438 + 1;
  iRam0068143c = iRam0068143c + (uint)bVar11;
  bVar11 = 0xfffffffe < uRam00681eb0;
  uRam00681eb0 = uRam00681eb0 + 1;
  iRam00681eb4 = iRam00681eb4 + (uint)bVar11;
  *(undefined1 *)((int)pppppcStack_c4 + (int)pppppcVar7) = 0;
  pppppcStack_c0 = pppppcVar7;
  iVar2 = func_0x000913cc(param_1 + 0x34c,&UNK_004cda3c);
  bVar11 = 0xfffffffe < uRam00681eb8;
  uRam00681eb8 = uRam00681eb8 + 1;
  iRam00681ebc = iRam00681ebc + (uint)bVar11;
  if (iVar2 == 0) {
    bVar11 = 0xfffffffe < uRam00681ec0;
    uRam00681ec0 = uRam00681ec0 + 1;
    iRam00681ec4 = iRam00681ec4 + (uint)bVar11;
    iVar2 = func_0x000913cc(param_2,&UNK_0049f550);
    bVar11 = 0xfffffffe < uRam00681ec8;
    uRam00681ec8 = uRam00681ec8 + 1;
    iRam00681ecc = iRam00681ecc + (uint)bVar11;
    if (iVar2 == 0) {
      bVar11 = 0xfffffffe < uRam00681ed0;
      uRam00681ed0 = uRam00681ed0 + 1;
      iRam00681ed4 = iRam00681ed4 + (uint)bVar11;
      iVar2 = _Z15getPmLogContextv();
      if (iVar2 == 0) {
        bVar11 = 0xfffffffe < uRam00681ee0;
        uRam00681ee0 = uRam00681ee0 + 1;
        iRam00681ee4 = iRam00681ee4 + (uint)bVar11;
LAB_0019a3b0:
        uVar4 = _Z15getPmLogContextv();
        bVar11 = 0xfffffffe < uRam00681f10;
        uRam00681f10 = uRam00681f10 + 1;
        iRam00681f14 = iRam00681f14 + (uint)bVar11;
        uVar5 = func_0x00193610();
        func_0x00091a40(uVar4,6,0,&UNK_0049ed3c,0,0,0,&UNK_004ae324,uVar5,0x7bf);
        bVar11 = 0xfffffffe < uRam00681f20;
        uRam00681f20 = uRam00681f20 + 1;
        iRam00681f24 = iRam00681f24 + (uint)bVar11;
      }
      else {
        bVar11 = 0xfffffffe < uRam00681ee8;
        uRam00681ee8 = uRam00681ee8 + 1;
        iRam00681eec = iRam00681eec + (uint)bVar11;
        piVar3 = (int *)_Z15getPmLogContextv();
        bVar11 = 0xfffffffe < uRam00681ef8;
        uRam00681ef8 = uRam00681ef8 + 1;
        iRam00681efc = iRam00681efc + (uint)bVar11;
        if (5 < *piVar3) {
          bVar11 = 0xfffffffe < uRam00681f00;
          uRam00681f00 = uRam00681f00 + 1;
          iRam00681f04 = iRam00681f04 + (uint)bVar11;
          goto LAB_0019a3b0;
        }
      }
      func_0x000914dc(&pppppcStack_c4,0,pppppcStack_c0,&UNK_004ae36c,0xb);
      bVar11 = 0xfffffffe < uRam00681f28;
      uRam00681f28 = uRam00681f28 + 1;
      iRam00681f2c = iRam00681f2c + (uint)bVar11;
    }
  }
  iVar2 = func_0x000913cc(param_2,&UNK_0049ef58);
  bVar11 = 0xfffffffe < uRam00681f38;
  uRam00681f38 = uRam00681f38 + 1;
  iRam00681f3c = iRam00681f3c + (uint)bVar11;
  if (iVar2 == 0) {
    bVar11 = 0xfffffffe < uRam00681f40;
    uRam00681f40 = uRam00681f40 + 1;
    iRam00681f44 = iRam00681f44 + (uint)bVar11;
    iVar2 = func_0x000913cc(param_1 + 0x364,&UNK_004ae378);
    bVar11 = 0xfffffffe < uRam00681f48;
    uRam00681f48 = uRam00681f48 + 1;
    iRam00681f4c = iRam00681f4c + (uint)bVar11;
    if (iVar2 == 0) {
      bVar11 = 0xfffffffe < uRam00681f50;
      uRam00681f50 = uRam00681f50 + 1;
      iRam00681f54 = iRam00681f54 + (uint)bVar11;
      iVar2 = _Z15getPmLogContextv();
      if (iVar2 == 0) {
        bVar11 = 0xfffffffe < uRam00681f60;
        uRam00681f60 = uRam00681f60 + 1;
        iRam00681f64 = iRam00681f64 + (uint)bVar11;
LAB_0019a2a0:
        uVar4 = _Z15getPmLogContextv();
        bVar11 = 0xfffffffe < uRam00681f90;
        uRam00681f90 = uRam00681f90 + 1;
        iRam00681f94 = iRam00681f94 + (uint)bVar11;
        uVar5 = func_0x00193610();
        func_0x00091a40(uVar4,6,0,&UNK_0049ed3c,0,0,0,&UNK_004ae388,uVar5,0x7c6);
        bVar11 = 0xfffffffe < uRam00681fa0;
        uRam00681fa0 = uRam00681fa0 + 1;
        iRam00681fa4 = iRam00681fa4 + (uint)bVar11;
      }
      else {
        bVar11 = 0xfffffffe < uRam00681f68;
        uRam00681f68 = uRam00681f68 + 1;
        iRam00681f6c = iRam00681f6c + (uint)bVar11;
        piVar3 = (int *)_Z15getPmLogContextv();
        bVar11 = 0xfffffffe < uRam00681f78;
        uRam00681f78 = uRam00681f78 + 1;
        iRam00681f7c = iRam00681f7c + (uint)bVar11;
        if (5 < *piVar3) {
          bVar11 = 0xfffffffe < uRam00681f80;
          uRam00681f80 = uRam00681f80 + 1;
          iRam00681f84 = iRam00681f84 + (uint)bVar11;
          goto LAB_0019a2a0;
        }
      }
      func_0x000914dc(&pppppcStack_c4,0,pppppcStack_c0,&UNK_0049ef70,0x16);
      bVar11 = 0xfffffffe < uRam00681fa8;
      uRam00681fa8 = uRam00681fa8 + 1;
      iRam00681fac = iRam00681fac + (uint)bVar11;
    }
  }
  iVar2 = _Z15getPmLogContextv();
  if (iVar2 == 0) {
    bVar11 = 0xfffffffe < uRam00681fc0;
    uRam00681fc0 = uRam00681fc0 + 1;
    iRam00681fc4 = iRam00681fc4 + (uint)bVar11;
LAB_00198fe2:
    uVar4 = _Z15getPmLogContextv();
    bVar11 = 0xfffffffe < uRam00681ff0;
    uRam00681ff0 = uRam00681ff0 + 1;
    iRam00681ff4 = iRam00681ff4 + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam00680bb8;
    uRam00680bb8 = uRam00680bb8 + 1;
    iRam00680bbc = iRam00680bbc + (uint)bVar11;
    uVar5 = _ZNK9TVManager14getOSSActivateEv(param_1);
    bVar11 = 0xfffffffe < uRam00682000;
    uRam00682000 = uRam00682000 + 1;
    iRam00682004 = iRam00682004 + (uint)bVar11;
    func_0x00091a40(uVar4,7,0,0,0,0,0,&UNK_004ae3dc,&UNK_004ade42,0x7cb,uVar5);
  }
  else {
    bVar11 = 0xfffffffe < uRam00681fc8;
    uRam00681fc8 = uRam00681fc8 + 1;
    iRam00681fcc = iRam00681fcc + (uint)bVar11;
    piVar3 = (int *)_Z15getPmLogContextv();
    bVar11 = 0xfffffffe < uRam00681fd8;
    uRam00681fd8 = uRam00681fd8 + 1;
    iRam00681fdc = iRam00681fdc + (uint)bVar11;
    if (6 < *piVar3) {
      bVar11 = 0xfffffffe < uRam00681fe0;
      uRam00681fe0 = uRam00681fe0 + 1;
      iRam00681fe4 = iRam00681fe4 + (uint)bVar11;
      goto LAB_00198fe2;
    }
  }
  pppppcVar10 = pppppcStack_c0;
  pppppcVar7 = pppppcStack_c4;
  iVar9 = *(int *)(param_1 + 0x2a8);
  uStack_a8 = 0;
  appppcStack_a4[0] = (code ****)((uint)appppcStack_a4[0] & 0xffffff00);
  ppppcStack_ac = (code ****)appppcStack_a4;
  if (iVar9 == 0) {
    bVar11 = 0xfffffffe < uRam00682018;
    uRam00682018 = uRam00682018 + 1;
    iRam0068201c = iRam0068201c + (uint)bVar11;
  }
  else {
    bVar11 = 0xfffffffe < uRam00682010;
    uRam00682010 = uRam00682010 + 1;
    iRam00682014 = iRam00682014 + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam00681410;
    uRam00681410 = uRam00681410 + 1;
    iRam00681414 = iRam00681414 + (uint)bVar11;
    iVar2 = 0x681418;
    pppppcStack_58 = (code *****)&pppppcStack_50;
    if (pppppcStack_c4 == (code *****)0x0) {
      bVar11 = 0xfffffffe < uRam00681418;
      uRam00681418 = uRam00681418 + 1;
      iRam0068141c = iRam0068141c + (uint)bVar11;
      pppppcVar7 = pppppcStack_c0;
      if (pppppcStack_c0 != (code *****)0x0) {
LAB_0019af8a:
        uVar6 = *(uint *)(iVar2 + 8);
        *(uint *)(iVar2 + 8) = uVar6 + 1;
        *(uint *)(iVar2 + 0xc) = *(int *)(iVar2 + 0xc) + (uint)(0xfffffffe < uVar6);
        func_0x000917ac(&UNK_0049d590);
        bVar11 = 0xfffffffe < uRam00681e18;
        uRam00681e18 = uRam00681e18 + 1;
        iRam00681e1c = iRam00681e1c + (uint)bVar11;
        func_0x000b0d4c(&uStack_68);
        bVar11 = 0xfffffffe < uRam00682620;
        uRam00682620 = uRam00682620 + 1;
        iRam00682624 = iRam00682624 + (uint)bVar11;
        func_0x00091ba8(&uStack_78);
        bVar11 = 0xfffffffe < uRam00682628;
        uRam00682628 = uRam00682628 + 1;
        iRam0068262c = iRam0068262c + (uint)bVar11;
        func_0x00091ba8(pppppcVar7);
        bVar11 = 0xfffffffe < uRam00682630;
        uRam00682630 = uRam00682630 + 1;
        iRam00682634 = iRam00682634 + (uint)bVar11;
        func_0x00091c20();
        bVar11 = 0xfffffffe < uRam006823c0;
        uRam006823c0 = uRam006823c0 + 1;
        iRam006823c4 = iRam006823c4 + (uint)bVar11;
        do {
          func_0x00091cf8(pppppcVar7);
          bVar11 = 0xfffffffe < uRam006826c8;
          uRam006826c8 = uRam006826c8 + 1;
          iRam006826cc = iRam006826cc + (uint)bVar11;
          func_0x00091cf8(&pppppcStack_c4);
          bVar11 = 0xfffffffe < uRam006826d0;
          uRam006826d0 = uRam006826d0 + 1;
          iRam006826d4 = iRam006826d4 + (uint)bVar11;
          func_0x00091c20();
          bVar11 = 0xfffffffe < uRam006824d8;
          uRam006824d8 = uRam006824d8 + 1;
          iRam006824dc = iRam006824dc + (uint)bVar11;
        } while( true );
      }
LAB_0019a1b2:
      pppppcStack_54 = pppppcStack_c0;
    }
    else {
      pppppcStack_7c = pppppcStack_c0;
      if (pppppcStack_c0 < (code *****)0x10) {
        if (pppppcStack_c0 == (code *****)0x1) {
          pppppcStack_50 = (code *****)CONCAT31(pppppcStack_50._1_3_,*(undefined1 *)pppppcStack_c4);
          pppppcStack_54 = pppppcStack_c0;
          goto LAB_001990f2;
        }
        if (pppppcStack_c0 == (code *****)0x0) goto LAB_0019a1b2;
      }
      else {
        bVar11 = 0xfffffffe < uRam00681428;
        uRam00681428 = uRam00681428 + 1;
        iRam0068142c = iRam0068142c + (uint)bVar11;
        pppppcStack_58 = (code *****)func_0x00092130(&pppppcStack_58,&pppppcStack_7c);
        bVar11 = 0xfffffffe < uRam00681430;
        uRam00681430 = uRam00681430 + 1;
        iRam00681434 = iRam00681434 + (uint)bVar11;
        pppppcStack_50 = pppppcStack_7c;
      }
      func_0x00091ea8(pppppcStack_58,pppppcVar7,pppppcVar10);
      pppppcStack_54 = pppppcStack_7c;
    }
LAB_001990f2:
    bVar11 = 0xfffffffe < uRam00681438;
    uRam00681438 = uRam00681438 + 1;
    iRam0068143c = iRam0068143c + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam00682028;
    uRam00682028 = uRam00682028 + 1;
    iRam0068202c = iRam0068202c + (uint)bVar11;
    *(undefined1 *)((int)pppppcStack_58 + (int)pppppcStack_54) = 0;
    _ZN13ModuleManager28notifyGetSettingsSoundOutputENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEERS5_
              (iVar9,&pppppcStack_58,&ppppcStack_ac);
    bVar11 = 0xfffffffe < uRam00682038;
    uRam00682038 = uRam00682038 + 1;
    iRam0068203c = iRam0068203c + (uint)bVar11;
    if ((code ******)pppppcStack_58 != &pppppcStack_50) {
      func_0x00091788(pppppcStack_58,(undefined *)((int)pppppcStack_50 + 1));
    }
    bVar11 = 0xfffffffe < uRam00682040;
    uRam00682040 = uRam00682040 + 1;
    iRam00682044 = iRam00682044 + (uint)bVar11;
    iVar2 = _Z15getPmLogContextv();
    if (iVar2 == 0) {
      bVar11 = 0xfffffffe < uRam00682050;
      uRam00682050 = uRam00682050 + 1;
      iRam00682054 = iRam00682054 + (uint)bVar11;
    }
    else {
      bVar11 = 0xfffffffe < uRam00682058;
      uRam00682058 = uRam00682058 + 1;
      iRam0068205c = iRam0068205c + (uint)bVar11;
      piVar3 = (int *)_Z15getPmLogContextv();
      bVar11 = 0xfffffffe < uRam00682068;
      uRam00682068 = uRam00682068 + 1;
      iRam0068206c = iRam0068206c + (uint)bVar11;
      if (*piVar3 < 6) goto LAB_0019964a;
      bVar11 = 0xfffffffe < uRam00682070;
      uRam00682070 = uRam00682070 + 1;
      iRam00682074 = iRam00682074 + (uint)bVar11;
    }
    uVar4 = _Z15getPmLogContextv();
    bVar11 = 0xfffffffe < uRam00682080;
    uRam00682080 = uRam00682080 + 1;
    iRam00682084 = iRam00682084 + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam00680bb8;
    uRam00680bb8 = uRam00680bb8 + 1;
    iRam00680bbc = iRam00680bbc + (uint)bVar11;
    func_0x00091a40(uVar4,6,0,&UNK_0049ed3c,0,0,0,&UNK_004ae404,&UNK_004ade42,0x7d1,ppppcStack_ac);
    bVar11 = 0xfffffffe < uRam00682090;
    uRam00682090 = uRam00682090 + 1;
    iRam00682094 = iRam00682094 + (uint)bVar11;
  }
LAB_0019964a:
  iVar2 = func_0x000913cc(&ppppcStack_ac,&UNK_0049f4dc);
  if (iVar2 != 0) {
    bVar11 = 0xfffffffe < uRam006820a0;
    uRam006820a0 = uRam006820a0 + 1;
    iRam006820a4 = iRam006820a4 + (uint)bVar11;
    iVar2 = func_0x000913cc(&pppppcStack_c4,&UNK_0049f550);
    if (iVar2 == 0) {
      iVar2 = *(int *)(param_1 + 0x2a8);
      if (iVar2 == 0) {
        bVar11 = 0xfffffffe < uRam006822a8;
        uRam006822a8 = uRam006822a8 + 1;
        iRam006822ac = iRam006822ac + (uint)bVar11;
      }
      else {
        bVar11 = 0xfffffffe < uRam006822a0;
        uRam006822a0 = uRam006822a0 + 1;
        iRam006822a4 = iRam006822a4 + (uint)bVar11;
        bVar11 = 0xfffffffe < uRam00680e18;
        uRam00680e18 = uRam00680e18 + 1;
        iRam00680e1c = iRam00680e1c + (uint)bVar11;
        bVar11 = 0xfffffffe < uRam00680e30;
        uRam00680e30 = uRam00680e30 + 1;
        iRam00680e34 = iRam00680e34 + (uint)bVar11;
        bVar11 = 0xfffffffe < uRam006822b8;
        uRam006822b8 = uRam006822b8 + 1;
        iRam006822bc = iRam006822bc + (uint)bVar11;
        bVar11 = 0xfffffffd < uRam00680e20;
        uRam00680e20 = uRam00680e20 + 2;
        iRam00680e24 = iRam00680e24 + (uint)bVar11;
        bVar11 = 0xfffffffd < uRam00680df0;
        uRam00680df0 = uRam00680df0 + 2;
        iRam00680df4 = iRam00680df4 + (uint)bVar11;
        ppppcStack_74 = (code ****)0x53746573;
        uStack_70 = 0x646e756f;
        uStack_6c = 0x7074754f;
        uStack_68 = 0x7475;
        uStack_78 = 0xe;
        uStack_66 = 0;
        pppppcStack_94 = (code *****)0x17;
        bVar11 = 0xfffffffe < uRam00680e08;
        uRam00680e08 = uRam00680e08 + 1;
        iRam00680e0c = iRam00680e0c + (uint)bVar11;
        pppppcStack_7c = &ppppcStack_74;
        pppppcStack_58 = (code *****)&pppppcStack_50;
        pppppcStack_58 = (code *****)func_0x00092130(&pppppcStack_58,&pppppcStack_94,0);
        pppppcStack_50 = pppppcStack_94;
        bVar11 = 0xfffffffe < uRam00680e10;
        uRam00680e10 = uRam00680e10 + 1;
        iRam00680e14 = iRam00680e14 + (uint)bVar11;
        bVar11 = 0xfffffffe < uRam00680e18;
        uRam00680e18 = uRam00680e18 + 1;
        iRam00680e1c = iRam00680e1c + (uint)bVar11;
        bVar11 = 0xfffffffe < uRam00680e30;
        uRam00680e30 = uRam00680e30 + 1;
        iRam00680e34 = iRam00680e34 + (uint)bVar11;
        pppppcStack_58[3] = (code ****)0x63697672;
        *pppppcStack_58 = (code ****)0x2e6d6f63;
        *(undefined2 *)(pppppcStack_58 + 5) = 0x6964;
        pppppcStack_58[4] = (code ****)0x75612e65;
        pppppcStack_58[1] = (code ****)0x6f626577;
        *(undefined1 *)((int)pppppcStack_58 + 0x16) = 0x6f;
        pppppcStack_58[2] = (code ****)0x65732e73;
        pppppcStack_54 = pppppcStack_94;
        *(undefined1 *)((int)pppppcStack_58 + (int)pppppcStack_94) = 0;
        bVar11 = 0xfffffffe < uRam006822c8;
        uRam006822c8 = uRam006822c8 + 1;
        iRam006822cc = iRam006822cc + (uint)bVar11;
        _ZN13ModuleManager27notifySetMasterVolumeParamsERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_S5_
                  (iVar2,&pppppcStack_7c,&pppppcStack_c4,&pppppcStack_58);
        bVar11 = 0xfffffffe < uRam006822d8;
        uRam006822d8 = uRam006822d8 + 1;
        iRam006822dc = iRam006822dc + (uint)bVar11;
        if ((code ******)pppppcStack_58 != &pppppcStack_50) {
          func_0x00091788(pppppcStack_58,(undefined *)((int)pppppcStack_50 + 1));
        }
        bVar11 = 0xfffffffe < uRam006822e0;
        uRam006822e0 = uRam006822e0 + 1;
        iRam006822e4 = iRam006822e4 + (uint)bVar11;
        if (pppppcStack_7c != &ppppcStack_74) {
          func_0x00091788(pppppcStack_7c,(int)ppppcStack_74 + 1);
        }
        bVar11 = 0xfffffffe < uRam006822e8;
        uRam006822e8 = uRam006822e8 + 1;
        iRam006822ec = iRam006822ec + (uint)bVar11;
      }
      goto LAB_00199ce8;
    }
    bVar11 = 0xfffffffe < uRam00682298;
    uRam00682298 = uRam00682298 + 1;
    iRam0068229c = iRam0068229c + (uint)bVar11;
    iVar2 = func_0x000913cc(&pppppcStack_c4,&UNK_0049ef88);
    if (iVar2 == 0) {
      if (*(int *)(param_1 + 0x2a8) == 0) {
        bVar11 = 0xfffffffe < uRam00682300;
        uRam00682300 = uRam00682300 + 1;
        iRam00682304 = iRam00682304 + (uint)bVar11;
      }
      else {
        bVar11 = 0xfffffffe < uRam006822f8;
        uRam006822f8 = uRam006822f8 + 1;
        iRam006822fc = iRam006822fc + (uint)bVar11;
        _ZN13ModuleManager33notifySetSoundOutForPowerOnlyModeERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
                  (*(int *)(param_1 + 0x2a8),&pppppcStack_c4);
        bVar11 = 0xfffffffe < uRam00682310;
        uRam00682310 = uRam00682310 + 1;
        iRam00682314 = iRam00682314 + (uint)bVar11;
      }
      goto LAB_00199ce8;
    }
    bVar11 = 0xfffffffe < uRam006822f0;
    uRam006822f0 = uRam006822f0 + 1;
    iRam006822f4 = iRam006822f4 + (uint)bVar11;
    iVar2 = _Z15getPmLogContextv();
    if (iVar2 == 0) {
      bVar11 = 0xfffffffe < uRam00682320;
      uRam00682320 = uRam00682320 + 1;
      iRam00682324 = iRam00682324 + (uint)bVar11;
LAB_0019a6e4:
      uVar4 = _Z15getPmLogContextv();
      bVar11 = 0xfffffffe < uRam00682350;
      uRam00682350 = uRam00682350 + 1;
      iRam00682354 = iRam00682354 + (uint)bVar11;
      uVar5 = func_0x00193610();
      func_0x00091a40(uVar4,6,0,&UNK_0049ed3c,0,0,0,&UNK_004ae4c8,uVar5,0x7f5,&UNK_004ae2dc,*param_2
                      ,param_3);
      bVar11 = 0xfffffffe < uRam00682360;
      uRam00682360 = uRam00682360 + 1;
      iRam00682364 = iRam00682364 + (uint)bVar11;
    }
    else {
      bVar11 = 0xfffffffe < uRam00682328;
      uRam00682328 = uRam00682328 + 1;
      iRam0068232c = iRam0068232c + (uint)bVar11;
      piVar3 = (int *)_Z15getPmLogContextv();
      bVar11 = 0xfffffffe < uRam00682338;
      uRam00682338 = uRam00682338 + 1;
      iRam0068233c = iRam0068233c + (uint)bVar11;
      if (5 < *piVar3) {
        bVar11 = 0xfffffffe < uRam00682340;
        uRam00682340 = uRam00682340 + 1;
        iRam00682344 = iRam00682344 + (uint)bVar11;
        goto LAB_0019a6e4;
      }
    }
    iVar2 = func_0x000913cc(&pppppcStack_c4,&UNK_0049efac);
    if (iVar2 == 0) {
      bVar11 = 0xfffffffe < uRam00682368;
      uRam00682368 = uRam00682368 + 1;
      iRam0068236c = iRam0068236c + (uint)bVar11;
      if (*(char *)(param_1 + 0x31c) == '\0') {
        bVar11 = 0xfffffffe < uRam00682378;
        uRam00682378 = uRam00682378 + 1;
        iRam0068237c = iRam0068237c + (uint)bVar11;
        iVar2 = func_0x000913cc(param_1 + 0x2ec,&UNK_0049f4dc);
        bVar11 = 0xfffffffe < uRam00682380;
        uRam00682380 = uRam00682380 + 1;
        iRam00682384 = iRam00682384 + (uint)bVar11;
        if (iVar2 == 0) goto LAB_0019a966;
        bVar11 = 0xfffffffe < uRam00682388;
        uRam00682388 = uRam00682388 + 1;
        iRam0068238c = iRam0068238c + (uint)bVar11;
      }
      func_0x000914dc(&pppppcStack_c4,0,pppppcStack_c0,&UNK_0049e6bc,0x1f);
    }
    else {
      bVar11 = 0xfffffffe < uRam00682370;
      uRam00682370 = uRam00682370 + 1;
      iRam00682374 = iRam00682374 + (uint)bVar11;
LAB_0019a966:
      iVar2 = func_0x000913cc(&ppppcStack_ac,&UNK_0049f564);
      if (iVar2 == 0) {
        bVar11 = 0xfffffffe < uRam00682398;
        uRam00682398 = uRam00682398 + 1;
        iRam0068239c = iRam0068239c + (uint)bVar11;
        iVar2 = _Z15getPmLogContextv();
        if (iVar2 == 0) {
          bVar11 = 0xfffffffe < uRam006823b0;
          uRam006823b0 = uRam006823b0 + 1;
          iRam006823b4 = iRam006823b4 + (uint)bVar11;
LAB_0019a9fe:
          uVar4 = _Z15getPmLogContextv();
          bVar11 = 0xfffffffe < uRam006823e0;
          uRam006823e0 = uRam006823e0 + 1;
          iRam006823e4 = iRam006823e4 + (uint)bVar11;
          uVar5 = func_0x00193610();
          func_0x00091a40(uVar4,6,0,&UNK_0049ed3c,0,0,0,&UNK_004ae500,uVar5,0x7fc,&UNK_004ae2dc,
                          *param_2,param_3);
          bVar11 = 0xfffffffe < uRam006823f0;
          uRam006823f0 = uRam006823f0 + 1;
          iRam006823f4 = iRam006823f4 + (uint)bVar11;
        }
        else {
          bVar11 = 0xfffffffe < uRam006823b8;
          uRam006823b8 = uRam006823b8 + 1;
          iRam006823bc = iRam006823bc + (uint)bVar11;
          piVar3 = (int *)_Z15getPmLogContextv();
          bVar11 = 0xfffffffe < uRam006823c8;
          uRam006823c8 = uRam006823c8 + 1;
          iRam006823cc = iRam006823cc + (uint)bVar11;
          if (5 < *piVar3) {
            bVar11 = 0xfffffffe < uRam006823d0;
            uRam006823d0 = uRam006823d0 + 1;
            iRam006823d4 = iRam006823d4 + (uint)bVar11;
            goto LAB_0019a9fe;
          }
        }
        iVar2 = _ZNK9TVManager36isAvailableSetBtTVAtmosForSingluarBTEv(param_1);
        if (iVar2 != 0) {
          bVar11 = 0xfffffffe < uRam00682400;
          uRam00682400 = uRam00682400 + 1;
          iRam00682404 = iRam00682404 + (uint)bVar11;
          iVar2 = func_0x000913cc(param_1 + 0x424,&UNK_004cd3bc);
          bVar11 = 0xfffffffe < uRam00682408;
          uRam00682408 = uRam00682408 + 1;
          iRam0068240c = iRam0068240c + (uint)bVar11;
          if (iVar2 == 0) {
            bVar11 = 0xfffffffe < uRam00682410;
            uRam00682410 = uRam00682410 + 1;
            iRam00682414 = iRam00682414 + (uint)bVar11;
            func_0x001939d8(&pppppcStack_c4,&UNK_0049efe0);
            bVar11 = 0xfffffffe < uRam00682428;
            uRam00682428 = uRam00682428 + 1;
            iRam0068242c = iRam0068242c + (uint)bVar11;
            goto LAB_0019a7fe;
          }
        }
        if (*(char *)(param_1 + 0x39f) == '\0') {
          bVar11 = 0xfffffffe < uRam00682420;
          uRam00682420 = uRam00682420 + 1;
          iRam00682424 = iRam00682424 + (uint)bVar11;
LAB_0019ac5c:
          iVar2 = _ZNK9TVManager21isAvailableSetBTAtmosEv(param_1);
          bVar11 = 0xfffffffe < uRam00682470;
          uRam00682470 = uRam00682470 + 1;
          iRam00682474 = iRam00682474 + (uint)bVar11;
          if (iVar2 != 0) {
            bVar11 = 0xfffffffe < uRam00682478;
            uRam00682478 = uRam00682478 + 1;
            iRam0068247c = iRam0068247c + (uint)bVar11;
            iVar2 = func_0x000913cc(param_1 + 0x424,&UNK_004cd3bc);
            bVar11 = 0xfffffffe < uRam00682480;
            uRam00682480 = uRam00682480 + 1;
            iRam00682484 = iRam00682484 + (uint)bVar11;
            if (iVar2 == 0) {
              bVar11 = 0xfffffffe < uRam00682488;
              uRam00682488 = uRam00682488 + 1;
              iRam0068248c = iRam0068248c + (uint)bVar11;
              func_0x001939d8(&pppppcStack_c4,&UNK_004ae538);
              bVar11 = 0xfffffffe < uRam006824a0;
              uRam006824a0 = uRam006824a0 + 1;
              iRam006824a4 = iRam006824a4 + (uint)bVar11;
              goto LAB_0019a7fe;
            }
          }
          func_0x001939d8(&pppppcStack_c4,&UNK_0049f564);
          bVar11 = 0xfffffffe < uRam00682490;
          uRam00682490 = uRam00682490 + 1;
          iRam00682494 = iRam00682494 + (uint)bVar11;
        }
        else {
          bVar11 = 0xfffffffe < uRam00682418;
          uRam00682418 = uRam00682418 + 1;
          iRam0068241c = iRam0068241c + (uint)bVar11;
          iVar2 = func_0x000913cc(param_1 + 0x424,&UNK_004cd3bc);
          bVar11 = 0xfffffffe < uRam00682438;
          uRam00682438 = uRam00682438 + 1;
          iRam0068243c = iRam0068243c + (uint)bVar11;
          if (iVar2 == 0) {
            bVar11 = 0xfffffffe < uRam00682440;
            uRam00682440 = uRam00682440 + 1;
            iRam00682444 = iRam00682444 + (uint)bVar11;
            iVar2 = func_0x000913cc(param_1 + 0x274,&UNK_004cdae0);
            bVar11 = 0xfffffffe < uRam00682448;
            uRam00682448 = uRam00682448 + 1;
            iRam0068244c = iRam0068244c + (uint)bVar11;
            if (iVar2 != 0) goto LAB_0019ac5c;
            bVar11 = 0xfffffffe < uRam00682450;
            uRam00682450 = uRam00682450 + 1;
            iRam00682454 = iRam00682454 + (uint)bVar11;
          }
          func_0x001939d8(&pppppcStack_c4,&UNK_0049f004);
          bVar11 = 0xfffffffe < uRam00682458;
          uRam00682458 = uRam00682458 + 1;
          iRam0068245c = iRam0068245c + (uint)bVar11;
        }
      }
      else {
        bVar11 = 0xfffffffe < uRam006823a0;
        uRam006823a0 = uRam006823a0 + 1;
        iRam006823a4 = iRam006823a4 + (uint)bVar11;
        iVar2 = func_0x000913cc(&ppppcStack_ac,&UNK_0049ef58);
        bVar11 = 0xfffffffe < uRam006824b0;
        uRam006824b0 = uRam006824b0 + 1;
        iRam006824b4 = iRam006824b4 + (uint)bVar11;
        if (iVar2 == 0) {
          bVar11 = 0xfffffffe < uRam006824b8;
          uRam006824b8 = uRam006824b8 + 1;
          iRam006824bc = iRam006824bc + (uint)bVar11;
          iVar2 = func_0x000913cc(param_1 + 0x2ec,&UNK_004ae544);
          if (iVar2 == 0) {
            bVar11 = 0xfffffffe < uRam006824c0;
            uRam006824c0 = uRam006824c0 + 1;
            iRam006824c4 = iRam006824c4 + (uint)bVar11;
            func_0x001939d8(&pppppcStack_c4,&UNK_0049ef58);
            bVar11 = 0xfffffffe < uRam006824d0;
            uRam006824d0 = uRam006824d0 + 1;
            iRam006824d4 = iRam006824d4 + (uint)bVar11;
          }
          else {
            bVar11 = 0xfffffffe < uRam006824c8;
            uRam006824c8 = uRam006824c8 + 1;
            iRam006824cc = iRam006824cc + (uint)bVar11;
            iVar2 = _ZNK9TVManager32isAvailableSetBtTVAtmosWithTVSpkEv(param_1);
            bVar11 = 0xfffffffe < uRam006824e8;
            uRam006824e8 = uRam006824e8 + 1;
            iRam006824ec = iRam006824ec + (uint)bVar11;
            if (iVar2 != 0) {
              bVar11 = 0xfffffffe < uRam006824f0;
              uRam006824f0 = uRam006824f0 + 1;
              iRam006824f4 = iRam006824f4 + (uint)bVar11;
              iVar2 = func_0x000913cc(param_1 + 0x424,&UNK_004cd3bc);
              bVar11 = 0xfffffffe < uRam006824f8;
              uRam006824f8 = uRam006824f8 + 1;
              iRam006824fc = iRam006824fc + (uint)bVar11;
              if (iVar2 == 0) {
                bVar11 = 0xfffffffe < uRam00682500;
                uRam00682500 = uRam00682500 + 1;
                iRam00682504 = iRam00682504 + (uint)bVar11;
                func_0x001939d8(&pppppcStack_c4,&UNK_0049efec);
                bVar11 = 0xfffffffe < uRam00682518;
                uRam00682518 = uRam00682518 + 1;
                iRam0068251c = iRam0068251c + (uint)bVar11;
                goto LAB_0019a7fe;
              }
            }
            iVar2 = func_0x000913cc(param_1 + 0x364,&UNK_004ae378);
            if (iVar2 == 0) {
              bVar11 = 0xfffffffe < uRam00682508;
              uRam00682508 = uRam00682508 + 1;
              iRam0068250c = iRam0068250c + (uint)bVar11;
              func_0x001939d8(&pppppcStack_c4,&UNK_0049ef70);
              bVar11 = 0xfffffffe < uRam00682528;
              uRam00682528 = uRam00682528 + 1;
              iRam0068252c = iRam0068252c + (uint)bVar11;
            }
            else {
              bVar11 = 0xfffffffe < uRam00682510;
              uRam00682510 = uRam00682510 + 1;
              iRam00682514 = iRam00682514 + (uint)bVar11;
              iVar2 = func_0x000913cc(param_1 + 0x364,&UNK_004ae550);
              bVar11 = 0xfffffffe < uRam00682538;
              uRam00682538 = uRam00682538 + 1;
              iRam0068253c = iRam0068253c + (uint)bVar11;
              if (iVar2 == 0) {
                bVar11 = 0xfffffffe < uRam00682540;
                uRam00682540 = uRam00682540 + 1;
                iRam00682544 = iRam00682544 + (uint)bVar11;
                func_0x001939d8(&pppppcStack_c4,&UNK_0049ef58);
                bVar11 = 0xfffffffe < uRam00682548;
                uRam00682548 = uRam00682548 + 1;
                iRam0068254c = iRam0068254c + (uint)bVar11;
              }
            }
          }
        }
      }
    }
LAB_0019a7fe:
    iVar2 = *(int *)(param_1 + 0x2a8);
    if (iVar2 == 0) {
      bVar11 = 0xfffffffe < uRam00682560;
      uRam00682560 = uRam00682560 + 1;
      iRam00682564 = iRam00682564 + (uint)bVar11;
    }
    else {
      bVar11 = 0xfffffffe < uRam00682558;
      uRam00682558 = uRam00682558 + 1;
      iRam0068255c = iRam0068255c + (uint)bVar11;
      func_0x00193cec(&pppppcStack_7c,&UNK_004ae4b8);
      bVar11 = 0xfffffffe < uRam00682570;
      uRam00682570 = uRam00682570 + 1;
      iRam00682574 = iRam00682574 + (uint)bVar11;
      func_0x00193cec(&pppppcStack_58,&UNK_0049dcac);
      bVar11 = 0xfffffffe < uRam00682580;
      uRam00682580 = uRam00682580 + 1;
      iRam00682584 = iRam00682584 + (uint)bVar11;
      _ZN13ModuleManager27notifySetMasterVolumeParamsERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEES7_S5_
                (iVar2,&pppppcStack_7c,&pppppcStack_c4,&pppppcStack_58);
      bVar11 = 0xfffffffe < uRam00682590;
      uRam00682590 = uRam00682590 + 1;
      iRam00682594 = iRam00682594 + (uint)bVar11;
      if ((code ******)pppppcStack_58 != &pppppcStack_50) {
        func_0x00091788(pppppcStack_58,(undefined *)((int)pppppcStack_50 + 1));
      }
      bVar11 = 0xfffffffe < uRam00682598;
      uRam00682598 = uRam00682598 + 1;
      iRam0068259c = iRam0068259c + (uint)bVar11;
      if (pppppcStack_7c != &ppppcStack_74) {
        func_0x00091788(pppppcStack_7c,(int)ppppcStack_74 + 1);
      }
      bVar11 = 0xfffffffe < uRam006825a0;
      uRam006825a0 = uRam006825a0 + 1;
      iRam006825a4 = iRam006825a4 + (uint)bVar11;
    }
    goto LAB_00199ce8;
  }
  bVar11 = 0xfffffffe < uRam00682098;
  uRam00682098 = uRam00682098 + 1;
  iRam0068209c = iRam0068209c + (uint)bVar11;
  iVar2 = func_0x000913cc(param_1 + 0x2ec,&UNK_0049f4dc);
  bVar11 = 0xfffffffe < uRam006820a8;
  uRam006820a8 = uRam006820a8 + 1;
  iRam006820ac = iRam006820ac + (uint)bVar11;
  if (iVar2 == 0) {
    bVar11 = 0xfffffffe < uRam006820b0;
    uRam006820b0 = uRam006820b0 + 1;
    iRam006820b4 = iRam006820b4 + (uint)bVar11;
    func_0x00091830(&pppppcStack_7c);
    bVar11 = 0xfffffffe < uRam006820b8;
    uRam006820b8 = uRam006820b8 + 1;
    iRam006820bc = iRam006820bc + (uint)bVar11;
    func_0x000914dc(param_1 + 0x2ec,0,*(undefined4 *)(param_1 + 0x2f0),&UNK_004cdae0,3);
    bVar11 = 0xfffffffe < uRam006820c8;
    uRam006820c8 = uRam006820c8 + 1;
    iRam006820cc = iRam006820cc + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam00682728;
    uRam00682728 = uRam00682728 + 1;
    iRam0068272c = iRam0068272c + (uint)bVar11;
    func_0x0009183c(&pppppcStack_58,&UNK_004a201c);
    bVar11 = 0xfffffffe < uRam00682730;
    uRam00682730 = uRam00682730 + 1;
    iRam00682734 = iRam00682734 + (uint)bVar11;
    func_0x0009183c(apuStack_4c,&UNK_0049fa74);
    bVar11 = 0xfffffffe < uRam00682740;
    uRam00682740 = uRam00682740 + 1;
    iRam00682744 = iRam00682744 + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam006820e0;
    uRam006820e0 = uRam006820e0 + 1;
    iRam006820e4 = iRam006820e4 + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam00682750;
    uRam00682750 = uRam00682750 + 1;
    iRam00682754 = iRam00682754 + (uint)bVar11;
    func_0x0009183c(&pppppcStack_94,&UNK_004ae424);
    bVar11 = 0xfffffffe < uRam00682758;
    uRam00682758 = uRam00682758 + 1;
    iRam0068275c = iRam0068275c + (uint)bVar11;
    func_0x0009183c(auStack_88,&UNK_004cdae0);
    uStack_cc = 1;
    bVar11 = 0xfffffffe < uRam00682768;
    uRam00682768 = uRam00682768 + 1;
    iRam0068276c = iRam0068276c + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam006820f0;
    uRam006820f0 = uRam006820f0 + 1;
    iRam006820f4 = iRam006820f4 + (uint)bVar11;
    pppppcStack_d0 = (code *****)&pppppcStack_94;
    func_0x000b0db4(&uStack_dc,&pppppcStack_94,1);
    bVar11 = 0xfffffffe < uRam006820f8;
    uRam006820f8 = uRam006820f8 + 1;
    iRam006820fc = iRam006820fc + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam00682778;
    uRam00682778 = uRam00682778 + 1;
    iRam0068277c = iRam0068277c + (uint)bVar11;
    uStack_dc = 0x510964;
    func_0x0009183c(auStack_40,&UNK_004a2028);
    bVar11 = 0xfffffffe < uRam00682780;
    uRam00682780 = uRam00682780 + 1;
    iRam00682784 = iRam00682784 + (uint)bVar11;
    func_0x0009156c(auStack_34);
    uVar5 = uStack_2c;
    uVar4 = uStack_30;
    uStack_30 = uStack_d8;
    uStack_d8 = uVar4;
    bVar11 = 0xfffffffe < uRam00682788;
    uRam00682788 = uRam00682788 + 1;
    iRam0068278c = iRam0068278c + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam00682108;
    uRam00682108 = uRam00682108 + 1;
    iRam0068210c = iRam0068210c + (uint)bVar11;
    uStack_2c = uStack_d4;
    uStack_d4 = uVar5;
    func_0x000b0db4(&pppppcStack_d0,&pppppcStack_58,2);
    pppppcStack_d0 = (code *****)(_ZTVN7pbnjson7JObjectE + 8);
    bVar11 = 0xfffffffe < uRam00682110;
    uRam00682110 = uRam00682110 + 1;
    iRam00682114 = iRam00682114 + (uint)bVar11;
    func_0x0009156c(auStack_e8);
    uVar5 = uStack_c8;
    uVar4 = uStack_e4;
    uStack_e4 = uStack_cc;
    uStack_cc = uVar4;
    bVar11 = 0xfffffffe < uRam00682118;
    uRam00682118 = uRam00682118 + 1;
    iRam0068211c = iRam0068211c + (uint)bVar11;
    uStack_c8 = uStack_e0;
    uStack_e0 = uVar5;
    pppppcStack_d0 = (code *****)(_ZTVN7pbnjson7JObjectE + 8);
    func_0x00091434(&pppppcStack_d0);
    bVar11 = 0xfffffffe < uRam00682128;
    uRam00682128 = uRam00682128 + 1;
    iRam0068212c = iRam0068212c + (uint)bVar11;
    pppppcVar7 = &ppppcStack_28;
    do {
      pppppcVar10 = pppppcVar7 + -6;
      bVar11 = 0xfffffffe < uRam00682138;
      uRam00682138 = uRam00682138 + 1;
      iRam0068213c = iRam0068213c + (uint)bVar11;
      func_0x00091ba8(pppppcVar7 + -3);
      bVar11 = 0xfffffffe < uRam00682140;
      uRam00682140 = uRam00682140 + 1;
      iRam00682144 = iRam00682144 + (uint)bVar11;
      func_0x00091ba8(pppppcVar10);
      pppppcVar7 = pppppcVar10;
    } while ((code ******)pppppcVar10 != &pppppcStack_58);
    uStack_dc = 0x510964;
    bVar11 = 0xfffffffe < uRam00682130;
    uRam00682130 = uRam00682130 + 1;
    iRam00682134 = iRam00682134 + (uint)bVar11;
    func_0x00091434(&uStack_dc);
    bVar11 = 0xfffffffe < uRam00682148;
    uRam00682148 = uRam00682148 + 1;
    iRam0068214c = iRam0068214c + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam00682158;
    uRam00682158 = uRam00682158 + 1;
    iRam0068215c = iRam0068215c + (uint)bVar11;
    func_0x00091ba8(auStack_88);
    bVar11 = 0xfffffffe < uRam00682160;
    uRam00682160 = uRam00682160 + 1;
    iRam00682164 = iRam00682164 + (uint)bVar11;
    func_0x00091ba8(&pppppcStack_94);
    bVar11 = 0xfffffffe < uRam00682150;
    uRam00682150 = uRam00682150 + 1;
    iRam00682154 = iRam00682154 + (uint)bVar11;
    iVar2 = _Z15getPmLogContextv();
    if (iVar2 == 0) {
      bVar11 = 0xfffffffe < uRam00682170;
      uRam00682170 = uRam00682170 + 1;
      iRam00682174 = iRam00682174 + (uint)bVar11;
LAB_00199a94:
      uVar4 = _Z15getPmLogContextv();
      bVar11 = 0xfffffffe < uRam006821a0;
      uRam006821a0 = uRam006821a0 + 1;
      iRam006821a4 = iRam006821a4 + (uint)bVar11;
      uVar5 = func_0x00193610();
      func_0x00091470(&pppppcStack_58,auStack_e8,0);
      bVar11 = 0xfffffffe < uRam006821b0;
      uRam006821b0 = uRam006821b0 + 1;
      iRam006821b4 = iRam006821b4 + (uint)bVar11;
      func_0x00091a40(uVar4,6,0,&UNK_0049ed3c,0,0,0,&UNK_004ae430,uVar5,0x7dd,pppppcStack_58);
      bVar11 = 0xfffffffe < uRam006821c0;
      uRam006821c0 = uRam006821c0 + 1;
      iRam006821c4 = iRam006821c4 + (uint)bVar11;
      if ((code ******)pppppcStack_58 != &pppppcStack_50) {
        func_0x00091788(pppppcStack_58,(undefined *)((int)pppppcStack_50 + 1));
      }
      bVar11 = 0xfffffffe < uRam006821c8;
      uRam006821c8 = uRam006821c8 + 1;
      iRam006821cc = iRam006821cc + (uint)bVar11;
    }
    else {
      bVar11 = 0xfffffffe < uRam00682178;
      uRam00682178 = uRam00682178 + 1;
      iRam0068217c = iRam0068217c + (uint)bVar11;
      piVar3 = (int *)_Z15getPmLogContextv();
      bVar11 = 0xfffffffe < uRam00682188;
      uRam00682188 = uRam00682188 + 1;
      iRam0068218c = iRam0068218c + (uint)bVar11;
      if (5 < *piVar3) {
        bVar11 = 0xfffffffe < uRam00682190;
        uRam00682190 = uRam00682190 + 1;
        iRam00682194 = iRam00682194 + (uint)bVar11;
        goto LAB_00199a94;
      }
    }
    uVar4 = _Z14GetPalmServicev();
    bVar11 = 0xfffffffe < uRam006821d8;
    uRam006821d8 = uRam006821d8 + 1;
    iRam006821dc = iRam006821dc + (uint)bVar11;
    func_0x00091470(&pppppcStack_58,auStack_e8,0);
    bVar11 = 0xfffffffe < uRam006821e8;
    uRam006821e8 = uRam006821e8 + 1;
    iRam006821ec = iRam006821ec + (uint)bVar11;
    iVar2 = func_0x00091818(uVar4,&UNK_004ae460,pppppcStack_58,0,0,0,&pppppcStack_7c);
    bVar11 = 0xfffffffe < uRam006821f8;
    uRam006821f8 = uRam006821f8 + 1;
    iRam006821fc = iRam006821fc + (uint)bVar11;
    if ((code ******)pppppcStack_58 != &pppppcStack_50) {
      func_0x00091788(pppppcStack_58,(undefined *)((int)pppppcStack_50 + 1));
    }
    bVar11 = 0xfffffffe < uRam00682200;
    uRam00682200 = uRam00682200 + 1;
    iRam00682204 = iRam00682204 + (uint)bVar11;
    if (iVar2 == 0) {
      bVar11 = 0xfffffffe < uRam00682208;
      uRam00682208 = uRam00682208 + 1;
      iRam0068220c = iRam0068220c + (uint)bVar11;
      _ZN8CLSError5PrintEPKci14GLogLevelFlags(&pppppcStack_7c,&UNK_004ae2dc,0x7df,0x10);
      bVar11 = 0xfffffffe < uRam00682218;
      uRam00682218 = uRam00682218 + 1;
      iRam0068221c = iRam0068221c + (uint)bVar11;
    }
    func_0x00091ba8(auStack_e8);
    bVar11 = 0xfffffffe < uRam00682220;
    uRam00682220 = uRam00682220 + 1;
    iRam00682224 = iRam00682224 + (uint)bVar11;
  }
  iVar2 = _Z15getPmLogContextv();
  if (iVar2 == 0) {
    bVar11 = 0xfffffffe < uRam00682230;
    uRam00682230 = uRam00682230 + 1;
    iRam00682234 = iRam00682234 + (uint)bVar11;
LAB_0019a082:
    uVar4 = _Z15getPmLogContextv();
    bVar11 = 0xfffffffe < uRam00682260;
    uRam00682260 = uRam00682260 + 1;
    iRam00682264 = iRam00682264 + (uint)bVar11;
    bVar11 = 0xfffffffe < uRam00680bb8;
    uRam00680bb8 = uRam00680bb8 + 1;
    iRam00680bbc = iRam00680bbc + (uint)bVar11;
    uVar5 = _ZNK9TVManager22getArcActivationStatusEv(param_1);
    bVar11 = 0xfffffffe < uRam00682270;
    uRam00682270 = uRam00682270 + 1;
    iRam00682274 = iRam00682274 + (uint)bVar11;
    func_0x00091a40(uVar4,7,0,0,0,0,0,&UNK_004ae494,&UNK_004ade42,0x7e3,uVar5);
    bVar11 = 0xfffffffe < uRam00682280;
    uRam00682280 = uRam00682280 + 1;
    iRam00682284 = iRam00682284 + (uint)bVar11;
  }
  else {
    bVar11 = 0xfffffffe < uRam00682238;
    uRam00682238 = uRam00682238 + 1;
    iRam0068223c = iRam0068223c + (uint)bVar11;
    piVar3 = (int *)_Z15getPmLogContextv();
    bVar11 = 0xfffffffe < uRam00682248;
    uRam00682248 = uRam00682248 + 1;
    iRam0068224c = iRam0068224c + (uint)bVar11;
    if (6 < *piVar3) {
      bVar11 = 0xfffffffe < uRam00682250;
      uRam00682250 = uRam00682250 + 1;
      iRam00682254 = iRam00682254 + (uint)bVar11;
      goto LAB_0019a082;
    }
  }
  _ZN9TVManager25setExternalArcSoundOutputERNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
            (param_1,&pppppcStack_c4);
  bVar11 = 0xfffffffe < uRam00682290;
  uRam00682290 = uRam00682290 + 1;
  iRam00682294 = iRam00682294 + (uint)bVar11;
LAB_00199ce8:
  _ZN9TVManager13setVideoDelayERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
            (param_1,&ppppcStack_ac);
  bVar11 = 0xfffffffe < uRam006825b0;
  uRam006825b0 = uRam006825b0 + 1;
  iRam006825b4 = iRam006825b4 + (uint)bVar11;
  if ((code *****)ppppcStack_ac != appppcStack_a4) {
    func_0x00091788(ppppcStack_ac,(int)appppcStack_a4[0] + 1);
  }
  bVar11 = 0xfffffffe < uRam006825b8;
  uRam006825b8 = uRam006825b8 + 1;
  iRam006825bc = iRam006825bc + (uint)bVar11;
  if ((code ******)pppppcStack_c4 != apppppcStack_bc) {
    func_0x00091788(pppppcStack_c4,(undefined *)((int)apppppcStack_bc[0] + 1));
  }
  bVar11 = 0xfffffffe < uRam006825c0;
  uRam006825c0 = uRam006825c0 + 1;
  iRam006825c4 = iRam006825c4 + (uint)bVar11;
  return;
}


