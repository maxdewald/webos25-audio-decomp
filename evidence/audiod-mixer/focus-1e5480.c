/* LOCAL STATIC EVIDENCE: Ghidra pseudocode, not original source. SHA256 170d468085255d381245693260840771b00d39c718473ec024566fb1df1ca3af image base 00010000 */

/* @001ec608 _ZN9TVManager21mixDigitalSoundOutputEN7pbnjson6JValueE */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

int _ZN9TVManager21mixDigitalSoundOutputEN7pbnjson6JValueE(int param_1,undefined4 param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  undefined *puVar6;
  uint uVar7;
  uint uVar8;
  uint *puVar9;
  undefined4 uVar10;
  undefined1 auStack_44 [12];
  int *apiStack_38 [2];
  int aiStack_30 [4];
  
  iVar2 = _UNK_001ec920;
  iVar4 = _UNK_001ec91c;
  iVar1 = _UNK_001ec918;
  uVar7 = *(uint *)(&UNK_001ec716 + _UNK_001ec918);
  piVar5 = (int *)(&UNK_001ec71a + _UNK_001ec918);
  puVar9 = (uint *)(&UNK_001ed01a + _UNK_001ec91c);
  puVar6 = &UNK_001ec634 + _UNK_001ec920;
  *(uint *)(&UNK_001ec716 + _UNK_001ec918) = uVar7 + 1;
  uVar8 = *puVar9;
  *(uint *)(&UNK_001ec71a + iVar1) = *piVar5 + (uint)(0xfffffffe < uVar7);
  *puVar9 = uVar8 + 1;
  *(uint *)(&UNK_001ed01e + iVar4) = *(int *)(&UNK_001ed01e + iVar4) + (uint)(0xfffffffe < uVar8);
  apiStack_38[0] = aiStack_30;
  func_0x001e73e0(apiStack_38,puVar6,&UNK_001ec637 + iVar2);
  uVar7 = *(uint *)(&UNK_001ed02a + iVar4);
  *(uint *)(&UNK_001ed02a + iVar4) = uVar7 + 1;
  *(uint *)(&UNK_001ed02e + iVar4) = *(int *)(&UNK_001ed02e + iVar4) + (uint)(0xfffffffe < uVar7);
  uVar7 = *(uint *)(&UNK_001ec71e + iVar1);
  *(uint *)(&UNK_001ec71e + iVar1) = uVar7 + 1;
  *(uint *)(&UNK_001ec722 + iVar1) = *(int *)(&UNK_001ec722 + iVar1) + (uint)(0xfffffffe < uVar7);
  iVar4 = func_0x00091b9c(param_2,apiStack_38);
  iVar1 = _UNK_001ec924;
  uVar7 = *(uint *)(&UNK_001ec79a + _UNK_001ec924);
  *(uint *)(&UNK_001ec79a + _UNK_001ec924) = uVar7 + 1;
  *(uint *)(&UNK_001ec79e + iVar1) = *(int *)(&UNK_001ec79e + iVar1) + (uint)(0xfffffffe < uVar7);
  if (apiStack_38[0] != aiStack_30) {
    func_0x00091788(apiStack_38[0],aiStack_30[0] + 1);
  }
  iVar1 = _UNK_001ec928;
  uVar7 = *(uint *)(&UNK_001ec7c8 + _UNK_001ec928);
  *(uint *)(&UNK_001ec7c8 + _UNK_001ec928) = uVar7 + 1;
  *(uint *)(&UNK_001ec7cc + iVar1) = *(int *)(&UNK_001ec7cc + iVar1) + (uint)(0xfffffffe < uVar7);
  iVar3 = _UNK_001ec930;
  iVar2 = _UNK_001ec92c;
  if (iVar4 != 0) {
    uVar7 = *(uint *)(&UNK_001ec7d0 + iVar1);
    uVar10 = *(undefined4 *)(param_1 + 0x400);
    *(uint *)(&UNK_001ec7d0 + iVar1) = uVar7 + 1;
    *(uint *)(&UNK_001ec7d4 + iVar1) = *(int *)(&UNK_001ec7d4 + iVar1) + (uint)(0xfffffffe < uVar7);
    uVar7 = *(uint *)(&UNK_001ed0d8 + iVar2);
    *(uint *)(&UNK_001ed0d8 + iVar2) = uVar7 + 1;
    *(uint *)(&UNK_001ed0dc + iVar2) = *(int *)(&UNK_001ed0dc + iVar2) + (uint)(0xfffffffe < uVar7);
    apiStack_38[0] = aiStack_30;
    func_0x001e73e0(apiStack_38,&UNK_001ec6f2 + iVar3,&UNK_001ec6fa + iVar3);
    uVar7 = *(uint *)(&UNK_001ed0e8 + iVar2);
    *(uint *)(&UNK_001ed0e8 + iVar2) = uVar7 + 1;
    *(uint *)(&UNK_001ed0ec + iVar2) = *(int *)(&UNK_001ed0ec + iVar2) + (uint)(0xfffffffe < uVar7);
    uVar7 = *(uint *)(&UNK_001ec7d8 + iVar1);
    *(uint *)(&UNK_001ec7d8 + iVar1) = uVar7 + 1;
    *(uint *)(&UNK_001ec7dc + iVar1) = *(int *)(&UNK_001ec7dc + iVar1) + (uint)(0xfffffffe < uVar7);
    func_0x000923ac(auStack_44,param_2);
    iVar1 = _UNK_001ec934;
    uVar7 = *(uint *)(&UNK_001ec886 + _UNK_001ec934);
    *(uint *)(&UNK_001ec886 + _UNK_001ec934) = uVar7 + 1;
    *(uint *)(&UNK_001ec88a + iVar1) = *(int *)(&UNK_001ec88a + iVar1) + (uint)(0xfffffffe < uVar7);
    iVar4 = _ZN10AudioMixer11setSettingsENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueEPFbP8LSHandleP9LSMessagePvEP8envelope
                      (uVar10,apiStack_38,auStack_44,0,0);
    iVar1 = _UNK_001ec938;
    uVar7 = *(uint *)(&UNK_001ec8bc + _UNK_001ec938);
    *(uint *)(&UNK_001ec8bc + _UNK_001ec938) = uVar7 + 1;
    *(uint *)(&UNK_001ec8c0 + iVar1) = *(int *)(&UNK_001ec8c0 + iVar1) + (uint)(0xfffffffe < uVar7);
    func_0x00091ba8(auStack_44);
    uVar7 = *(uint *)(&UNK_001ec8c4 + iVar1);
    *(uint *)(&UNK_001ec8c4 + iVar1) = uVar7 + 1;
    *(uint *)(&UNK_001ec8c8 + iVar1) = *(int *)(&UNK_001ec8c8 + iVar1) + (uint)(0xfffffffe < uVar7);
    if (apiStack_38[0] != aiStack_30) {
      func_0x00091788(apiStack_38[0],aiStack_30[0] + 1);
    }
    iVar1 = _UNK_001ec93c;
    uVar7 = *(uint *)(&UNK_001ec90a + _UNK_001ec93c);
    piVar5 = (int *)(&UNK_001ec90e + _UNK_001ec93c);
    *(uint *)(&UNK_001ec90a + _UNK_001ec93c) = uVar7 + 1;
    *(uint *)(&UNK_001ec90e + iVar1) = *piVar5 + (uint)(0xfffffffe < uVar7);
    if (iVar4 != 0) {
      return iVar4;
    }
    uVar7 = *(uint *)(&UNK_001ec912 + iVar1);
    *(uint *)(&UNK_001ec912 + iVar1) = uVar7 + 1;
    *(uint *)(&UNK_001ec916 + iVar1) = *(int *)(&UNK_001ec916 + iVar1) + (uint)(0xfffffffe < uVar7);
    iVar4 = _Z15getPmLogContextv();
    if (iVar4 == 0) {
      uVar7 = *(uint *)(&UNK_001ec91a + iVar1);
      *(uint *)(&UNK_001ec91a + iVar1) = uVar7 + 1;
      *(uint *)(&UNK_001ec91e + iVar1) =
           *(int *)(&UNK_001ec91e + iVar1) + (uint)(0xfffffffe < uVar7);
    }
    else {
      uVar7 = *(uint *)(&UNK_001ec922 + iVar1);
      *(uint *)(&UNK_001ec922 + iVar1) = uVar7 + 1;
      *(uint *)(&UNK_001ec926 + iVar1) =
           *(int *)(&UNK_001ec926 + iVar1) + (uint)(0xfffffffe < uVar7);
      piVar5 = (int *)_Z15getPmLogContextv();
      uVar7 = *(uint *)(&UNK_001ec92a + iVar1);
      iVar4 = *piVar5;
      *(uint *)(&UNK_001ec92a + iVar1) = uVar7 + 1;
      *(uint *)(&UNK_001ec92e + iVar1) =
           *(int *)(&UNK_001ec92e + iVar1) + (uint)(0xfffffffe < uVar7);
      if (iVar4 < 3) {
        return 0;
      }
      uVar7 = *(uint *)(&UNK_001ec932 + iVar1);
      *(uint *)(&UNK_001ec932 + iVar1) = uVar7 + 1;
      *(uint *)(&UNK_001ec936 + iVar1) =
           *(int *)(&UNK_001ec936 + iVar1) + (uint)(0xfffffffe < uVar7);
    }
    iVar1 = _UNK_001ec940;
    uVar10 = _Z15getPmLogContextv();
    iVar4 = _UNK_001ec944;
    uVar7 = *(uint *)(&UNK_001ec9a2 + iVar1);
    *(uint *)(&UNK_001ec9a2 + iVar1) = uVar7 + 1;
    iVar2 = _UNK_001ec948;
    *(uint *)(&UNK_001ec9a6 + iVar1) = *(int *)(&UNK_001ec9a6 + iVar1) + (uint)(0xfffffffe < uVar7);
    puVar6 = &UNK_001ec842 + _UNK_001ec94c;
    uVar7 = *(uint *)(&UNK_001ecae8 + iVar4);
    *(uint *)(&UNK_001ecae8 + iVar4) = uVar7 + 1;
    *(uint *)(&UNK_001ecaec + iVar4) = *(int *)(&UNK_001ecaec + iVar4) + (uint)(0xfffffffe < uVar7);
    func_0x00091a40(uVar10,3,0,&UNK_001ec866 + _UNK_001ec950,0,0,0,puVar6,&UNK_001ec88a + iVar2,
                    0x217);
    uVar7 = *(uint *)(&UNK_001ec9aa + iVar1);
    *(uint *)(&UNK_001ec9aa + iVar1) = uVar7 + 1;
    *(uint *)(&UNK_001ec9ae + iVar1) = *(int *)(&UNK_001ec9ae + iVar1) + (uint)(0xfffffffe < uVar7);
  }
  return 0;
}



