/* LOCAL STATIC EVIDENCE: Ghidra pseudocode, not original source. SHA256 170d468085255d381245693260840771b00d39c718473ec024566fb1df1ca3af image base 00010000 */

/* @001be7e4 _ZN9TVManager19loadModuleTVManagerEP10_GMainLoopP8LSHandle */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
_ZN9TVManager19loadModuleTVManagerEP10_GMainLoopP8LSHandle(undefined4 param_1,undefined4 param_2)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  int *piVar8;
  int **ppiVar9;
  int **ppiVar10;
  undefined4 uVar11;
  undefined4 *puVar12;
  bool bVar13;
  bool bVar14;
  int iStack_78;
  int **ppiStack_74;
  int *piStack_70;
  int *apiStack_6c [4];
  int *piStack_5c;
  int iStack_58;
  int aiStack_54 [4];
  undefined1 auStack_44 [32];
  
  bVar13 = 0xfffffffe < uRam00688ed0;
  uRam00688ed0 = uRam00688ed0 + 1;
  iRam00688ed4 = iRam00688ed4 + (uint)bVar13;
  iVar1 = _Z15getPmLogContextv();
  if (iVar1 == 0) {
    bVar13 = 0xfffffffe < uRam00688ed8;
    uRam00688ed8 = uRam00688ed8 + 1;
    iRam00688edc = iRam00688edc + (uint)bVar13;
LAB_001be87e:
    uVar11 = _Z15getPmLogContextv();
    bVar13 = 0xfffffffe < uRam00688ef8;
    uRam00688ef8 = uRam00688ef8 + 1;
    iRam00688efc = iRam00688efc + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680bb8;
    uRam00680bb8 = uRam00680bb8 + 1;
    iRam00680bbc = iRam00680bbc + (uint)bVar13;
    func_0x00091a40(uVar11,6,0,&UNK_0049ed3c,0,0,0,&UNK_004af564,&UNK_004ade42,0xaaa);
    bVar13 = 0xfffffffe < uRam00688f00;
    uRam00688f00 = uRam00688f00 + 1;
    iRam00688f04 = iRam00688f04 + (uint)bVar13;
  }
  else {
    bVar13 = 0xfffffffe < uRam00688ee0;
    uRam00688ee0 = uRam00688ee0 + 1;
    iRam00688ee4 = iRam00688ee4 + (uint)bVar13;
    piVar2 = (int *)_Z15getPmLogContextv();
    bVar13 = 0xfffffffe < uRam00688ee8;
    uRam00688ee8 = uRam00688ee8 + 1;
    iRam00688eec = iRam00688eec + (uint)bVar13;
    if (5 < *piVar2) {
      bVar13 = 0xfffffffe < uRam00688ef0;
      uRam00688ef0 = uRam00688ef0 + 1;
      iRam00688ef4 = iRam00688ef4 + (uint)bVar13;
      goto LAB_001be87e;
    }
  }
  func_0x00091830(auStack_44);
  bVar13 = 0xfffffffe < uRam00688f08;
  uRam00688f08 = uRam00688f08 + 1;
  iRam00688f0c = iRam00688f0c + (uint)bVar13;
  if (_ZN9TVManager10mTVManagerE != 0) {
    return 0;
  }
  bVar13 = 0xfffffffe < uRam00688f10;
  uRam00688f10 = uRam00688f10 + 1;
  iRam00688f14 = iRam00688f14 + (uint)bVar13;
  iVar1 = func_0x00091e00(0x4ec,&_ZSt7nothrow);
  if (iVar1 == 0) {
    bVar13 = 0xfffffffe < uRam00688f20;
    bVar14 = 0xfffffffe < uRam00688f30;
    _ZN9TVManager10mTVManagerE = iVar1;
    uRam00688f20 = uRam00688f20 + 1;
    iRam00688f24 = iRam00688f24 + (uint)bVar13;
    uRam00688f30 = uRam00688f30 + 1;
    iRam00688f34 = iRam00688f34 + (uint)bVar14;
    return 0;
  }
  bVar13 = 0xfffffffe < uRam00688f18;
  uRam00688f18 = uRam00688f18 + 1;
  iRam00688f1c = iRam00688f1c + (uint)bVar13;
  _ZN9TVManagerC2Ev();
  _ZN9TVManager10mTVManagerE = iVar1;
  if (*(int *)(iVar1 + 0x2a8) == 0) {
    bVar13 = 0xfffffffe < uRam00688f40;
    uRam00688f40 = uRam00688f40 + 1;
    iRam00688f44 = iRam00688f44 + (uint)bVar13;
  }
  else {
    bVar13 = 0xfffffffe < uRam00688f38;
    uRam00688f38 = uRam00688f38 + 1;
    iRam00688f3c = iRam00688f3c + (uint)bVar13;
    iVar1 = _Z15getPmLogContextv();
    if (iVar1 == 0) {
      bVar13 = 0xfffffffe < uRam00688f48;
      uRam00688f48 = uRam00688f48 + 1;
      iRam00688f4c = iRam00688f4c + (uint)bVar13;
LAB_001c2978:
      uVar11 = _Z15getPmLogContextv();
      bVar13 = 0xfffffffe < uRam00688f68;
      uRam00688f68 = uRam00688f68 + 1;
      iRam00688f6c = iRam00688f6c + (uint)bVar13;
      uVar3 = func_0x00193610();
      func_0x00091a40(uVar11,7,0,0,0,0,0,&UNK_004af58c,uVar3,0xab5);
      bVar13 = 0xfffffffe < uRam00688f70;
      uRam00688f70 = uRam00688f70 + 1;
      iRam00688f74 = iRam00688f74 + (uint)bVar13;
    }
    else {
      bVar13 = 0xfffffffe < uRam00688f50;
      uRam00688f50 = uRam00688f50 + 1;
      iRam00688f54 = iRam00688f54 + (uint)bVar13;
      piVar2 = (int *)_Z15getPmLogContextv();
      bVar13 = 0xfffffffe < uRam00688f58;
      uRam00688f58 = uRam00688f58 + 1;
      iRam00688f5c = iRam00688f5c + (uint)bVar13;
      if (6 < *piVar2) {
        bVar13 = 0xfffffffe < uRam00688f60;
        uRam00688f60 = uRam00688f60 + 1;
        iRam00688f64 = iRam00688f64 + (uint)bVar13;
        goto LAB_001c2978;
      }
    }
    _ZN13ModuleManager20subscribeModuleEventEP15ModuleInterfacebN5utils9eventTypeE
              (*(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8),_ZN9TVManager10mTVManagerE,0,0x47
              );
    bVar13 = 0xfffffffe < uRam00688f78;
    uRam00688f78 = uRam00688f78 + 1;
    iRam00688f7c = iRam00688f7c + (uint)bVar13;
    _ZN13ModuleManager25subscribeServerStatusInfoEP15ModuleInterfaceb10serverType
              (*(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8),_ZN9TVManager10mTVManagerE,0,9);
    iVar1 = _ZN9TVManager10mTVManagerE;
    bVar13 = 0xfffffffe < uRam00688f80;
    uRam00688f80 = uRam00688f80 + 1;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    iRam00688f84 = iRam00688f84 + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004af5b4);
    bVar13 = 0xfffffffe < uRam00688f88;
    uRam00688f88 = uRam00688f88 + 1;
    iRam00688f8c = iRam00688f8c + (uint)bVar13;
    func_0x00193cec(&piStack_5c,&UNK_004af5e0);
    bVar13 = 0xfffffffe < uRam00688f98;
    uRam00688f98 = uRam00688f98 + 1;
    iRam00688f9c = iRam00688f9c + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x2d,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00688fa8;
    uRam00688fa8 = uRam00688fa8 + 1;
    iRam00688fac = iRam00688fac + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00688fb0;
    uRam00688fb0 = uRam00688fb0 + 1;
    iRam00688fb4 = iRam00688fb4 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00688fb8;
    uRam00688fb8 = uRam00688fb8 + 1;
    iRam00688fbc = iRam00688fbc + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004af5b4);
    bVar13 = 0xfffffffe < uRam00688fc0;
    uRam00688fc0 = uRam00688fc0 + 1;
    iRam00688fc4 = iRam00688fc4 + (uint)bVar13;
    func_0x00193cec(&piStack_5c,&UNK_004af610);
    bVar13 = 0xfffffffe < uRam00688fd0;
    uRam00688fd0 = uRam00688fd0 + 1;
    iRam00688fd4 = iRam00688fd4 + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x2e,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00688fe0;
    uRam00688fe0 = uRam00688fe0 + 1;
    iRam00688fe4 = iRam00688fe4 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00688fe8;
    uRam00688fe8 = uRam00688fe8 + 1;
    iRam00688fec = iRam00688fec + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00688ff0;
    uRam00688ff0 = uRam00688ff0 + 1;
    iRam00688ff4 = iRam00688ff4 + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004af5b4);
    bVar13 = 0xfffffffe < uRam00688ff8;
    uRam00688ff8 = uRam00688ff8 + 1;
    iRam00688ffc = iRam00688ffc + (uint)bVar13;
    func_0x00193cec(&piStack_5c,&UNK_004af640);
    bVar13 = 0xfffffffe < uRam00689008;
    uRam00689008 = uRam00689008 + 1;
    iRam0068900c = iRam0068900c + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x2f,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689018;
    uRam00689018 = uRam00689018 + 1;
    iRam0068901c = iRam0068901c + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689020;
    uRam00689020 = uRam00689020 + 1;
    iRam00689024 = iRam00689024 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689028;
    uRam00689028 = uRam00689028 + 1;
    iRam0068902c = iRam0068902c + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004af5b4);
    bVar13 = 0xfffffffe < uRam00689030;
    uRam00689030 = uRam00689030 + 1;
    iRam00689034 = iRam00689034 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    iStack_78 = 0x49;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78,0);
    aiStack_54[0] = iStack_78;
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    func_0x00091ea8(piStack_5c,&UNK_004af670,0x49);
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689040;
    uRam00689040 = uRam00689040 + 1;
    iStack_58 = iStack_78;
    iRam00689044 = iRam00689044 + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x30,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689050;
    uRam00689050 = uRam00689050 + 1;
    iRam00689054 = iRam00689054 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689058;
    uRam00689058 = uRam00689058 + 1;
    iRam0068905c = iRam0068905c + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    bVar13 = 0xfffffffe < uRam00689060;
    uRam00689060 = uRam00689060 + 1;
    iRam00689064 = iRam00689064 + (uint)bVar13;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    func_0x00193cec(&ppiStack_74,&UNK_004af5b4);
    bVar13 = 0xfffffffe < uRam00689068;
    uRam00689068 = uRam00689068 + 1;
    iRam0068906c = iRam0068906c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    iStack_78 = 0x2d;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    aiStack_54[0] = iStack_78;
    piVar2 = piStack_5c;
    piVar6 = (int *)&UNK_004af6bc;
    do {
      piVar8 = piVar2;
      iVar4 = piVar6[1];
      iVar5 = piVar6[2];
      iVar7 = piVar6[3];
      bVar13 = piVar6 != (int *)&UNK_004af6cc;
      *piVar8 = *piVar6;
      piVar8[1] = iVar4;
      piVar8[2] = iVar5;
      piVar8[3] = iVar7;
      piVar2 = piVar8 + 4;
      piVar6 = piVar6 + 4;
    } while (bVar13);
    piVar8[6] = 0x65757274;
    piVar8[5] = 0x3a226562;
    piVar8[4] = 0x69726373;
    *(undefined1 *)(piVar8 + 7) = 0x7d;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iStack_58 = iStack_78;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689078;
    uRam00689078 = uRam00689078 + 1;
    iRam0068907c = iRam0068907c + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x31,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689088;
    uRam00689088 = uRam00689088 + 1;
    iRam0068908c = iRam0068908c + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689090;
    uRam00689090 = uRam00689090 + 1;
    iRam00689094 = iRam00689094 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689098;
    uRam00689098 = uRam00689098 + 1;
    piStack_5c = (int *)0x2a;
    iRam0068909c = iRam0068909c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    ppiStack_74 = apiStack_6c;
    ppiStack_74 = (int **)func_0x00092130(&ppiStack_74,&piStack_5c,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    apiStack_6c[0] = piStack_5c;
    ppiVar10 = ppiStack_74;
    puVar12 = (undefined4 *)&UNK_004af5b4;
    do {
      ppiVar9 = ppiVar10;
      piVar2 = (int *)puVar12[1];
      piVar6 = (int *)puVar12[2];
      piVar8 = (int *)puVar12[3];
      bVar13 = puVar12 != (undefined4 *)&UNK_004af5c4;
      *ppiVar9 = (int *)*puVar12;
      ppiVar9[1] = piVar2;
      ppiVar9[2] = piVar6;
      ppiVar9[3] = piVar8;
      ppiVar10 = ppiVar9 + 4;
      puVar12 = puVar12 + 4;
    } while (bVar13);
    ppiVar9[5] = (int *)0x69666e6f;
    ppiVar9[4] = (int *)0x43746567;
    *(undefined2 *)(ppiVar9 + 6) = 0x7367;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam006890a0;
    uRam006890a0 = uRam006890a0 + 1;
    piStack_70 = piStack_5c;
    iRam006890a4 = iRam006890a4 + (uint)bVar13;
    *(undefined1 *)((int)ppiStack_74 + (int)piStack_5c) = 0;
    func_0x00193cec(&piStack_5c,&UNK_004af6ec);
    bVar13 = 0xfffffffe < uRam006890b0;
    uRam006890b0 = uRam006890b0 + 1;
    iRam006890b4 = iRam006890b4 + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x32,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam006890c0;
    uRam006890c0 = uRam006890c0 + 1;
    iRam006890c4 = iRam006890c4 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam006890c8;
    uRam006890c8 = uRam006890c8 + 1;
    iRam006890cc = iRam006890cc + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam006890d0;
    uRam006890d0 = uRam006890d0 + 1;
    piStack_5c = (int *)0x2a;
    iRam006890d4 = iRam006890d4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    ppiStack_74 = apiStack_6c;
    ppiStack_74 = (int **)func_0x00092130(&ppiStack_74,&piStack_5c,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    apiStack_6c[0] = piStack_5c;
    ppiVar10 = ppiStack_74;
    puVar12 = (undefined4 *)&UNK_004af5b4;
    do {
      ppiVar9 = ppiVar10;
      piVar2 = (int *)puVar12[1];
      piVar6 = (int *)puVar12[2];
      piVar8 = (int *)puVar12[3];
      bVar13 = puVar12 != (undefined4 *)&UNK_004af5c4;
      *ppiVar9 = (int *)*puVar12;
      ppiVar9[1] = piVar2;
      ppiVar9[2] = piVar6;
      ppiVar9[3] = piVar8;
      ppiVar10 = ppiVar9 + 4;
      puVar12 = puVar12 + 4;
    } while (bVar13);
    ppiVar9[5] = (int *)0x69666e6f;
    ppiVar9[4] = (int *)0x43746567;
    *(undefined2 *)(ppiVar9 + 6) = 0x7367;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam006890d8;
    uRam006890d8 = uRam006890d8 + 1;
    iRam006890dc = iRam006890dc + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    piStack_70 = piStack_5c;
    *(undefined1 *)((int)ppiStack_74 + (int)piStack_5c) = 0;
    iStack_78 = 0x32;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    aiStack_54[0] = iStack_78;
    piVar2 = piStack_5c;
    piVar6 = (int *)&UNK_004af728;
    do {
      piVar8 = piVar2 + 4;
      iVar4 = piVar6[1];
      iVar5 = piVar6[2];
      iVar7 = piVar6[3];
      bVar13 = piVar6 != (int *)&UNK_004af748;
      *piVar2 = *piVar6;
      piVar2[1] = iVar4;
      piVar2[2] = iVar5;
      piVar2[3] = iVar7;
      piVar2 = piVar8;
      piVar6 = piVar6 + 4;
    } while (bVar13);
    *(undefined2 *)piVar8 = 0x7d65;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iStack_58 = iStack_78;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam006890e8;
    uRam006890e8 = uRam006890e8 + 1;
    iRam006890ec = iRam006890ec + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x2b,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam006890f8;
    uRam006890f8 = uRam006890f8 + 1;
    iRam006890fc = iRam006890fc + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689100;
    uRam00689100 = uRam00689100 + 1;
    iRam00689104 = iRam00689104 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689108;
    uRam00689108 = uRam00689108 + 1;
    iRam0068910c = iRam0068910c + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004af5b4);
    bVar13 = 0xfffffffe < uRam00689110;
    uRam00689110 = uRam00689110 + 1;
    iRam00689114 = iRam00689114 + (uint)bVar13;
    func_0x00193cec(&piStack_5c,&UNK_004af75c);
    bVar13 = 0xfffffffe < uRam00689120;
    uRam00689120 = uRam00689120 + 1;
    iRam00689124 = iRam00689124 + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x2c,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689130;
    uRam00689130 = uRam00689130 + 1;
    iRam00689134 = iRam00689134 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689138;
    uRam00689138 = uRam00689138 + 1;
    iRam0068913c = iRam0068913c + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689140;
    uRam00689140 = uRam00689140 + 1;
    iRam00689144 = iRam00689144 + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004af5b4);
    bVar13 = 0xfffffffe < uRam00689148;
    uRam00689148 = uRam00689148 + 1;
    iRam0068914c = iRam0068914c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    iStack_78 = 0x3a;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    aiStack_54[0] = iStack_78;
    piVar2 = piStack_5c;
    piVar6 = (int *)&UNK_004af794;
    do {
      piVar8 = piVar2;
      iVar4 = piVar6[1];
      iVar5 = piVar6[2];
      iVar7 = piVar6[3];
      bVar13 = piVar6 != (int *)&UNK_004af7b4;
      *piVar8 = *piVar6;
      piVar8[1] = iVar4;
      piVar8[2] = iVar5;
      piVar8[3] = iVar7;
      piVar2 = piVar8 + 4;
      piVar6 = piVar6 + 4;
    } while (bVar13);
    piVar8[5] = 0x7572743a;
    piVar8[4] = 0x22656269;
    *(undefined2 *)(piVar8 + 6) = 0x7d65;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689158;
    uRam00689158 = uRam00689158 + 1;
    iStack_58 = iStack_78;
    iRam0068915c = iRam0068915c + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,10,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689168;
    uRam00689168 = uRam00689168 + 1;
    iRam0068916c = iRam0068916c + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689170;
    uRam00689170 = uRam00689170 + 1;
    iRam00689174 = iRam00689174 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689178;
    uRam00689178 = uRam00689178 + 1;
    iRam0068917c = iRam0068917c + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004af5b4);
    bVar13 = 0xfffffffe < uRam00689180;
    uRam00689180 = uRam00689180 + 1;
    iRam00689184 = iRam00689184 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    iStack_78 = 0x57;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78,0);
    aiStack_54[0] = iStack_78;
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    func_0x00091ea8(piStack_5c,&UNK_004af7d0,0x57);
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689190;
    uRam00689190 = uRam00689190 + 1;
    iStack_58 = iStack_78;
    iRam00689194 = iRam00689194 + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x28,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam006891a0;
    uRam006891a0 = uRam006891a0 + 1;
    iRam006891a4 = iRam006891a4 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam006891a8;
    uRam006891a8 = uRam006891a8 + 1;
    iRam006891ac = iRam006891ac + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    bVar13 = 0xfffffffe < uRam006891b0;
    uRam006891b0 = uRam006891b0 + 1;
    iRam006891b4 = iRam006891b4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    piStack_5c = (int *)0x2a;
    ppiStack_74 = apiStack_6c;
    ppiStack_74 = (int **)func_0x00092130(&ppiStack_74,&piStack_5c,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    apiStack_6c[0] = piStack_5c;
    ppiVar10 = ppiStack_74;
    puVar12 = (undefined4 *)&UNK_004af5b4;
    do {
      ppiVar9 = ppiVar10;
      piVar2 = (int *)puVar12[1];
      piVar6 = (int *)puVar12[2];
      piVar8 = (int *)puVar12[3];
      bVar13 = puVar12 != (undefined4 *)&UNK_004af5c4;
      *ppiVar9 = (int *)*puVar12;
      ppiVar9[1] = piVar2;
      ppiVar9[2] = piVar6;
      ppiVar9[3] = piVar8;
      ppiVar10 = ppiVar9 + 4;
      puVar12 = puVar12 + 4;
    } while (bVar13);
    ppiVar9[5] = (int *)0x69666e6f;
    ppiVar9[4] = (int *)0x43746567;
    *(undefined2 *)(ppiVar9 + 6) = 0x7367;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam006891b8;
    uRam006891b8 = uRam006891b8 + 1;
    iRam006891bc = iRam006891bc + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    piStack_70 = piStack_5c;
    *(undefined1 *)((int)ppiStack_74 + (int)piStack_5c) = 0;
    iStack_78 = 0x3c;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    aiStack_54[0] = iStack_78;
    piVar2 = piStack_5c;
    piVar6 = (int *)&UNK_004af828;
    do {
      piVar8 = piVar2;
      iVar4 = piVar6[1];
      iVar5 = piVar6[2];
      iVar7 = piVar6[3];
      bVar13 = piVar6 != (int *)&UNK_004af848;
      *piVar8 = *piVar6;
      piVar8[1] = iVar4;
      piVar8[2] = iVar5;
      piVar8[3] = iVar7;
      piVar2 = piVar8 + 4;
      piVar6 = piVar6 + 4;
    } while (bVar13);
    piVar8[5] = 0x743a2265;
    piVar8[4] = 0x62697263;
    piVar8[6] = 0x7d657572;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iStack_58 = iStack_78;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam006891c8;
    uRam006891c8 = uRam006891c8 + 1;
    iRam006891cc = iRam006891cc + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0xb,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam006891d8;
    uRam006891d8 = uRam006891d8 + 1;
    iRam006891dc = iRam006891dc + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam006891e0;
    uRam006891e0 = uRam006891e0 + 1;
    iRam006891e4 = iRam006891e4 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam006891e8;
    uRam006891e8 = uRam006891e8 + 1;
    iRam006891ec = iRam006891ec + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004af5b4);
    bVar13 = 0xfffffffe < uRam006891f0;
    uRam006891f0 = uRam006891f0 + 1;
    iRam006891f4 = iRam006891f4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    iStack_78 = 0x3a;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    aiStack_54[0] = iStack_78;
    piVar2 = piStack_5c;
    piVar6 = (int *)&UNK_004af868;
    do {
      piVar8 = piVar2;
      iVar4 = piVar6[1];
      iVar5 = piVar6[2];
      iVar7 = piVar6[3];
      bVar13 = piVar6 != (int *)&UNK_004af888;
      *piVar8 = *piVar6;
      piVar8[1] = iVar4;
      piVar8[2] = iVar5;
      piVar8[3] = iVar7;
      piVar2 = piVar8 + 4;
      piVar6 = piVar6 + 4;
    } while (bVar13);
    piVar8[5] = 0x7572743a;
    piVar8[4] = 0x22656269;
    *(undefined2 *)(piVar8 + 6) = 0x7d65;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689200;
    uRam00689200 = uRam00689200 + 1;
    iStack_58 = iStack_78;
    iRam00689204 = iRam00689204 + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0xc,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689210;
    uRam00689210 = uRam00689210 + 1;
    iRam00689214 = iRam00689214 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689218;
    uRam00689218 = uRam00689218 + 1;
    iRam0068921c = iRam0068921c + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689220;
    uRam00689220 = uRam00689220 + 1;
    iRam00689224 = iRam00689224 + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004af5b4);
    bVar13 = 0xfffffffe < uRam00689228;
    uRam00689228 = uRam00689228 + 1;
    iRam0068922c = iRam0068922c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    iStack_78 = 0x36;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    aiStack_54[0] = iStack_78;
    piVar2 = piStack_5c;
    piVar6 = (int *)&UNK_004af8a4;
    do {
      piVar8 = piVar2;
      iVar4 = piVar6[1];
      iVar5 = piVar6[2];
      iVar7 = piVar6[3];
      bVar13 = piVar6 != (int *)&UNK_004af8c4;
      *piVar8 = *piVar6;
      piVar8[1] = iVar4;
      piVar8[2] = iVar5;
      piVar8[3] = iVar7;
      piVar2 = piVar8 + 4;
      piVar6 = piVar6 + 4;
    } while (bVar13);
    piVar8[4] = 0x7572743a;
    *(undefined2 *)(piVar8 + 5) = 0x7d65;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689238;
    uRam00689238 = uRam00689238 + 1;
    iStack_58 = iStack_78;
    iRam0068923c = iRam0068923c + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0xd,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689248;
    uRam00689248 = uRam00689248 + 1;
    iRam0068924c = iRam0068924c + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689250;
    uRam00689250 = uRam00689250 + 1;
    iRam00689254 = iRam00689254 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689258;
    uRam00689258 = uRam00689258 + 1;
    piStack_5c = (int *)0x2a;
    iRam0068925c = iRam0068925c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    ppiStack_74 = apiStack_6c;
    ppiStack_74 = (int **)func_0x00092130(&ppiStack_74,&piStack_5c,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    apiStack_6c[0] = piStack_5c;
    ppiVar10 = ppiStack_74;
    puVar12 = (undefined4 *)&UNK_004af5b4;
    do {
      ppiVar9 = ppiVar10;
      piVar2 = (int *)puVar12[1];
      piVar6 = (int *)puVar12[2];
      piVar8 = (int *)puVar12[3];
      bVar13 = puVar12 != (undefined4 *)&UNK_004af5c4;
      *ppiVar9 = (int *)*puVar12;
      ppiVar9[1] = piVar2;
      ppiVar9[2] = piVar6;
      ppiVar9[3] = piVar8;
      ppiVar10 = ppiVar9 + 4;
      puVar12 = puVar12 + 4;
    } while (bVar13);
    ppiVar9[5] = (int *)0x69666e6f;
    ppiVar9[4] = (int *)0x43746567;
    *(undefined2 *)(ppiVar9 + 6) = 0x7367;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689260;
    uRam00689260 = uRam00689260 + 1;
    piStack_70 = piStack_5c;
    iRam00689264 = iRam00689264 + (uint)bVar13;
    *(undefined1 *)((int)ppiStack_74 + (int)piStack_5c) = 0;
    func_0x00193cec(&piStack_5c,&UNK_004af8dc);
    bVar13 = 0xfffffffe < uRam00689270;
    uRam00689270 = uRam00689270 + 1;
    iRam00689274 = iRam00689274 + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x1e,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689280;
    uRam00689280 = uRam00689280 + 1;
    iRam00689284 = iRam00689284 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689288;
    uRam00689288 = uRam00689288 + 1;
    iRam0068928c = iRam0068928c + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689290;
    uRam00689290 = uRam00689290 + 1;
    iRam00689294 = iRam00689294 + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004af5b4);
    bVar13 = 0xfffffffe < uRam00689298;
    uRam00689298 = uRam00689298 + 1;
    iRam0068929c = iRam0068929c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    iStack_78 = 0x36;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    aiStack_54[0] = iStack_78;
    piVar2 = piStack_5c;
    piVar6 = (int *)&UNK_004af918;
    do {
      piVar8 = piVar2;
      iVar4 = piVar6[1];
      iVar5 = piVar6[2];
      iVar7 = piVar6[3];
      bVar13 = piVar6 != (int *)&UNK_004af938;
      *piVar8 = *piVar6;
      piVar8[1] = iVar4;
      piVar8[2] = iVar5;
      piVar8[3] = iVar7;
      piVar2 = piVar8 + 4;
      piVar6 = piVar6 + 4;
    } while (bVar13);
    piVar8[4] = 0x7572743a;
    *(undefined2 *)(piVar8 + 5) = 0x7d65;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam006892a8;
    uRam006892a8 = uRam006892a8 + 1;
    iStack_58 = iStack_78;
    iRam006892ac = iRam006892ac + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x48,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam006892b8;
    uRam006892b8 = uRam006892b8 + 1;
    iRam006892bc = iRam006892bc + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam006892c0;
    uRam006892c0 = uRam006892c0 + 1;
    iRam006892c4 = iRam006892c4 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    bVar13 = 0xfffffffe < uRam006892c8;
    uRam006892c8 = uRam006892c8 + 1;
    iRam006892cc = iRam006892cc + (uint)bVar13;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    func_0x00193cec(&ppiStack_74,&UNK_004af5b4);
    bVar13 = 0xfffffffe < uRam006892d0;
    uRam006892d0 = uRam006892d0 + 1;
    iRam006892d4 = iRam006892d4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    iStack_78 = 0x3f;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    aiStack_54[0] = iStack_78;
    piVar2 = piStack_5c;
    piVar6 = (int *)&UNK_004af950;
    do {
      piVar8 = piVar2;
      iVar4 = piVar6[1];
      iVar5 = piVar6[2];
      iVar7 = piVar6[3];
      bVar13 = piVar6 != (int *)&UNK_004af970;
      *piVar8 = *piVar6;
      piVar8[1] = iVar4;
      piVar8[2] = iVar5;
      piVar8[3] = iVar7;
      piVar2 = piVar8 + 4;
      piVar6 = piVar6 + 4;
    } while (bVar13);
    piVar8[6] = 0x72743a22;
    piVar8[5] = 0x65626972;
    piVar8[4] = 0x63736275;
    *(undefined2 *)(piVar8 + 7) = 0x6575;
    *(undefined1 *)((int)piVar8 + 0x1e) = 0x7d;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iStack_58 = iStack_78;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam006892e0;
    uRam006892e0 = uRam006892e0 + 1;
    iRam006892e4 = iRam006892e4 + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x33,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam006892f0;
    uRam006892f0 = uRam006892f0 + 1;
    iRam006892f4 = iRam006892f4 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam006892f8;
    uRam006892f8 = uRam006892f8 + 1;
    iRam006892fc = iRam006892fc + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689300;
    uRam00689300 = uRam00689300 + 1;
    piStack_5c = (int *)0x2a;
    iRam00689304 = iRam00689304 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    ppiStack_74 = apiStack_6c;
    ppiStack_74 = (int **)func_0x00092130(&ppiStack_74,&piStack_5c,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    apiStack_6c[0] = piStack_5c;
    ppiVar10 = ppiStack_74;
    puVar12 = (undefined4 *)&UNK_004af5b4;
    do {
      ppiVar9 = ppiVar10;
      piVar2 = (int *)puVar12[1];
      piVar6 = (int *)puVar12[2];
      piVar8 = (int *)puVar12[3];
      bVar13 = puVar12 != (undefined4 *)&UNK_004af5c4;
      *ppiVar9 = (int *)*puVar12;
      ppiVar9[1] = piVar2;
      ppiVar9[2] = piVar6;
      ppiVar9[3] = piVar8;
      ppiVar10 = ppiVar9 + 4;
      puVar12 = puVar12 + 4;
    } while (bVar13);
    ppiVar9[5] = (int *)0x69666e6f;
    ppiVar9[4] = (int *)0x43746567;
    *(undefined2 *)(ppiVar9 + 6) = 0x7367;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689308;
    uRam00689308 = uRam00689308 + 1;
    iRam0068930c = iRam0068930c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    piStack_70 = piStack_5c;
    *(undefined1 *)((int)ppiStack_74 + (int)piStack_5c) = 0;
    iStack_78 = 0x3a;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    aiStack_54[0] = iStack_78;
    piVar2 = piStack_5c;
    piVar6 = (int *)&UNK_004af990;
    do {
      piVar8 = piVar2;
      iVar4 = piVar6[1];
      iVar5 = piVar6[2];
      iVar7 = piVar6[3];
      bVar13 = piVar6 != (int *)&UNK_004af9b0;
      *piVar8 = *piVar6;
      piVar8[1] = iVar4;
      piVar8[2] = iVar5;
      piVar8[3] = iVar7;
      piVar2 = piVar8 + 4;
      piVar6 = piVar6 + 4;
    } while (bVar13);
    piVar8[5] = 0x7572743a;
    piVar8[4] = 0x22656269;
    *(undefined2 *)(piVar8 + 6) = 0x7d65;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689318;
    uRam00689318 = uRam00689318 + 1;
    iStack_58 = iStack_78;
    iRam0068931c = iRam0068931c + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x3b,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689328;
    uRam00689328 = uRam00689328 + 1;
    iRam0068932c = iRam0068932c + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689330;
    uRam00689330 = uRam00689330 + 1;
    iRam00689334 = iRam00689334 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689338;
    uRam00689338 = uRam00689338 + 1;
    piStack_5c = (int *)0x2a;
    iRam0068933c = iRam0068933c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    ppiStack_74 = apiStack_6c;
    ppiStack_74 = (int **)func_0x00092130(&ppiStack_74,&piStack_5c,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    apiStack_6c[0] = piStack_5c;
    ppiVar10 = ppiStack_74;
    puVar12 = (undefined4 *)&UNK_004af5b4;
    do {
      ppiVar9 = ppiVar10;
      piVar2 = (int *)puVar12[1];
      piVar6 = (int *)puVar12[2];
      piVar8 = (int *)puVar12[3];
      bVar13 = puVar12 != (undefined4 *)&UNK_004af5c4;
      *ppiVar9 = (int *)*puVar12;
      ppiVar9[1] = piVar2;
      ppiVar9[2] = piVar6;
      ppiVar9[3] = piVar8;
      ppiVar10 = ppiVar9 + 4;
      puVar12 = puVar12 + 4;
    } while (bVar13);
    ppiVar9[5] = (int *)0x69666e6f;
    ppiVar9[4] = (int *)0x43746567;
    *(undefined2 *)(ppiVar9 + 6) = 0x7367;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689340;
    uRam00689340 = uRam00689340 + 1;
    iRam00689344 = iRam00689344 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    piStack_70 = piStack_5c;
    *(undefined1 *)((int)ppiStack_74 + (int)piStack_5c) = 0;
    iStack_78 = 0x56;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78);
    aiStack_54[0] = iStack_78;
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    func_0x00091ea8(piStack_5c,&UNK_004af9cc,0x56);
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689350;
    uRam00689350 = uRam00689350 + 1;
    iStack_58 = iStack_78;
    iRam00689354 = iRam00689354 + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x46,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689360;
    uRam00689360 = uRam00689360 + 1;
    iRam00689364 = iRam00689364 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689368;
    uRam00689368 = uRam00689368 + 1;
    iRam0068936c = iRam0068936c + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689370;
    uRam00689370 = uRam00689370 + 1;
    iRam00689374 = iRam00689374 + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004af5b4);
    bVar13 = 0xfffffffe < uRam00689378;
    uRam00689378 = uRam00689378 + 1;
    iRam0068937c = iRam0068937c + (uint)bVar13;
    func_0x00193cec(&piStack_5c,&UNK_004afa24);
    bVar13 = 0xfffffffe < uRam00689388;
    uRam00689388 = uRam00689388 + 1;
    iRam0068938c = iRam0068938c + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x3c,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689398;
    uRam00689398 = uRam00689398 + 1;
    iRam0068939c = iRam0068939c + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam006893a0;
    uRam006893a0 = uRam006893a0 + 1;
    iRam006893a4 = iRam006893a4 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam006893a8;
    uRam006893a8 = uRam006893a8 + 1;
    piStack_5c = (int *)0x2a;
    iRam006893ac = iRam006893ac + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    ppiStack_74 = apiStack_6c;
    ppiStack_74 = (int **)func_0x00092130(&ppiStack_74,&piStack_5c,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    apiStack_6c[0] = piStack_5c;
    ppiVar10 = ppiStack_74;
    puVar12 = (undefined4 *)&UNK_004af5b4;
    do {
      ppiVar9 = ppiVar10;
      piVar2 = (int *)puVar12[1];
      piVar6 = (int *)puVar12[2];
      piVar8 = (int *)puVar12[3];
      bVar13 = puVar12 != (undefined4 *)&UNK_004af5c4;
      *ppiVar9 = (int *)*puVar12;
      ppiVar9[1] = piVar2;
      ppiVar9[2] = piVar6;
      ppiVar9[3] = piVar8;
      ppiVar10 = ppiVar9 + 4;
      puVar12 = puVar12 + 4;
    } while (bVar13);
    ppiVar9[5] = (int *)0x69666e6f;
    ppiVar9[4] = (int *)0x43746567;
    *(undefined2 *)(ppiVar9 + 6) = 0x7367;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam006893b0;
    uRam006893b0 = uRam006893b0 + 1;
    piStack_70 = piStack_5c;
    iRam006893b4 = iRam006893b4 + (uint)bVar13;
    *(undefined1 *)((int)ppiStack_74 + (int)piStack_5c) = 0;
    func_0x00193cec(&piStack_5c,&UNK_004afa84);
    bVar13 = 0xfffffffe < uRam006893c0;
    uRam006893c0 = uRam006893c0 + 1;
    iRam006893c4 = iRam006893c4 + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x47,9,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam006893d0;
    uRam006893d0 = uRam006893d0 + 1;
    iRam006893d4 = iRam006893d4 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam006893d8;
    uRam006893d8 = uRam006893d8 + 1;
    iRam006893dc = iRam006893dc + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam006893e0;
    uRam006893e0 = uRam006893e0 + 1;
    iRam006893e4 = iRam006893e4 + (uint)bVar13;
    iVar1 = _Z15getPmLogContextv();
    if (iVar1 == 0) {
      bVar13 = 0xfffffffe < uRam006893e8;
      uRam006893e8 = uRam006893e8 + 1;
      iRam006893ec = iRam006893ec + (uint)bVar13;
LAB_001c0e5a:
      uVar11 = _Z15getPmLogContextv();
      bVar13 = 0xfffffffe < uRam00689408;
      uRam00689408 = uRam00689408 + 1;
      iRam0068940c = iRam0068940c + (uint)bVar13;
      uVar3 = func_0x00193610();
      func_0x00091a40(uVar11,7,0,0,0,0,0,&UNK_004afae4,uVar3,0xafb);
      bVar13 = 0xfffffffe < uRam00689410;
      uRam00689410 = uRam00689410 + 1;
      iRam00689414 = iRam00689414 + (uint)bVar13;
    }
    else {
      bVar13 = 0xfffffffe < uRam006893f0;
      uRam006893f0 = uRam006893f0 + 1;
      iRam006893f4 = iRam006893f4 + (uint)bVar13;
      piVar2 = (int *)_Z15getPmLogContextv();
      bVar13 = 0xfffffffe < uRam006893f8;
      uRam006893f8 = uRam006893f8 + 1;
      iRam006893fc = iRam006893fc + (uint)bVar13;
      if (6 < *piVar2) {
        bVar13 = 0xfffffffe < uRam00689400;
        uRam00689400 = uRam00689400 + 1;
        iRam00689404 = iRam00689404 + (uint)bVar13;
        goto LAB_001c0e5a;
      }
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    piStack_5c = (int *)0x32;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    ppiStack_74 = apiStack_6c;
    ppiStack_74 = (int **)func_0x00092130(&ppiStack_74,&piStack_5c,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    apiStack_6c[0] = piStack_5c;
    ppiVar10 = ppiStack_74;
    puVar12 = (undefined4 *)&UNK_004afb08;
    do {
      ppiVar9 = ppiVar10 + 4;
      piVar2 = (int *)puVar12[1];
      piVar6 = (int *)puVar12[2];
      piVar8 = (int *)puVar12[3];
      bVar13 = puVar12 != (undefined4 *)&UNK_004afb28;
      *ppiVar10 = (int *)*puVar12;
      ppiVar10[1] = piVar2;
      ppiVar10[2] = piVar6;
      ppiVar10[3] = piVar8;
      ppiVar10 = ppiVar9;
      puVar12 = puVar12 + 4;
    } while (bVar13);
    *(undefined2 *)ppiVar9 = 0x6574;
    piStack_70 = piStack_5c;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689418;
    uRam00689418 = uRam00689418 + 1;
    iRam0068941c = iRam0068941c + (uint)bVar13;
    *(undefined1 *)((int)ppiStack_74 + (int)piStack_5c) = 0;
    func_0x00193cec(&piStack_5c,&UNK_004afb3c);
    bVar13 = 0xfffffffe < uRam00689428;
    uRam00689428 = uRam00689428 + 1;
    iRam0068942c = iRam0068942c + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,9,8,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689438;
    uRam00689438 = uRam00689438 + 1;
    iRam0068943c = iRam0068943c + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689440;
    uRam00689440 = uRam00689440 + 1;
    iRam00689444 = iRam00689444 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    bVar13 = 0xfffffffe < uRam00689448;
    uRam00689448 = uRam00689448 + 1;
    iRam0068944c = iRam0068944c + (uint)bVar13;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    func_0x00193cec(&ppiStack_74,&UNK_004afb50);
    bVar13 = 0xfffffffe < uRam00689450;
    uRam00689450 = uRam00689450 + 1;
    iRam00689454 = iRam00689454 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    iStack_78 = 0x2c;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    aiStack_54[0] = iStack_78;
    piVar2 = piStack_5c;
    piVar6 = (int *)&UNK_004afb88;
    do {
      piVar8 = piVar2;
      iVar4 = piVar6[1];
      iVar5 = piVar6[2];
      iVar7 = piVar6[3];
      bVar13 = piVar6 != (int *)&UNK_004afb98;
      *piVar8 = *piVar6;
      piVar8[1] = iVar4;
      piVar8[2] = iVar5;
      piVar8[3] = iVar7;
      piVar2 = piVar8 + 4;
      piVar6 = piVar6 + 4;
    } while (bVar13);
    piVar8[5] = 0x743a2265;
    piVar8[4] = 0x62697263;
    piVar8[6] = 0x7d657572;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689460;
    uRam00689460 = uRam00689460 + 1;
    iStack_58 = iStack_78;
    iRam00689464 = iRam00689464 + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0xe,0xe,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689470;
    uRam00689470 = uRam00689470 + 1;
    iRam00689474 = iRam00689474 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689478;
    uRam00689478 = uRam00689478 + 1;
    iRam0068947c = iRam0068947c + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    bVar13 = 0xfffffffe < uRam00689480;
    uRam00689480 = uRam00689480 + 1;
    iRam00689484 = iRam00689484 + (uint)bVar13;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    func_0x00193cec(&ppiStack_74,&UNK_004afb50);
    bVar13 = 0xfffffffe < uRam00689488;
    uRam00689488 = uRam00689488 + 1;
    iRam0068948c = iRam0068948c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    iStack_78 = 0x2e;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    aiStack_54[0] = iStack_78;
    piVar2 = piStack_5c;
    piVar6 = (int *)&UNK_004afbb8;
    do {
      piVar8 = piVar2;
      iVar4 = piVar6[1];
      iVar5 = piVar6[2];
      iVar7 = piVar6[3];
      bVar13 = piVar6 != (int *)&UNK_004afbc8;
      *piVar8 = *piVar6;
      piVar8[1] = iVar4;
      piVar8[2] = iVar5;
      piVar8[3] = iVar7;
      piVar2 = piVar8 + 4;
      piVar6 = piVar6 + 4;
    } while (bVar13);
    piVar8[6] = 0x7572743a;
    piVar8[4] = 0x72637362;
    piVar8[5] = 0x22656269;
    *(undefined2 *)(piVar8 + 7) = 0x7d65;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iStack_58 = iStack_78;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689498;
    uRam00689498 = uRam00689498 + 1;
    iRam0068949c = iRam0068949c + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0xf,0xe,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam006894a8;
    uRam006894a8 = uRam006894a8 + 1;
    iRam006894ac = iRam006894ac + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam006894b0;
    uRam006894b0 = uRam006894b0 + 1;
    iRam006894b4 = iRam006894b4 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam006894b8;
    uRam006894b8 = uRam006894b8 + 1;
    piStack_5c = (int *)0x35;
    iRam006894bc = iRam006894bc + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    ppiStack_74 = apiStack_6c;
    ppiStack_74 = (int **)func_0x00092130(&ppiStack_74,&piStack_5c,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    apiStack_6c[0] = piStack_5c;
    ppiVar10 = ppiStack_74;
    puVar12 = (undefined4 *)&UNK_004afb50;
    do {
      ppiVar9 = ppiVar10;
      piVar2 = (int *)puVar12[1];
      piVar6 = (int *)puVar12[2];
      piVar8 = (int *)puVar12[3];
      bVar13 = puVar12 != (undefined4 *)&UNK_004afb70;
      *ppiVar9 = (int *)*puVar12;
      ppiVar9[1] = piVar2;
      ppiVar9[2] = piVar6;
      ppiVar9[3] = piVar8;
      ppiVar10 = ppiVar9 + 4;
      puVar12 = puVar12 + 4;
    } while (bVar13);
    ppiVar9[4] = (int *)0x704f7972;
    *(undefined1 *)(ppiVar9 + 5) = 0x74;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam006894c0;
    uRam006894c0 = uRam006894c0 + 1;
    piStack_70 = piStack_5c;
    iRam006894c4 = iRam006894c4 + (uint)bVar13;
    *(undefined1 *)((int)ppiStack_74 + (int)piStack_5c) = 0;
    func_0x00193cec(&piStack_5c,&UNK_004afbe8);
    bVar13 = 0xfffffffe < uRam006894d0;
    uRam006894d0 = uRam006894d0 + 1;
    iRam006894d4 = iRam006894d4 + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x10,0xe,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam006894e0;
    uRam006894e0 = uRam006894e0 + 1;
    iRam006894e4 = iRam006894e4 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam006894e8;
    uRam006894e8 = uRam006894e8 + 1;
    iRam006894ec = iRam006894ec + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam006894f0;
    uRam006894f0 = uRam006894f0 + 1;
    iRam006894f4 = iRam006894f4 + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004afc18);
    bVar13 = 0xfffffffe < uRam006894f8;
    uRam006894f8 = uRam006894f8 + 1;
    iRam006894fc = iRam006894fc + (uint)bVar13;
    func_0x00193cec(&piStack_5c,&UNK_004afb3c);
    bVar13 = 0xfffffffe < uRam00689508;
    uRam00689508 = uRam00689508 + 1;
    iRam0068950c = iRam0068950c + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x11,0xb,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689518;
    uRam00689518 = uRam00689518 + 1;
    iRam0068951c = iRam0068951c + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689520;
    uRam00689520 = uRam00689520 + 1;
    iRam00689524 = iRam00689524 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689528;
    uRam00689528 = uRam00689528 + 1;
    iRam0068952c = iRam0068952c + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004afc48);
    bVar13 = 0xfffffffe < uRam00689530;
    uRam00689530 = uRam00689530 + 1;
    iRam00689534 = iRam00689534 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    iStack_78 = 0x12;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    aiStack_54[0] = iStack_78;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689540;
    uRam00689540 = uRam00689540 + 1;
    iRam00689544 = iRam00689544 + (uint)bVar13;
    piStack_5c[3] = 0x7572743a;
    *piStack_5c = 0x7573227b;
    piStack_5c[1] = 0x72637362;
    piStack_5c[2] = 0x22656269;
    *(undefined2 *)(piStack_5c + 4) = 0x7d65;
    iStack_58 = iStack_78;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x24,0x11,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689550;
    uRam00689550 = uRam00689550 + 1;
    iRam00689554 = iRam00689554 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689558;
    uRam00689558 = uRam00689558 + 1;
    iRam0068955c = iRam0068955c + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689560;
    uRam00689560 = uRam00689560 + 1;
    iRam00689564 = iRam00689564 + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004afc80);
    bVar13 = 0xfffffffe < uRam00689568;
    uRam00689568 = uRam00689568 + 1;
    iRam0068956c = iRam0068956c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    iStack_78 = 0x26;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    aiStack_54[0] = iStack_78;
    piVar2 = piStack_5c;
    piVar6 = (int *)&UNK_004afcc0;
    do {
      piVar8 = piVar2;
      iVar4 = piVar6[1];
      iVar5 = piVar6[2];
      iVar7 = piVar6[3];
      bVar13 = piVar6 != (int *)&UNK_004afcd0;
      *piVar8 = *piVar6;
      piVar8[1] = iVar4;
      piVar8[2] = iVar5;
      piVar8[3] = iVar7;
      piVar2 = piVar8 + 4;
      piVar6 = piVar6 + 4;
    } while (bVar13);
    piVar8[4] = 0x7572743a;
    *(undefined2 *)(piVar8 + 5) = 0x7d65;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689578;
    uRam00689578 = uRam00689578 + 1;
    iStack_58 = iStack_78;
    iRam0068957c = iRam0068957c + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x3e,0x13,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689588;
    uRam00689588 = uRam00689588 + 1;
    iRam0068958c = iRam0068958c + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689590;
    uRam00689590 = uRam00689590 + 1;
    iRam00689594 = iRam00689594 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689598;
    uRam00689598 = uRam00689598 + 1;
    piStack_5c = (int *)0x30;
    iRam0068959c = iRam0068959c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    ppiStack_74 = apiStack_6c;
    ppiStack_74 = (int **)func_0x00092130(&ppiStack_74,&piStack_5c,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    apiStack_6c[0] = piStack_5c;
    ppiVar10 = ppiStack_74;
    puVar12 = (undefined4 *)&UNK_004afce8;
    do {
      piVar2 = (int *)puVar12[1];
      piVar6 = (int *)puVar12[2];
      piVar8 = (int *)puVar12[3];
      bVar13 = puVar12 != (undefined4 *)&UNK_004afd08;
      *ppiVar10 = (int *)*puVar12;
      ppiVar10[1] = piVar2;
      ppiVar10[2] = piVar6;
      ppiVar10[3] = piVar8;
      ppiVar10 = ppiVar10 + 4;
      puVar12 = puVar12 + 4;
    } while (bVar13);
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam006895a0;
    uRam006895a0 = uRam006895a0 + 1;
    iRam006895a4 = iRam006895a4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    piStack_70 = piStack_5c;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    *(undefined1 *)((int)ppiStack_74 + (int)piStack_5c) = 0;
    iStack_78 = 0x12;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    aiStack_54[0] = iStack_78;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam006895b0;
    uRam006895b0 = uRam006895b0 + 1;
    iRam006895b4 = iRam006895b4 + (uint)bVar13;
    piStack_5c[3] = 0x7572743a;
    *piStack_5c = 0x7573227b;
    piStack_5c[1] = 0x72637362;
    piStack_5c[2] = 0x22656269;
    *(undefined2 *)(piStack_5c + 4) = 0x7d65;
    iStack_58 = iStack_78;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x36,0x15,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam006895c0;
    uRam006895c0 = uRam006895c0 + 1;
    iRam006895c4 = iRam006895c4 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam006895c8;
    uRam006895c8 = uRam006895c8 + 1;
    iRam006895cc = iRam006895cc + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam006895d0;
    uRam006895d0 = uRam006895d0 + 1;
    iRam006895d4 = iRam006895d4 + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004afd1c);
    bVar13 = 0xfffffffe < uRam006895d8;
    uRam006895d8 = uRam006895d8 + 1;
    iRam006895dc = iRam006895dc + (uint)bVar13;
    func_0x00193cec(&piStack_5c,&UNK_004afd54);
    bVar13 = 0xfffffffe < uRam006895e8;
    uRam006895e8 = uRam006895e8 + 1;
    iRam006895ec = iRam006895ec + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x37,0x16,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam006895f8;
    uRam006895f8 = uRam006895f8 + 1;
    iRam006895fc = iRam006895fc + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689600;
    uRam00689600 = uRam00689600 + 1;
    iRam00689604 = iRam00689604 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689608;
    uRam00689608 = uRam00689608 + 1;
    iRam0068960c = iRam0068960c + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004afd90);
    bVar13 = 0xfffffffe < uRam00689610;
    uRam00689610 = uRam00689610 + 1;
    iRam00689614 = iRam00689614 + (uint)bVar13;
    func_0x00193cec(&piStack_5c,&UNK_004afb3c);
    bVar13 = 0xfffffffe < uRam00689620;
    uRam00689620 = uRam00689620 + 1;
    iRam00689624 = iRam00689624 + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x38,0x14,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689630;
    uRam00689630 = uRam00689630 + 1;
    iRam00689634 = iRam00689634 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689638;
    uRam00689638 = uRam00689638 + 1;
    iRam0068963c = iRam0068963c + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689640;
    uRam00689640 = uRam00689640 + 1;
    iRam00689644 = iRam00689644 + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004afdc8);
    bVar13 = 0xfffffffe < uRam00689648;
    uRam00689648 = uRam00689648 + 1;
    iRam0068964c = iRam0068964c + (uint)bVar13;
    func_0x00193cec(&piStack_5c,&UNK_004afb3c);
    bVar13 = 0xfffffffe < uRam00689658;
    uRam00689658 = uRam00689658 + 1;
    iRam0068965c = iRam0068965c + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x39,0x17,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689668;
    uRam00689668 = uRam00689668 + 1;
    iRam0068966c = iRam0068966c + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689670;
    uRam00689670 = uRam00689670 + 1;
    iRam00689674 = iRam00689674 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam00689678;
    uRam00689678 = uRam00689678 + 1;
    piStack_5c = (int *)0x35;
    iRam0068967c = iRam0068967c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    ppiStack_74 = apiStack_6c;
    ppiStack_74 = (int **)func_0x00092130(&ppiStack_74,&piStack_5c,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    apiStack_6c[0] = piStack_5c;
    ppiVar10 = ppiStack_74;
    puVar12 = (undefined4 *)&UNK_004afb50;
    do {
      ppiVar9 = ppiVar10;
      piVar2 = (int *)puVar12[1];
      piVar6 = (int *)puVar12[2];
      piVar8 = (int *)puVar12[3];
      bVar13 = puVar12 != (undefined4 *)&UNK_004afb70;
      *ppiVar9 = (int *)*puVar12;
      ppiVar9[1] = piVar2;
      ppiVar9[2] = piVar6;
      ppiVar9[3] = piVar8;
      ppiVar10 = ppiVar9 + 4;
      puVar12 = puVar12 + 4;
    } while (bVar13);
    ppiVar9[4] = (int *)0x704f7972;
    *(undefined1 *)(ppiVar9 + 5) = 0x74;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689680;
    uRam00689680 = uRam00689680 + 1;
    piStack_70 = piStack_5c;
    iRam00689684 = iRam00689684 + (uint)bVar13;
    *(undefined1 *)((int)ppiStack_74 + (int)piStack_5c) = 0;
    func_0x00193cec(&piStack_5c,&UNK_004afe00);
    bVar13 = 0xfffffffe < uRam00689690;
    uRam00689690 = uRam00689690 + 1;
    iRam00689694 = iRam00689694 + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x3f,0xe,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam006896a0;
    uRam006896a0 = uRam006896a0 + 1;
    iRam006896a4 = iRam006896a4 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam006896a8;
    uRam006896a8 = uRam006896a8 + 1;
    iRam006896ac = iRam006896ac + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam006896b0;
    uRam006896b0 = uRam006896b0 + 1;
    iRam006896b4 = iRam006896b4 + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004afe2c);
    bVar13 = 0xfffffffe < uRam006896b8;
    uRam006896b8 = uRam006896b8 + 1;
    iRam006896bc = iRam006896bc + (uint)bVar13;
    func_0x00193cec(&piStack_5c,&UNK_004afb3c);
    bVar13 = 0xfffffffe < uRam006896c8;
    uRam006896c8 = uRam006896c8 + 1;
    iRam006896cc = iRam006896cc + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x40,0x18,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam006896d8;
    uRam006896d8 = uRam006896d8 + 1;
    iRam006896dc = iRam006896dc + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam006896e0;
    uRam006896e0 = uRam006896e0 + 1;
    iRam006896e4 = iRam006896e4 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    bVar13 = 0xfffffffe < uRam006896e8;
    uRam006896e8 = uRam006896e8 + 1;
    iRam006896ec = iRam006896ec + (uint)bVar13;
    func_0x00193cec(&ppiStack_74,&UNK_004afe5c);
    bVar13 = 0xfffffffe < uRam006896f0;
    uRam006896f0 = uRam006896f0 + 1;
    iRam006896f4 = iRam006896f4 + (uint)bVar13;
    func_0x00193cec(&piStack_5c,&UNK_004afe90);
    bVar13 = 0xfffffffe < uRam00689700;
    uRam00689700 = uRam00689700 + 1;
    iRam00689704 = iRam00689704 + (uint)bVar13;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x44,0x1a,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689710;
    uRam00689710 = uRam00689710 + 1;
    iRam00689714 = iRam00689714 + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689718;
    uRam00689718 = uRam00689718 + 1;
    iRam0068971c = iRam0068971c + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    iVar1 = _ZN9TVManager10mTVManagerE;
    bVar13 = 0xfffffffe < uRam00689720;
    uRam00689720 = uRam00689720 + 1;
    iRam00689724 = iRam00689724 + (uint)bVar13;
    uVar11 = *(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8);
    func_0x00193cec(&ppiStack_74,&UNK_004afe5c);
    bVar13 = 0xfffffffe < uRam00689728;
    uRam00689728 = uRam00689728 + 1;
    iRam0068972c = iRam0068972c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e20;
    uRam00680e20 = uRam00680e20 + 1;
    iRam00680e24 = iRam00680e24 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680df0;
    uRam00680df0 = uRam00680df0 + 1;
    iRam00680df4 = iRam00680df4 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e08;
    uRam00680e08 = uRam00680e08 + 1;
    iRam00680e0c = iRam00680e0c + (uint)bVar13;
    iStack_78 = 0x29;
    piStack_5c = aiStack_54;
    piStack_5c = (int *)func_0x00092130(&piStack_5c,&iStack_78,0);
    bVar13 = 0xfffffffe < uRam00680e10;
    uRam00680e10 = uRam00680e10 + 1;
    iRam00680e14 = iRam00680e14 + (uint)bVar13;
    aiStack_54[0] = iStack_78;
    piVar2 = piStack_5c;
    piVar6 = (int *)&UNK_004afebc;
    do {
      piVar8 = piVar2;
      iVar4 = piVar6[1];
      iVar5 = piVar6[2];
      iVar7 = piVar6[3];
      bVar13 = piVar6 != (int *)&UNK_004afecc;
      *piVar8 = *piVar6;
      piVar8[1] = iVar4;
      piVar8[2] = iVar5;
      piVar8[3] = iVar7;
      piVar2 = piVar8 + 4;
      piVar6 = piVar6 + 4;
    } while (bVar13);
    piVar8[5] = 0x65757274;
    piVar8[4] = 0x203a2265;
    *(undefined1 *)(piVar8 + 6) = 0x7d;
    bVar13 = 0xfffffffe < uRam00680e18;
    uRam00680e18 = uRam00680e18 + 1;
    iRam00680e1c = iRam00680e1c + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00680e30;
    uRam00680e30 = uRam00680e30 + 1;
    iRam00680e34 = iRam00680e34 + (uint)bVar13;
    bVar13 = 0xfffffffe < uRam00689738;
    uRam00689738 = uRam00689738 + 1;
    iStack_58 = iStack_78;
    iRam0068973c = iRam0068973c + (uint)bVar13;
    *(undefined1 *)((int)piStack_5c + iStack_78) = 0;
    _ZN13ModuleManager16subscribeKeyInfoEP15ModuleInterfaceb11LunaKeyType10serverTypeRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESB_
              (uVar11,iVar1,0,0x45,0x1a,&ppiStack_74,&piStack_5c);
    bVar13 = 0xfffffffe < uRam00689748;
    uRam00689748 = uRam00689748 + 1;
    iRam0068974c = iRam0068974c + (uint)bVar13;
    if (piStack_5c != aiStack_54) {
      func_0x00091788(piStack_5c,aiStack_54[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689750;
    uRam00689750 = uRam00689750 + 1;
    iRam00689754 = iRam00689754 + (uint)bVar13;
    if (ppiStack_74 != apiStack_6c) {
      func_0x00091788(ppiStack_74,(int)apiStack_6c[0] + 1);
    }
    bVar13 = 0xfffffffe < uRam00689758;
    uRam00689758 = uRam00689758 + 1;
    iRam0068975c = iRam0068975c + (uint)bVar13;
    _ZN13ModuleManager23notifyGetSoundOutputMapEv
              (*(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8));
    bVar13 = 0xfffffffe < uRam00689760;
    uRam00689760 = uRam00689760 + 1;
    iRam00689764 = iRam00689764 + (uint)bVar13;
    _ZN13ModuleManager28notifyGetPreviousSoundOutputEv
              (*(undefined4 *)(_ZN9TVManager10mTVManagerE + 0x2a8));
    bVar13 = 0xfffffffe < uRam00689768;
    uRam00689768 = uRam00689768 + 1;
    iRam0068976c = iRam0068976c + (uint)bVar13;
    iVar1 = _Z15getPmLogContextv();
    if (iVar1 == 0) {
      bVar13 = 0xfffffffe < uRam00689770;
      uRam00689770 = uRam00689770 + 1;
      iRam00689774 = iRam00689774 + (uint)bVar13;
    }
    else {
      bVar13 = 0xfffffffe < uRam00689778;
      uRam00689778 = uRam00689778 + 1;
      iRam0068977c = iRam0068977c + (uint)bVar13;
      piVar2 = (int *)_Z15getPmLogContextv();
      bVar13 = 0xfffffffe < uRam00689780;
      uRam00689780 = uRam00689780 + 1;
      iRam00689784 = iRam00689784 + (uint)bVar13;
      if (*piVar2 < 7) goto LAB_001c2654;
      bVar13 = 0xfffffffe < uRam00689788;
      uRam00689788 = uRam00689788 + 1;
      iRam0068978c = iRam0068978c + (uint)bVar13;
    }
    uVar11 = _Z15getPmLogContextv();
    bVar13 = 0xfffffffe < uRam00689790;
    uRam00689790 = uRam00689790 + 1;
    iRam00689794 = iRam00689794 + (uint)bVar13;
    uVar3 = func_0x00193610();
    func_0x00091a40(uVar11,7,0,0,0,0,0,&UNK_004afee8,uVar3,0xb1e);
    bVar13 = 0xfffffffe < uRam00689798;
    uRam00689798 = uRam00689798 + 1;
    iRam0068979c = iRam0068979c + (uint)bVar13;
  }
LAB_001c2654:
  iVar1 = func_0x00091df4(param_2,&UNK_004aff08,0x524228,0,auStack_44);
  if (iVar1 == 0) {
    bVar13 = 0xfffffffe < uRam006897a0;
    uRam006897a0 = uRam006897a0 + 1;
    iRam006897a4 = iRam006897a4 + (uint)bVar13;
  }
  else {
    bVar13 = 0xfffffffe < uRam006897a8;
    uRam006897a8 = uRam006897a8 + 1;
    iRam006897ac = iRam006897ac + (uint)bVar13;
    iVar1 = func_0x00091698(param_2,&UNK_004aff08,_ZN9TVManager10mTVManagerE,auStack_44);
    bVar13 = 0xfffffffe < uRam006897b0;
    uRam006897b0 = uRam006897b0 + 1;
    iRam006897b4 = iRam006897b4 + (uint)bVar13;
    if (iVar1 != 0) {
      return 0;
    }
    bVar13 = 0xfffffffe < uRam006897b8;
    uRam006897b8 = uRam006897b8 + 1;
    iRam006897bc = iRam006897bc + (uint)bVar13;
  }
  iVar1 = _Z15getPmLogContextv();
  if (iVar1 == 0) {
    bVar13 = 0xfffffffe < uRam006897c0;
    uRam006897c0 = uRam006897c0 + 1;
    iRam006897c4 = iRam006897c4 + (uint)bVar13;
  }
  else {
    bVar13 = 0xfffffffe < uRam006897c8;
    uRam006897c8 = uRam006897c8 + 1;
    iRam006897cc = iRam006897cc + (uint)bVar13;
    piVar2 = (int *)_Z15getPmLogContextv();
    bVar13 = 0xfffffffe < uRam006897d0;
    uRam006897d0 = uRam006897d0 + 1;
    iRam006897d4 = iRam006897d4 + (uint)bVar13;
    if (*piVar2 < 3) goto LAB_001c2902;
    bVar13 = 0xfffffffe < uRam006897d8;
    uRam006897d8 = uRam006897d8 + 1;
    iRam006897dc = iRam006897dc + (uint)bVar13;
  }
  uVar11 = _Z15getPmLogContextv();
  bVar13 = 0xfffffffe < uRam006897e0;
  uRam006897e0 = uRam006897e0 + 1;
  iRam006897e4 = iRam006897e4 + (uint)bVar13;
  uVar3 = func_0x00193610();
  func_0x00091a40(uVar11,3,0,&UNK_0049ed3c,0,0,0,&UNK_004a0e38,uVar3,0xb23,&UNK_004aff0c,
                  &UNK_004aff08);
  bVar13 = 0xfffffffe < uRam006897e8;
  uRam006897e8 = uRam006897e8 + 1;
  iRam006897ec = iRam006897ec + (uint)bVar13;
LAB_001c2902:
  func_0x00091324(auStack_44,_stderr);
  bVar13 = 0xfffffffe < uRam006897f0;
  uRam006897f0 = uRam006897f0 + 1;
  iRam006897f4 = iRam006897f4 + (uint)bVar13;
  func_0x000914a0(auStack_44);
  iRam006897fc = iRam006897fc + (uint)(0xfffffffe < uRam006897f8);
  uRam006897f8 = uRam006897f8 + 1;
  return 0xffffffff;
}