/* @001d186c _ZN9TVManager22_mixDigitalSoundOutputEP8LSHandleP9LSMessagePv */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_ZN9TVManager22_mixDigitalSoundOutputEP8LSHandleP9LSMessagePv
          (undefined4 param_1,undefined4 param_2,int param_3)

{
  uint *puVar1;
  int iVar2;
  int iVar3;
  undefined4 uVar4;
  int *piVar5;
  int iVar6;
  undefined *puVar7;
  uint uVar8;
  undefined *puVar9;
  int iVar10;
  uint uVar11;
  uint *puVar12;
  bool bVar13;
  undefined1 uStack_131;
  undefined1 auStack_130 [12];
  uint *puStack_124;
  int iStack_120;
  uint auStack_11c [4];
  uint *puStack_10c;
  int iStack_108;
  uint auStack_104 [4];
  uint *puStack_f4;
  undefined4 uStack_f0;
  uint auStack_ec [4];
  uint *puStack_dc;
  int iStack_d8;
  uint auStack_d4 [4];
  uint *puStack_c4;
  int iStack_c0;
  uint auStack_bc [4];
  undefined1 auStack_ac [8];
  undefined1 auStack_a4 [12];
  uint auStack_98 [29];
  
  iVar10 = _UNK_001d229c;
  uVar11 = *(uint *)(&UNK_001d2690 + _UNK_001d229c);
  puVar12 = (uint *)(&UNK_001d2698 + _UNK_001d229c);
  puVar7 = &UNK_001d1894 + _UNK_001d22a0;
  *(uint *)(&UNK_001d2690 + _UNK_001d229c) = uVar11 + 1;
  *(uint *)(&UNK_001d2694 + iVar10) = *(int *)(&UNK_001d2694 + iVar10) + (uint)(0xfffffffe < uVar11)
  ;
  _ZN19LSMessageJsonParserC1EP9LSMessagePKc(auStack_ac,param_2,puVar7);
  iVar6 = _UNK_001d22a4;
  uVar11 = *puVar12;
  *puVar12 = uVar11 + 1;
  *(uint *)(&UNK_001d269c + iVar10) = *(int *)(&UNK_001d269c + iVar10) + (uint)(0xfffffffe < uVar11)
  ;
  iVar2 = _ZN19LSMessageJsonParser5parseEPKcP8LSHandle10ELogOption
                    (auStack_ac,&UNK_001d18ba + iVar6,param_1,1);
  iVar3 = _UNK_001d2304;
  iVar6 = _UNK_001d22e4;
  iVar10 = _UNK_001d22a8;
  if (iVar2 == 0) {
    puVar12 = auStack_98;
    uVar11 = *(uint *)(&UNK_001d2982 + _UNK_001d22e4);
    piVar5 = (int *)(&UNK_001d2986 + _UNK_001d22e4);
    *(uint *)(&UNK_001d2982 + _UNK_001d22e4) = uVar11 + 1;
    *(uint *)(&UNK_001d2986 + iVar6) = *piVar5 + (uint)(0xfffffffe < uVar11);
    goto code_r0x001d1b76;
  }
  iStack_120 = 0;
  auStack_11c[0] = auStack_11c[0] & 0xffffff00;
  iStack_108 = 0;
  auStack_104[0] = auStack_104[0] & 0xffffff00;
  uStack_131 = 0;
  puStack_124 = auStack_11c;
  puStack_10c = auStack_104;
  if (param_3 == 0) {
    uVar11 = *(uint *)(&UNK_001d2b20 + _UNK_001d2304);
    piVar5 = (int *)(&UNK_001d2b24 + _UNK_001d2304);
    *(uint *)(&UNK_001d2b20 + _UNK_001d2304) = uVar11 + 1;
    *(uint *)(&UNK_001d2b24 + iVar3) = *piVar5 + (uint)(0xfffffffe < uVar11);
    iVar3 = _Z15getPmLogContextv();
    iVar6 = _UNK_001d2344;
    iVar10 = _UNK_001d2308;
    if (iVar3 == 0) {
      uVar11 = *(uint *)(&UNK_001d2ea6 + _UNK_001d2344);
      piVar5 = (int *)(&UNK_001d2eaa + _UNK_001d2344);
      *(uint *)(&UNK_001d2ea6 + _UNK_001d2344) = uVar11 + 1;
      *(uint *)(&UNK_001d2eaa + iVar6) = *piVar5 + (uint)(0xfffffffe < uVar11);
    }
    else {
      uVar11 = *(uint *)(&UNK_001d2c94 + _UNK_001d2308);
      piVar5 = (int *)(&UNK_001d2c98 + _UNK_001d2308);
      *(uint *)(&UNK_001d2c94 + _UNK_001d2308) = uVar11 + 1;
      *(uint *)(&UNK_001d2c98 + iVar10) = *piVar5 + (uint)(0xfffffffe < uVar11);
      piVar5 = (int *)_Z15getPmLogContextv();
      iVar10 = _UNK_001d230c;
      iVar6 = *piVar5;
      uVar11 = *(uint *)(&UNK_001d2cc6 + _UNK_001d230c);
      *(uint *)(&UNK_001d2cc6 + _UNK_001d230c) = uVar11 + 1;
      if (iVar6 < 3) {
        puVar12 = auStack_98;
      }
      *(uint *)(&UNK_001d2cca + iVar10) =
           *(int *)(&UNK_001d2cca + iVar10) + (uint)(0xfffffffe < uVar11);
      if (iVar6 < 3) goto code_r0x001d1dee;
      uVar11 = *(uint *)(&UNK_001d2cce + iVar10);
      *(uint *)(&UNK_001d2cce + iVar10) = uVar11 + 1;
      *(uint *)(&UNK_001d2cd2 + iVar10) =
           *(int *)(&UNK_001d2cd2 + iVar10) + (uint)(0xfffffffe < uVar11);
    }
    uVar4 = _Z15getPmLogContextv();
    iVar10 = _UNK_001d2310;
    uVar11 = *(uint *)(&UNK_001d2d18 + _UNK_001d2310);
    piVar5 = (int *)(&UNK_001d2d1c + _UNK_001d2310);
    *(uint *)(&UNK_001d2d18 + _UNK_001d2310) = uVar11 + 1;
    iVar6 = _UNK_001d2314;
    *(uint *)(&UNK_001d2d1c + iVar10) = *piVar5 + (uint)(0xfffffffe < uVar11);
    uVar11 = *(uint *)(&UNK_001d21dc + iVar6);
    *(uint *)(&UNK_001d21dc + iVar6) = uVar11 + 1;
    puVar7 = &UNK_001d1df4 + _UNK_001d2318;
    puVar9 = &UNK_001d1dae + _UNK_001d231c;
    *(uint *)(&UNK_001d21e0 + iVar6) = *(int *)(&UNK_001d21e0 + iVar6) + (uint)(0xfffffffe < uVar11)
    ;
    func_0x00091a40(uVar4,3,0,&UNK_001d1dce + _UNK_001d2320,0,0,0,puVar9,puVar7,0x26f);
    iVar10 = _UNK_001d2324;
    puVar12 = auStack_98;
    uVar11 = *(uint *)(&UNK_001d2d92 + _UNK_001d2324);
    piVar5 = (int *)(&UNK_001d2d96 + _UNK_001d2324);
    *(uint *)(&UNK_001d2d92 + _UNK_001d2324) = uVar11 + 1;
    *(uint *)(&UNK_001d2d96 + iVar10) = *piVar5 + (uint)(0xfffffffe < uVar11);
  }
  else {
    puVar12 = auStack_98;
    uVar11 = *(uint *)(&UNK_001d2730 + _UNK_001d22a8);
    *(uint *)(&UNK_001d2730 + _UNK_001d22a8) = uVar11 + 1;
    *(uint *)(&UNK_001d2734 + iVar10) =
         *(int *)(&UNK_001d2734 + iVar10) + (uint)(0xfffffffe < uVar11);
    func_0x0009222c(auStack_130,puVar12);
    iVar10 = _UNK_001d22ac;
    uVar11 = *(uint *)(&UNK_001d2764 + _UNK_001d22ac);
    puVar7 = &UNK_001d1932 + _UNK_001d22b0;
    *(uint *)(&UNK_001d2764 + _UNK_001d22ac) = uVar11 + 1;
    *(uint *)(&UNK_001d2768 + iVar10) =
         *(int *)(&UNK_001d2768 + iVar10) + (uint)(0xfffffffe < uVar11);
    func_0x001939fc(auStack_ac,puVar7,&uStack_131);
    iVar10 = _UNK_001d22b4;
    uVar11 = *(uint *)(&UNK_001d27a4 + _UNK_001d22b4);
    puVar7 = &UNK_001d195a + _UNK_001d22b8;
    *(uint *)(&UNK_001d27a4 + _UNK_001d22b4) = uVar11 + 1;
    *(uint *)(&UNK_001d27a8 + iVar10) =
         *(int *)(&UNK_001d27a8 + iVar10) + (uint)(0xfffffffe < uVar11);
    func_0x00193b74(auStack_ac,puVar7,&puStack_10c);
    iVar6 = _UNK_001d22f8;
    iVar10 = _UNK_001d22bc;
    uStack_f0 = 0;
    auStack_ec[0] = auStack_ec[0] & 0xffffff00;
    puStack_f4 = auStack_ec;
    if (iStack_108 == 0) {
      uVar11 = *(uint *)(&UNK_001d2ae2 + _UNK_001d22f8);
      *(uint *)(&UNK_001d2ae2 + _UNK_001d22f8) = uVar11 + 1;
      *(uint *)(&UNK_001d2ae6 + iVar6) =
           *(int *)(&UNK_001d2ae6 + iVar6) + (uint)(0xfffffffe < uVar11);
      iVar3 = func_0x00091f2c(param_2);
      iVar6 = _UNK_001d2330;
      iVar10 = _UNK_001d22fc;
      if (iVar3 == 0) {
        uVar11 = *(uint *)(&UNK_001d2ce0 + _UNK_001d2330);
        *(uint *)(&UNK_001d2ce0 + _UNK_001d2330) = uVar11 + 1;
        *(uint *)(&UNK_001d2ce4 + iVar6) =
             *(int *)(&UNK_001d2ce4 + iVar6) + (uint)(0xfffffffe < uVar11);
        uVar4 = func_0x00091ad0(param_2);
        iVar10 = _UNK_001d2334;
        uVar11 = *(uint *)(&UNK_001d2d2c + _UNK_001d2334);
        *(uint *)(&UNK_001d2d2c + _UNK_001d2334) = uVar11 + 1;
        *(uint *)(&UNK_001d2d30 + iVar10) =
             *(int *)(&UNK_001d2d30 + iVar10) + (uint)(0xfffffffe < uVar11);
        func_0x00193cec(&puStack_c4,uVar4);
        iVar10 = _UNK_001d2338;
        uVar11 = *(uint *)(&UNK_001d2d64 + _UNK_001d2338);
        *(uint *)(&UNK_001d2d64 + _UNK_001d2338) = uVar11 + 1;
        *(uint *)(&UNK_001d2d68 + iVar10) =
             *(int *)(&UNK_001d2d68 + iVar10) + (uint)(0xfffffffe < uVar11);
        func_0x00091668(&puStack_f4,&puStack_c4);
        iVar10 = _UNK_001d233c;
        uVar11 = *(uint *)(&UNK_001d2d92 + _UNK_001d233c);
        *(uint *)(&UNK_001d2d92 + _UNK_001d233c) = uVar11 + 1;
        *(uint *)(&UNK_001d2d96 + iVar10) =
             *(int *)(&UNK_001d2d96 + iVar10) + (uint)(0xfffffffe < uVar11);
        if (puStack_c4 != auStack_bc) {
          func_0x00091788(puStack_c4,auStack_bc[0] + 1);
        }
        iVar10 = _UNK_001d2340;
        uVar11 = *(uint *)(&UNK_001d2dcc + _UNK_001d2340);
        *(uint *)(&UNK_001d2dcc + _UNK_001d2340) = uVar11 + 1;
        *(uint *)(&UNK_001d2dd0 + iVar10) =
             *(int *)(&UNK_001d2dd0 + iVar10) + (uint)(0xfffffffe < uVar11);
      }
      else {
        uVar11 = *(uint *)(&UNK_001d2b28 + _UNK_001d22fc);
        *(uint *)(&UNK_001d2b28 + _UNK_001d22fc) = uVar11 + 1;
        *(uint *)(&UNK_001d2b2c + iVar10) =
             *(int *)(&UNK_001d2b2c + iVar10) + (uint)(0xfffffffe < uVar11);
        uVar4 = func_0x00091efc();
        func_0x000914dc(&puStack_f4,0,uStack_f0,iVar3,uVar4);
        iVar10 = _UNK_001d2300;
        uVar11 = *(uint *)(&UNK_001d2b72 + _UNK_001d2300);
        *(uint *)(&UNK_001d2b72 + _UNK_001d2300) = uVar11 + 1;
        *(uint *)(&UNK_001d2b76 + iVar10) =
             *(int *)(&UNK_001d2b76 + iVar10) + (uint)(0xfffffffe < uVar11);
      }
    }
    else {
      uVar11 = *(uint *)(&UNK_001d27f8 + _UNK_001d22bc);
      *(uint *)(&UNK_001d27f8 + _UNK_001d22bc) = uVar11 + 1;
      *(uint *)(&UNK_001d27fc + iVar10) =
           *(int *)(&UNK_001d27fc + iVar10) + (uint)(0xfffffffe < uVar11);
      func_0x00091668(&puStack_f4,&puStack_10c);
      iVar10 = _UNK_001d22c0;
      uVar11 = *(uint *)(&UNK_001d282a + _UNK_001d22c0);
      *(uint *)(&UNK_001d282a + _UNK_001d22c0) = uVar11 + 1;
      *(uint *)(&UNK_001d282e + iVar10) =
           *(int *)(&UNK_001d282e + iVar10) + (uint)(0xfffffffe < uVar11);
    }
    func_0x000923ac(&puStack_c4,auStack_130);
    iVar10 = _UNK_001d22c4;
    uVar11 = *(uint *)(&UNK_001d28ca + _UNK_001d22c4);
    *(uint *)(&UNK_001d28ca + _UNK_001d22c4) = uVar11 + 1;
    *(uint *)(&UNK_001d28ce + iVar10) =
         *(int *)(&UNK_001d28ce + iVar10) + (uint)(0xfffffffe < uVar11);
    iVar3 = _ZN9TVManager26_mixDigitalSoundOutputImplERKbRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE
                      (param_3,&uStack_131,&puStack_f4,&puStack_c4);
    iVar10 = _UNK_001d22c8;
    uVar11 = *(uint *)(&UNK_001d2902 + _UNK_001d22c8);
    *(uint *)(&UNK_001d2902 + _UNK_001d22c8) = uVar11 + 1;
    *(uint *)(&UNK_001d2906 + iVar10) =
         *(int *)(&UNK_001d2906 + iVar10) + (uint)(0xfffffffe < uVar11);
    func_0x00091ba8(&puStack_c4);
    iVar6 = _UNK_001d22cc;
    if (iVar3 == 0) {
      uVar11 = *(uint *)(&UNK_001d2912 + iVar10);
      *(uint *)(&UNK_001d2912 + iVar10) = uVar11 + 1;
      *(uint *)(&UNK_001d2916 + iVar10) =
           *(int *)(&UNK_001d2916 + iVar10) + (uint)(0xfffffffe < uVar11);
      _ZN5utils15getErrorMessageB5cxx11EP9LSMessage(&puStack_c4,param_2);
      uVar11 = auStack_11c[0];
      puVar1 = puStack_124;
      iVar10 = _UNK_001d22ec;
      uVar8 = *(uint *)(&UNK_001d2b1e + _UNK_001d22ec);
      *(uint *)(&UNK_001d2b1e + _UNK_001d22ec) = uVar8 + 1;
      *(uint *)(&UNK_001d2b22 + iVar10) =
           *(int *)(&UNK_001d2b22 + iVar10) + (uint)(0xfffffffe < uVar8);
      if (puStack_c4 == auStack_bc) {
        iVar10 = iStack_c0;
        if (iStack_c0 != 0) {
          if (iStack_c0 == 1) {
            *(undefined1 *)puStack_124 = (undefined1)auStack_bc[0];
            iVar10 = iStack_c0;
          }
          else {
            func_0x00091ea8(puStack_124,auStack_bc);
            iVar10 = iStack_c0;
          }
        }
        *(undefined1 *)((int)puStack_124 + iVar10) = 0;
        iStack_120 = iVar10;
      }
      else {
        bVar13 = puStack_124 == auStack_11c;
        puStack_124 = puStack_c4;
        iStack_120 = iStack_c0;
        if ((bVar13) || (auStack_11c[0] = auStack_bc[0], puVar1 == (uint *)0x0)) {
          auStack_11c[0] = auStack_bc[0];
          puStack_c4 = auStack_bc;
        }
        else {
          auStack_bc[0] = uVar11;
          puStack_c4 = puVar1;
        }
      }
      iVar10 = _UNK_001d22f0;
      iStack_c0 = 0;
      *(undefined1 *)puStack_c4 = 0;
      uVar11 = *(uint *)(&UNK_001d2b70 + _UNK_001d22f0);
      *(uint *)(&UNK_001d2b70 + _UNK_001d22f0) = uVar11 + 1;
      *(uint *)(&UNK_001d2b74 + iVar10) =
           *(int *)(&UNK_001d2b74 + iVar10) + (uint)(0xfffffffe < uVar11);
      if (puStack_c4 != auStack_bc) {
        func_0x00091788(puStack_c4,auStack_bc[0] + 1);
      }
      iVar10 = _UNK_001d22f4;
      uVar11 = *(uint *)(&UNK_001d2ba2 + _UNK_001d22f4);
      *(uint *)(&UNK_001d2ba2 + _UNK_001d22f4) = uVar11 + 1;
      *(uint *)(&UNK_001d2ba6 + iVar10) =
           *(int *)(&UNK_001d2ba6 + iVar10) + (uint)(0xfffffffe < uVar11);
    }
    else {
      uVar11 = *(uint *)(&UNK_001d290a + iVar10);
      *(uint *)(&UNK_001d290a + iVar10) = uVar11 + 1;
      *(uint *)(&UNK_001d290e + iVar10) =
           *(int *)(&UNK_001d290e + iVar10) + (uint)(0xfffffffe < uVar11);
      _Z13string_printfB5cxx11PKcz(&puStack_dc,&UNK_001d1a42 + iVar6);
      uVar11 = auStack_11c[0];
      puVar1 = puStack_124;
      iVar10 = _UNK_001d22d0;
      uVar8 = *(uint *)(&UNK_001d2974 + _UNK_001d22d0);
      *(uint *)(&UNK_001d2974 + _UNK_001d22d0) = uVar8 + 1;
      *(uint *)(&UNK_001d2978 + iVar10) =
           *(int *)(&UNK_001d2978 + iVar10) + (uint)(0xfffffffe < uVar8);
      if (puStack_dc == auStack_d4) {
        iVar10 = iStack_d8;
        if (iStack_d8 != 0) {
          if (iStack_d8 == 1) {
            *(undefined1 *)puStack_124 = (undefined1)auStack_d4[0];
            iVar10 = iStack_d8;
          }
          else {
            func_0x00091ea8(puStack_124,auStack_d4);
            iVar10 = iStack_d8;
          }
        }
        *(undefined1 *)((int)puStack_124 + iVar10) = 0;
        iStack_120 = iVar10;
      }
      else {
        bVar13 = puStack_124 == auStack_11c;
        puStack_124 = puStack_dc;
        iStack_120 = iStack_d8;
        if ((bVar13) || (auStack_11c[0] = auStack_d4[0], puVar1 == (uint *)0x0)) {
          auStack_11c[0] = auStack_d4[0];
          puStack_dc = auStack_d4;
        }
        else {
          auStack_d4[0] = uVar11;
          puStack_dc = puVar1;
        }
      }
      iVar10 = _UNK_001d22d4;
      iStack_d8 = 0;
      *(undefined1 *)puStack_dc = 0;
      uVar11 = *(uint *)(&UNK_001d29c6 + _UNK_001d22d4);
      *(uint *)(&UNK_001d29c6 + _UNK_001d22d4) = uVar11 + 1;
      *(uint *)(&UNK_001d29ca + iVar10) =
           *(int *)(&UNK_001d29ca + iVar10) + (uint)(0xfffffffe < uVar11);
      if (puStack_dc != auStack_d4) {
        func_0x00091788(puStack_dc,auStack_d4[0] + 1);
      }
      iVar10 = _UNK_001d22d8;
      uVar11 = *(uint *)(&UNK_001d29f8 + _UNK_001d22d8);
      *(uint *)(&UNK_001d29f8 + _UNK_001d22d8) = uVar11 + 1;
      *(uint *)(&UNK_001d29fc + iVar10) =
           *(int *)(&UNK_001d29fc + iVar10) + (uint)(0xfffffffe < uVar11);
    }
    _ZN5utils17LSMessageResponseEP8LSHandleP9LSMessagePKcNS_9ReplyTypeEb
              (param_1,param_2,puStack_124,0,0);
    iVar10 = _UNK_001d22dc;
    uVar11 = *(uint *)(&UNK_001d2a54 + _UNK_001d22dc);
    *(uint *)(&UNK_001d2a54 + _UNK_001d22dc) = uVar11 + 1;
    *(uint *)(&UNK_001d2a58 + iVar10) =
         *(int *)(&UNK_001d2a58 + iVar10) + (uint)(0xfffffffe < uVar11);
    if (puStack_f4 != auStack_ec) {
      func_0x00091788(puStack_f4,auStack_ec[0] + 1);
    }
    iVar10 = _UNK_001d22e0;
    uVar11 = *(uint *)(&UNK_001d2a88 + _UNK_001d22e0);
    *(uint *)(&UNK_001d2a88 + _UNK_001d22e0) = uVar11 + 1;
    *(uint *)(&UNK_001d2a8c + iVar10) =
         *(int *)(&UNK_001d2a8c + iVar10) + (uint)(0xfffffffe < uVar11);
    func_0x00091ba8(auStack_130);
    uVar11 = *(uint *)(&UNK_001d2a90 + iVar10);
    *(uint *)(&UNK_001d2a90 + iVar10) = uVar11 + 1;
    *(uint *)(&UNK_001d2a94 + iVar10) =
         *(int *)(&UNK_001d2a94 + iVar10) + (uint)(0xfffffffe < uVar11);
  }
code_r0x001d1dee:
  if (puStack_10c != auStack_104) {
    func_0x00091788(puStack_10c,auStack_104[0] + 1);
  }
  iVar10 = _UNK_001d2328;
  uVar11 = *(uint *)(&UNK_001d2dc8 + _UNK_001d2328);
  piVar5 = (int *)(&UNK_001d2dcc + _UNK_001d2328);
  *(uint *)(&UNK_001d2dc8 + _UNK_001d2328) = uVar11 + 1;
  *(uint *)(&UNK_001d2dcc + iVar10) = *piVar5 + (uint)(0xfffffffe < uVar11);
  if (puStack_124 != auStack_11c) {
    func_0x00091788(puStack_124,auStack_11c[0] + 1);
  }
  iVar10 = _UNK_001d232c;
  uVar11 = *(uint *)(&UNK_001d2df8 + _UNK_001d232c);
  piVar5 = (int *)(&UNK_001d2dfc + _UNK_001d232c);
  *(uint *)(&UNK_001d2df8 + _UNK_001d232c) = uVar11 + 1;
  *(uint *)(&UNK_001d2dfc + iVar10) = *piVar5 + (uint)(0xfffffffe < uVar11);
code_r0x001d1b76:
  iVar10 = _UNK_001d22e8;
  func_0x00091944(puVar12);
  uVar11 = *(uint *)(&UNK_001d2b56 + iVar10);
  *(uint *)(&UNK_001d2b56 + iVar10) = uVar11 + 1;
  *(uint *)(&UNK_001d2b5a + iVar10) = *(int *)(&UNK_001d2b5a + iVar10) + (uint)(0xfffffffe < uVar11)
  ;
  func_0x000917d0(auStack_a4);
  uVar11 = *(uint *)(&UNK_001d2b5e + iVar10);
  *(uint *)(&UNK_001d2b5e + iVar10) = uVar11 + 1;
  *(uint *)(&UNK_001d2b62 + iVar10) = *(int *)(&UNK_001d2b62 + iVar10) + (uint)(0xfffffffe < uVar11)
  ;
  return 1;
}


