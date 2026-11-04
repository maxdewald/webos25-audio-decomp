/* LOCAL STATIC EVIDENCE: Ghidra pseudocode, not original source. SHA256 170d468085255d381245693260840771b00d39c718473ec024566fb1df1ca3af image base 00010000 */

/* @001a5480 _ZN9TVManager26_mixDigitalSoundOutputImplERKbRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE */

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Restarted to delay deadcode elimination for space: ram */

undefined4
_ZN9TVManager26_mixDigitalSoundOutputImplERKbRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE
          (int param_1,char *param_2,undefined4 *param_3,undefined4 param_4)

{
  char cVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  undefined4 uVar5;
  uint uVar6;
  undefined4 *puVar7;
  int iVar8;
  int iVar9;
  bool bVar10;
  undefined4 *puStack_48;
  undefined4 *puStack_40;
  undefined1 auStack_34 [16];
  
  bVar10 = 0xfffffffe < uRam006843f0;
  uRam006843f0 = uRam006843f0 + 1;
  iRam006843f4 = iRam006843f4 + (uint)bVar10;
  iVar2 = _Z15getPmLogContextv();
  if (iVar2 == 0) {
    bVar10 = 0xfffffffe < uRam006843f8;
    uRam006843f8 = uRam006843f8 + 1;
    iRam006843fc = iRam006843fc + (uint)bVar10;
LAB_001a595e:
    uVar5 = _Z15getPmLogContextv();
    bVar10 = 0xfffffffe < uRam00684418;
    uRam00684418 = uRam00684418 + 1;
    iRam0068441c = iRam0068441c + (uint)bVar10;
    bVar10 = 0xfffffffe < uRam00680bb8;
    uRam00680bb8 = uRam00680bb8 + 1;
    iRam00680bbc = iRam00680bbc + (uint)bVar10;
    func_0x00091a40(uVar5,6,0,&UNK_004aeb50,0,0,0,&UNK_004aeb60,&UNK_004ade42,0x276,*param_2,
                    *param_3);
  }
  else {
    bVar10 = 0xfffffffe < uRam00684400;
    uRam00684400 = uRam00684400 + 1;
    iRam00684404 = iRam00684404 + (uint)bVar10;
    piVar3 = (int *)_Z15getPmLogContextv();
    bVar10 = 0xfffffffe < uRam00684408;
    uRam00684408 = uRam00684408 + 1;
    iRam0068440c = iRam0068440c + (uint)bVar10;
    if (5 < *piVar3) {
      bVar10 = 0xfffffffe < uRam00684410;
      uRam00684410 = uRam00684410 + 1;
      iRam00684414 = iRam00684414 + (uint)bVar10;
      goto LAB_001a595e;
    }
  }
  puStack_40 = *(undefined4 **)(param_1 + 0x2c4);
  puStack_48 = *(undefined4 **)(param_1 + 0x2c8);
  iVar2 = ((int)puStack_48 - (int)puStack_40 >> 3) * -0x55555555;
  bVar10 = 0xfffffffe < uRam006844a8;
  uRam006844a8 = uRam006844a8 + 1;
  iRam006844ac = iRam006844ac + (uint)bVar10;
  puVar7 = puStack_40;
  if (0 < iVar2 >> 2) {
    uVar5 = *param_3;
    iVar8 = param_3[1];
    uVar6 = uRam00683b28;
    iVar9 = iRam00683b2c;
    do {
      uRam00683b28 = uVar6 + 1;
      iRam00683b2c = iVar9 + (uint)(0xfffffffe < uVar6);
      if (puVar7[1] == iVar8) {
        if (iVar8 == 0) {
          bVar10 = 0xfffffffe < uRam00683b30;
          uRam00683b30 = uRam00683b30 + 1;
          iRam00683b34 = iRam00683b34 + (uint)bVar10;
        }
        else {
          bVar10 = 0xfffffffe < uRam00683b38;
          uRam00683b38 = uRam00683b38 + 1;
          iRam00683b3c = iRam00683b3c + (uint)bVar10;
          iVar4 = func_0x000921f0(*puVar7,uVar5,iVar8);
          if (iVar4 != 0) {
            bVar10 = 0xfffffffe < uRam00683b40;
            uRam00683b40 = uRam00683b40 + 1;
            iRam00683b44 = iRam00683b44 + (uint)bVar10;
            goto LAB_001a554c;
          }
        }
        bVar10 = 0xfffffffe < uRam006844b0;
        uRam006844b0 = uRam006844b0 + 1;
        iRam006844b4 = iRam006844b4 + (uint)bVar10;
        goto LAB_001a560e;
      }
LAB_001a554c:
      uRam00683b28 = uVar6 + 2;
      iRam00683b2c = iVar9 + (uint)(0xfffffffd < uVar6);
      if (iVar8 == puVar7[7]) {
        if (iVar8 == 0) {
          bVar10 = 0xfffffffe < uRam00683b30;
          uRam00683b30 = uRam00683b30 + 1;
          iRam00683b34 = iRam00683b34 + (uint)bVar10;
        }
        else {
          bVar10 = 0xfffffffe < uRam00683b38;
          uRam00683b38 = uRam00683b38 + 1;
          iRam00683b3c = iRam00683b3c + (uint)bVar10;
          iVar4 = func_0x000921f0(puVar7[6],uVar5,iVar8);
          if (iVar4 != 0) {
            bVar10 = 0xfffffffe < uRam00683b40;
            uRam00683b40 = uRam00683b40 + 1;
            iRam00683b44 = iRam00683b44 + (uint)bVar10;
            goto LAB_001a5564;
          }
        }
        puVar7 = puVar7 + 6;
        bVar10 = 0xfffffffe < uRam006844b8;
        uRam006844b8 = uRam006844b8 + 1;
        iRam006844bc = iRam006844bc + (uint)bVar10;
        cVar1 = *param_2;
        goto joined_r0x001a5752;
      }
LAB_001a5564:
      uRam00683b28 = uVar6 + 3;
      iRam00683b2c = iVar9 + (uint)(0xfffffffc < uVar6);
      if (iVar8 == puVar7[0xd]) {
        if (iVar8 == 0) {
          bVar10 = 0xfffffffe < uRam00683b30;
          uRam00683b30 = uRam00683b30 + 1;
          iRam00683b34 = iRam00683b34 + (uint)bVar10;
        }
        else {
          bVar10 = 0xfffffffe < uRam00683b38;
          uRam00683b38 = uRam00683b38 + 1;
          iRam00683b3c = iRam00683b3c + (uint)bVar10;
          iVar4 = func_0x000921f0(puVar7[0xc],uVar5,iVar8);
          if (iVar4 != 0) {
            bVar10 = 0xfffffffe < uRam00683b40;
            uRam00683b40 = uRam00683b40 + 1;
            iRam00683b44 = iRam00683b44 + (uint)bVar10;
            goto LAB_001a557c;
          }
        }
        puVar7 = puVar7 + 0xc;
        bVar10 = 0xfffffffe < uRam006844c0;
        uRam006844c0 = uRam006844c0 + 1;
        iRam006844c4 = iRam006844c4 + (uint)bVar10;
        cVar1 = *param_2;
        goto joined_r0x001a5752;
      }
LAB_001a557c:
      bVar10 = 0xfffffffb < uVar6;
      uVar6 = uVar6 + 4;
      iVar9 = iVar9 + (uint)bVar10;
      uRam00683b28 = uVar6;
      iRam00683b2c = iVar9;
      if (iVar8 == puVar7[0x13]) {
        if (iVar8 == 0) {
          bVar10 = 0xfffffffe < uRam00683b30;
          uRam00683b30 = uRam00683b30 + 1;
          iRam00683b34 = iRam00683b34 + (uint)bVar10;
        }
        else {
          bVar10 = 0xfffffffe < uRam00683b38;
          uRam00683b38 = uRam00683b38 + 1;
          iRam00683b3c = iRam00683b3c + (uint)bVar10;
          iVar4 = func_0x000921f0(puVar7[0x12],uVar5,iVar8);
          if (iVar4 != 0) {
            bVar10 = 0xfffffffe < uRam00683b40;
            uRam00683b40 = uRam00683b40 + 1;
            iRam00683b44 = iRam00683b44 + (uint)bVar10;
            goto LAB_001a5590;
          }
        }
        puVar7 = puVar7 + 0x12;
        bVar10 = 0xfffffffe < uRam006844c8;
        uRam006844c8 = uRam006844c8 + 1;
        iRam006844cc = iRam006844cc + (uint)bVar10;
        cVar1 = *param_2;
        goto joined_r0x001a5752;
      }
LAB_001a5590:
      puVar7 = puVar7 + 0x18;
      bVar10 = 0xfffffffe < uRam006844d0;
      uRam006844d0 = uRam006844d0 + 1;
      iRam006844d4 = iRam006844d4 + (uint)bVar10;
    } while (puVar7 != puStack_40 + (iVar2 >> 2) * 0x18);
    iVar2 = ((int)puStack_48 - (int)puVar7 >> 3) * -0x55555555;
  }
  if (iVar2 == 2) {
    uVar5 = *param_3;
    iVar2 = param_3[1];
LAB_001a5b24:
    uVar6 = uRam00683b28 + 1;
    iVar9 = iRam00683b2c + (uint)(0xfffffffe < uRam00683b28);
    if (iVar2 == puVar7[1]) {
      uRam00683b28 = uVar6;
      iRam00683b2c = iVar9;
      if (iVar2 == 0) {
        bVar10 = 0xfffffffe < uRam00683b30;
        uRam00683b30 = uRam00683b30 + 1;
        iRam00683b34 = iRam00683b34 + (uint)bVar10;
      }
      else {
        bVar10 = 0xfffffffe < uRam00683b38;
        uRam00683b38 = uRam00683b38 + 1;
        iRam00683b3c = iRam00683b3c + (uint)bVar10;
        iVar8 = func_0x000921f0(*puVar7,uVar5,iVar2);
        if (iVar8 != 0) {
          bVar10 = 0xfffffffe < uRam00683b40;
          uRam00683b40 = uRam00683b40 + 1;
          iRam00683b44 = iRam00683b44 + (uint)bVar10;
          goto LAB_001a5b3c;
        }
      }
      bVar10 = 0xfffffffe < uRam006844e8;
      uRam006844e8 = uRam006844e8 + 1;
      iRam006844ec = iRam006844ec + (uint)bVar10;
    }
    else {
LAB_001a5b3c:
      puVar7 = puVar7 + 6;
      bVar10 = 0xfffffffe < uRam006844f0;
      uRam006844f0 = uRam006844f0 + 1;
      iRam006844f4 = iRam006844f4 + (uint)bVar10;
LAB_001a5a9a:
      uRam00683b28 = uVar6 + 1;
      iRam00683b2c = iVar9 + (uint)(0xfffffffe < uVar6);
      if (puVar7[1] != iVar2) {
LAB_001a5ab2:
        bVar10 = 0xfffffffe < uRam00684500;
        uRam00684500 = uRam00684500 + 1;
        iRam00684504 = iRam00684504 + (uint)bVar10;
        goto LAB_001a59fc;
      }
      if (iVar2 == 0) {
        bVar10 = 0xfffffffe < uRam00683b30;
        uRam00683b30 = uRam00683b30 + 1;
        iRam00683b34 = iRam00683b34 + (uint)bVar10;
      }
      else {
        bVar10 = 0xfffffffe < uRam00683b38;
        uRam00683b38 = uRam00683b38 + 1;
        iRam00683b3c = iRam00683b3c + (uint)bVar10;
        iVar2 = func_0x000921f0(*puVar7,uVar5,iVar2);
        if (iVar2 != 0) {
          bVar10 = 0xfffffffe < uRam00683b40;
          uRam00683b40 = uRam00683b40 + 1;
          iRam00683b44 = iRam00683b44 + (uint)bVar10;
          goto LAB_001a5ab2;
        }
      }
      bVar10 = 0xfffffffe < uRam006844f8;
      uRam006844f8 = uRam006844f8 + 1;
      iRam006844fc = iRam006844fc + (uint)bVar10;
    }
LAB_001a560e:
    cVar1 = *param_2;
joined_r0x001a5752:
    if (cVar1 == '\0') {
      if (puStack_48 != puVar7) {
        bVar10 = 0xfffffffe < uRam00684428;
        uRam00684428 = uRam00684428 + 1;
        iRam0068442c = iRam0068442c + (uint)bVar10;
        _ZNSt6vectorINSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEESaIS5_EE8_M_eraseEN9__gnu_cxx17__normal_iteratorIPS5_S7_EE
                  (param_1 + 0x2c4,puVar7);
        bVar10 = 0xfffffffe < uRam00684438;
        uRam00684438 = uRam00684438 + 1;
        iRam0068443c = iRam0068443c + (uint)bVar10;
        puStack_40 = *(undefined4 **)(param_1 + 0x2c4);
        puStack_48 = *(undefined4 **)(param_1 + 0x2c8);
        goto LAB_001a58cc;
      }
LAB_001a5a1c:
      bVar10 = 0xfffffffe < uRam00684430;
      uRam00684430 = uRam00684430 + 1;
      iRam00684434 = iRam00684434 + (uint)bVar10;
LAB_001a58cc:
      if (puStack_48 == puStack_40) {
        bVar10 = 0xfffffffe < uRam00684440;
        uRam00684440 = uRam00684440 + 1;
        iRam00684444 = iRam00684444 + (uint)bVar10;
        func_0x000923ac(auStack_34,param_4);
        bVar10 = 0xfffffffe < uRam00684448;
        uRam00684448 = uRam00684448 + 1;
        iRam0068444c = iRam0068444c + (uint)bVar10;
        uVar5 = _ZN9TVManager21mixDigitalSoundOutputEN7pbnjson6JValueE(param_1,auStack_34);
        bVar10 = 0xfffffffe < uRam00684458;
        uRam00684458 = uRam00684458 + 1;
        iRam0068445c = iRam0068445c + (uint)bVar10;
        func_0x00091ba8(auStack_34);
        bVar10 = 0xfffffffe < uRam00684460;
        uRam00684460 = uRam00684460 + 1;
        iRam00684464 = iRam00684464 + (uint)bVar10;
        return uVar5;
      }
      return 1;
    }
    bVar10 = 0xfffffffe < uRam00684420;
    uRam00684420 = uRam00684420 + 1;
    iRam00684424 = iRam00684424 + (uint)bVar10;
    if (puStack_48 != puVar7) goto LAB_001a563c;
  }
  else {
    if (iVar2 == 3) {
      iVar2 = param_3[1];
      uVar6 = uRam00683b28 + 1;
      uVar5 = *param_3;
      iVar9 = iRam00683b2c + (uint)(0xfffffffe < uRam00683b28);
      uRam00683b28 = uVar6;
      iRam00683b2c = iVar9;
      if (puVar7[1] != iVar2) {
LAB_001a5af8:
        puVar7 = puVar7 + 6;
        bVar10 = 0xfffffffe < uRam006844e0;
        uRam006844e0 = uRam006844e0 + 1;
        iRam006844e4 = iRam006844e4 + (uint)bVar10;
        uRam00683b28 = uVar6;
        iRam00683b2c = iVar9;
        goto LAB_001a5b24;
      }
      if (puVar7[1] == 0) {
        bVar10 = 0xfffffffe < uRam00683b30;
        uRam00683b30 = uRam00683b30 + 1;
        iRam00683b34 = iRam00683b34 + (uint)bVar10;
      }
      else {
        bVar10 = 0xfffffffe < uRam00683b38;
        uRam00683b38 = uRam00683b38 + 1;
        iRam00683b3c = iRam00683b3c + (uint)bVar10;
        iVar8 = func_0x000921f0(*puVar7,uVar5,iVar2);
        if (iVar8 != 0) {
          bVar10 = 0xfffffffe < uRam00683b40;
          uRam00683b40 = uRam00683b40 + 1;
          iRam00683b44 = iRam00683b44 + (uint)bVar10;
          goto LAB_001a5af8;
        }
      }
      bVar10 = 0xfffffffe < uRam006844d8;
      uRam006844d8 = uRam006844d8 + 1;
      iRam006844dc = iRam006844dc + (uint)bVar10;
      goto LAB_001a560e;
    }
    if (iVar2 == 1) {
      uVar5 = *param_3;
      iVar2 = param_3[1];
      uVar6 = uRam00683b28;
      iVar9 = iRam00683b2c;
      goto LAB_001a5a9a;
    }
LAB_001a59fc:
    bVar10 = 0xfffffffe < uRam00684508;
    uRam00684508 = uRam00684508 + 1;
    iRam0068450c = iRam0068450c + (uint)bVar10;
    if (*param_2 == '\0') goto LAB_001a5a1c;
    bVar10 = 0xfffffffe < uRam00684420;
    uRam00684420 = uRam00684420 + 1;
    iRam00684424 = iRam00684424 + (uint)bVar10;
  }
  bVar10 = 0xfffffffe < uRam00684468;
  uRam00684468 = uRam00684468 + 1;
  iRam0068446c = iRam0068446c + (uint)bVar10;
  func_0x001df544(param_1 + 0x2c4,param_3);
  bVar10 = 0xfffffffe < uRam00684470;
  uRam00684470 = uRam00684470 + 1;
  iRam00684474 = iRam00684474 + (uint)bVar10;
LAB_001a563c:
  func_0x000923ac(auStack_34,param_4);
  bVar10 = 0xfffffffe < uRam00684478;
  uRam00684478 = uRam00684478 + 1;
  iRam0068447c = iRam0068447c + (uint)bVar10;
  uVar5 = _ZN9TVManager21mixDigitalSoundOutputEN7pbnjson6JValueE(param_1,auStack_34);
  bVar10 = 0xfffffffe < uRam00684488;
  uRam00684488 = uRam00684488 + 1;
  iRam0068448c = iRam0068448c + (uint)bVar10;
  func_0x00091ba8(auStack_34);
  iRam00684494 = iRam00684494 + (uint)(0xfffffffe < uRam00684490);
  uRam00684490 = uRam00684490 + 1;
  return uVar5;
}



/* @001ec608 _ZN9TVManager21mixDigitalSoundOutputEN7pbnjson6JValueE */

int _ZN9TVManager21mixDigitalSoundOutputEN7pbnjson6JValueE(int param_1,undefined4 param_2)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  bool bVar4;
  undefined1 auStack_44 [12];
  int *apiStack_38 [2];
  int aiStack_30 [4];
  
  bVar4 = 0xfffffffe < uRam00690760;
  uRam00690760 = uRam00690760 + 1;
  iRam00690764 = iRam00690764 + (uint)bVar4;
  bVar4 = 0xfffffffe < uRam0068ff60;
  uRam0068ff60 = uRam0068ff60 + 1;
  iRam0068ff64 = iRam0068ff64 + (uint)bVar4;
  apiStack_38[0] = aiStack_30;
  func_0x001e73e0(apiStack_38,&UNK_004b0b2c,&UNK_004b0b2f);
  bVar4 = 0xfffffffe < uRam0068ff70;
  uRam0068ff70 = uRam0068ff70 + 1;
  iRam0068ff74 = iRam0068ff74 + (uint)bVar4;
  bVar4 = 0xfffffffe < uRam00690768;
  uRam00690768 = uRam00690768 + 1;
  iRam0069076c = iRam0069076c + (uint)bVar4;
  iVar1 = func_0x00091b9c(param_2,apiStack_38);
  bVar4 = 0xfffffffe < uRam00690778;
  uRam00690778 = uRam00690778 + 1;
  iRam0069077c = iRam0069077c + (uint)bVar4;
  if (apiStack_38[0] != aiStack_30) {
    func_0x00091788(apiStack_38[0],aiStack_30[0] + 1);
  }
  bVar4 = 0xfffffffe < uRam00690780;
  uRam00690780 = uRam00690780 + 1;
  iRam00690784 = iRam00690784 + (uint)bVar4;
  if (iVar1 != 0) {
    bVar4 = 0xfffffffe < uRam00690788;
    uRam00690788 = uRam00690788 + 1;
    uVar3 = *(undefined4 *)(param_1 + 0x400);
    iRam0069078c = iRam0069078c + (uint)bVar4;
    bVar4 = 0xfffffffe < uRam0068ff60;
    uRam0068ff60 = uRam0068ff60 + 1;
    iRam0068ff64 = iRam0068ff64 + (uint)bVar4;
    apiStack_38[0] = aiStack_30;
    func_0x001e73e0(apiStack_38,&UNK_004b2aec,&UNK_004b2af4);
    bVar4 = 0xfffffffe < uRam0068ff70;
    uRam0068ff70 = uRam0068ff70 + 1;
    iRam0068ff74 = iRam0068ff74 + (uint)bVar4;
    bVar4 = 0xfffffffe < uRam00690790;
    uRam00690790 = uRam00690790 + 1;
    iRam00690794 = iRam00690794 + (uint)bVar4;
    func_0x000923ac(auStack_44,param_2);
    bVar4 = 0xfffffffe < uRam006907a0;
    uRam006907a0 = uRam006907a0 + 1;
    iRam006907a4 = iRam006907a4 + (uint)bVar4;
    iVar1 = _ZN10AudioMixer11setSettingsENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueEPFbP8LSHandleP9LSMessagePvEP8envelope
                      (uVar3,apiStack_38,auStack_44,0,0);
    bVar4 = 0xfffffffe < uRam006907b0;
    uRam006907b0 = uRam006907b0 + 1;
    iRam006907b4 = iRam006907b4 + (uint)bVar4;
    func_0x00091ba8(auStack_44);
    bVar4 = 0xfffffffe < uRam006907b8;
    uRam006907b8 = uRam006907b8 + 1;
    iRam006907bc = iRam006907bc + (uint)bVar4;
    if (apiStack_38[0] != aiStack_30) {
      func_0x00091788(apiStack_38[0],aiStack_30[0] + 1);
    }
    bVar4 = 0xfffffffe < uRam006907c0;
    uRam006907c0 = uRam006907c0 + 1;
    iRam006907c4 = iRam006907c4 + (uint)bVar4;
    if (iVar1 != 0) {
      return iVar1;
    }
    bVar4 = 0xfffffffe < uRam006907c8;
    uRam006907c8 = uRam006907c8 + 1;
    iRam006907cc = iRam006907cc + (uint)bVar4;
    iVar1 = _Z15getPmLogContextv();
    if (iVar1 == 0) {
      bVar4 = 0xfffffffe < uRam006907d0;
      uRam006907d0 = uRam006907d0 + 1;
      iRam006907d4 = iRam006907d4 + (uint)bVar4;
    }
    else {
      bVar4 = 0xfffffffe < uRam006907d8;
      uRam006907d8 = uRam006907d8 + 1;
      iRam006907dc = iRam006907dc + (uint)bVar4;
      piVar2 = (int *)_Z15getPmLogContextv();
      bVar4 = 0xfffffffe < uRam006907e0;
      uRam006907e0 = uRam006907e0 + 1;
      iRam006907e4 = iRam006907e4 + (uint)bVar4;
      if (*piVar2 < 3) {
        return 0;
      }
      bVar4 = 0xfffffffe < uRam006907e8;
      uRam006907e8 = uRam006907e8 + 1;
      iRam006907ec = iRam006907ec + (uint)bVar4;
    }
    uVar3 = _Z15getPmLogContextv();
    bVar4 = 0xfffffffe < uRam006907f0;
    uRam006907f0 = uRam006907f0 + 1;
    iRam006907f4 = iRam006907f4 + (uint)bVar4;
    bVar4 = 0xfffffffe < uRam0068f838;
    uRam0068f838 = uRam0068f838 + 1;
    iRam0068f83c = iRam0068f83c + (uint)bVar4;
    func_0x00091a40(uVar3,3,0,&UNK_0049f788,0,0,0,&UNK_004aeed0,&UNK_004b250e,0x217);
    bVar4 = 0xfffffffe < uRam006907f8;
    uRam006907f8 = uRam006907f8 + 1;
    iRam006907fc = iRam006907fc + (uint)bVar4;
  }
  return 0;
}



/* @001d186c _ZN9TVManager22_mixDigitalSoundOutputEP8LSHandleP9LSMessagePv */

undefined4
_ZN9TVManager22_mixDigitalSoundOutputEP8LSHandleP9LSMessagePv
          (undefined4 param_1,undefined4 param_2,int param_3)

{
  uint *puVar1;
  uint uVar2;
  int iVar3;
  undefined4 uVar4;
  int *piVar5;
  bool bVar6;
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
  undefined1 auStack_98 [116];
  
  bVar6 = 0xfffffffe < uRam0068bf20;
  uRam0068bf20 = uRam0068bf20 + 1;
  iRam0068bf24 = iRam0068bf24 + (uint)bVar6;
  _ZN19LSMessageJsonParserC1EP9LSMessagePKc(auStack_ac,param_2,&UNK_004b0a8c);
  bVar6 = 0xfffffffe < uRam0068bf28;
  uRam0068bf28 = uRam0068bf28 + 1;
  iRam0068bf2c = iRam0068bf2c + (uint)bVar6;
  iVar3 = _ZN19LSMessageJsonParser5parseEPKcP8LSHandle10ELogOption
                    (auStack_ac,&UNK_004b0b14,param_1,1);
  if (iVar3 == 0) {
    bVar6 = 0xfffffffe < uRam0068bf38;
    uRam0068bf38 = uRam0068bf38 + 1;
    iRam0068bf3c = iRam0068bf3c + (uint)bVar6;
    goto LAB_001d1b76;
  }
  iStack_120 = 0;
  auStack_11c[0] = auStack_11c[0] & 0xffffff00;
  iStack_108 = 0;
  auStack_104[0] = auStack_104[0] & 0xffffff00;
  uStack_131 = 0;
  puStack_124 = auStack_11c;
  puStack_10c = auStack_104;
  if (param_3 == 0) {
    bVar6 = 0xfffffffe < uRam0068bf48;
    uRam0068bf48 = uRam0068bf48 + 1;
    iRam0068bf4c = iRam0068bf4c + (uint)bVar6;
    iVar3 = _Z15getPmLogContextv();
    if (iVar3 == 0) {
      bVar6 = 0xfffffffe < uRam0068c090;
      uRam0068c090 = uRam0068c090 + 1;
      iRam0068c094 = iRam0068c094 + (uint)bVar6;
    }
    else {
      bVar6 = 0xfffffffe < uRam0068c098;
      uRam0068c098 = uRam0068c098 + 1;
      iRam0068c09c = iRam0068c09c + (uint)bVar6;
      piVar5 = (int *)_Z15getPmLogContextv();
      bVar6 = 0xfffffffe < uRam0068c0a8;
      uRam0068c0a8 = uRam0068c0a8 + 1;
      iRam0068c0ac = iRam0068c0ac + (uint)bVar6;
      if (*piVar5 < 3) goto LAB_001d1dee;
      bVar6 = 0xfffffffe < uRam0068c0b0;
      uRam0068c0b0 = uRam0068c0b0 + 1;
      iRam0068c0b4 = iRam0068c0b4 + (uint)bVar6;
    }
    uVar4 = _Z15getPmLogContextv();
    bVar6 = 0xfffffffe < uRam0068c0c0;
    uRam0068c0c0 = uRam0068c0c0 + 1;
    iRam0068c0c4 = iRam0068c0c4 + (uint)bVar6;
    bVar6 = 0xfffffffe < uRam00680bb8;
    uRam00680bb8 = uRam00680bb8 + 1;
    iRam00680bbc = iRam00680bbc + (uint)bVar6;
    func_0x00091a40(uVar4,3,0,&UNK_0049ed3c,0,0,0,&UNK_004b0b30,&UNK_004ade42,0x26f);
    bVar6 = 0xfffffffe < uRam0068c0d0;
    uRam0068c0d0 = uRam0068c0d0 + 1;
    iRam0068c0d4 = iRam0068c0d4 + (uint)bVar6;
  }
  else {
    bVar6 = 0xfffffffe < uRam0068bf40;
    uRam0068bf40 = uRam0068bf40 + 1;
    iRam0068bf44 = iRam0068bf44 + (uint)bVar6;
    func_0x0009222c(auStack_130,auStack_98);
    bVar6 = 0xfffffffe < uRam0068bf50;
    uRam0068bf50 = uRam0068bf50 + 1;
    iRam0068bf54 = iRam0068bf54 + (uint)bVar6;
    func_0x001939fc(auStack_ac,&UNK_004b0b2c,&uStack_131);
    bVar6 = 0xfffffffe < uRam0068bf68;
    uRam0068bf68 = uRam0068bf68 + 1;
    iRam0068bf6c = iRam0068bf6c + (uint)bVar6;
    func_0x00193b74(auStack_ac,&UNK_004ae24c,&puStack_10c);
    uStack_f0 = 0;
    auStack_ec[0] = auStack_ec[0] & 0xffffff00;
    puStack_f4 = auStack_ec;
    if (iStack_108 == 0) {
      bVar6 = 0xfffffffe < uRam0068bf80;
      uRam0068bf80 = uRam0068bf80 + 1;
      iRam0068bf84 = iRam0068bf84 + (uint)bVar6;
      iVar3 = func_0x00091f2c(param_2);
      if (iVar3 == 0) {
        bVar6 = 0xfffffffe < uRam0068bfa8;
        uRam0068bfa8 = uRam0068bfa8 + 1;
        iRam0068bfac = iRam0068bfac + (uint)bVar6;
        uVar4 = func_0x00091ad0(param_2);
        bVar6 = 0xfffffffe < uRam0068bfc8;
        uRam0068bfc8 = uRam0068bfc8 + 1;
        iRam0068bfcc = iRam0068bfcc + (uint)bVar6;
        func_0x00193cec(&puStack_c4,uVar4);
        bVar6 = 0xfffffffe < uRam0068bfd8;
        uRam0068bfd8 = uRam0068bfd8 + 1;
        iRam0068bfdc = iRam0068bfdc + (uint)bVar6;
        func_0x00091668(&puStack_f4,&puStack_c4);
        bVar6 = 0xfffffffe < uRam0068bfe0;
        uRam0068bfe0 = uRam0068bfe0 + 1;
        iRam0068bfe4 = iRam0068bfe4 + (uint)bVar6;
        if (puStack_c4 != auStack_bc) {
          func_0x00091788(puStack_c4,auStack_bc[0] + 1);
        }
        bVar6 = 0xfffffffe < uRam0068bff0;
        uRam0068bff0 = uRam0068bff0 + 1;
        iRam0068bff4 = iRam0068bff4 + (uint)bVar6;
      }
      else {
        bVar6 = 0xfffffffe < uRam0068bfa0;
        uRam0068bfa0 = uRam0068bfa0 + 1;
        iRam0068bfa4 = iRam0068bfa4 + (uint)bVar6;
        uVar4 = func_0x00091efc();
        func_0x000914dc(&puStack_f4,0,uStack_f0,iVar3,uVar4);
        bVar6 = 0xfffffffe < uRam0068bfb8;
        uRam0068bfb8 = uRam0068bfb8 + 1;
        iRam0068bfbc = iRam0068bfbc + (uint)bVar6;
      }
    }
    else {
      bVar6 = 0xfffffffe < uRam0068bf78;
      uRam0068bf78 = uRam0068bf78 + 1;
      iRam0068bf7c = iRam0068bf7c + (uint)bVar6;
      func_0x00091668(&puStack_f4,&puStack_10c);
      bVar6 = 0xfffffffe < uRam0068bf88;
      uRam0068bf88 = uRam0068bf88 + 1;
      iRam0068bf8c = iRam0068bf8c + (uint)bVar6;
    }
    func_0x000923ac(&puStack_c4,auStack_130);
    bVar6 = 0xfffffffe < uRam0068c000;
    uRam0068c000 = uRam0068c000 + 1;
    iRam0068c004 = iRam0068c004 + (uint)bVar6;
    iVar3 = _ZN9TVManager26_mixDigitalSoundOutputImplERKbRKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueE
                      (param_3,&uStack_131,&puStack_f4,&puStack_c4);
    bVar6 = 0xfffffffe < uRam0068c010;
    uRam0068c010 = uRam0068c010 + 1;
    iRam0068c014 = iRam0068c014 + (uint)bVar6;
    func_0x00091ba8(&puStack_c4);
    if (iVar3 == 0) {
      bVar6 = 0xfffffffe < uRam0068c020;
      uRam0068c020 = uRam0068c020 + 1;
      iRam0068c024 = iRam0068c024 + (uint)bVar6;
      _ZN5utils15getErrorMessageB5cxx11EP9LSMessage(&puStack_c4,param_2);
      uVar2 = auStack_11c[0];
      puVar1 = puStack_124;
      bVar6 = 0xfffffffe < uRam0068c050;
      uRam0068c050 = uRam0068c050 + 1;
      iRam0068c054 = iRam0068c054 + (uint)bVar6;
      if (puStack_c4 == auStack_bc) {
        iVar3 = iStack_c0;
        if (iStack_c0 != 0) {
          if (iStack_c0 == 1) {
            *(undefined1 *)puStack_124 = (undefined1)auStack_bc[0];
            iVar3 = iStack_c0;
          }
          else {
            func_0x00091ea8(puStack_124,auStack_bc);
            iVar3 = iStack_c0;
          }
        }
        *(undefined1 *)((int)puStack_124 + iVar3) = 0;
        iStack_120 = iVar3;
      }
      else {
        bVar6 = puStack_124 == auStack_11c;
        puStack_124 = puStack_c4;
        iStack_120 = iStack_c0;
        if ((bVar6) || (auStack_11c[0] = auStack_bc[0], puVar1 == (uint *)0x0)) {
          auStack_11c[0] = auStack_bc[0];
          puStack_c4 = auStack_bc;
        }
        else {
          auStack_bc[0] = uVar2;
          puStack_c4 = puVar1;
        }
      }
      iStack_c0 = 0;
      *(undefined1 *)puStack_c4 = 0;
      bVar6 = 0xfffffffe < uRam0068c058;
      uRam0068c058 = uRam0068c058 + 1;
      iRam0068c05c = iRam0068c05c + (uint)bVar6;
      if (puStack_c4 != auStack_bc) {
        func_0x00091788(puStack_c4,auStack_bc[0] + 1);
      }
      bVar6 = 0xfffffffe < uRam0068c060;
      uRam0068c060 = uRam0068c060 + 1;
      iRam0068c064 = iRam0068c064 + (uint)bVar6;
    }
    else {
      bVar6 = 0xfffffffe < uRam0068c018;
      uRam0068c018 = uRam0068c018 + 1;
      iRam0068c01c = iRam0068c01c + (uint)bVar6;
      _Z13string_printfB5cxx11PKcz(&puStack_dc,&UNK_0049e05c);
      uVar2 = auStack_11c[0];
      puVar1 = puStack_124;
      bVar6 = 0xfffffffe < uRam0068c030;
      uRam0068c030 = uRam0068c030 + 1;
      iRam0068c034 = iRam0068c034 + (uint)bVar6;
      if (puStack_dc == auStack_d4) {
        iVar3 = iStack_d8;
        if (iStack_d8 != 0) {
          if (iStack_d8 == 1) {
            *(undefined1 *)puStack_124 = (undefined1)auStack_d4[0];
            iVar3 = iStack_d8;
          }
          else {
            func_0x00091ea8(puStack_124,auStack_d4);
            iVar3 = iStack_d8;
          }
        }
        *(undefined1 *)((int)puStack_124 + iVar3) = 0;
        iStack_120 = iVar3;
      }
      else {
        bVar6 = puStack_124 == auStack_11c;
        puStack_124 = puStack_dc;
        iStack_120 = iStack_d8;
        if ((bVar6) || (auStack_11c[0] = auStack_d4[0], puVar1 == (uint *)0x0)) {
          auStack_11c[0] = auStack_d4[0];
          puStack_dc = auStack_d4;
        }
        else {
          auStack_d4[0] = uVar2;
          puStack_dc = puVar1;
        }
      }
      iStack_d8 = 0;
      *(undefined1 *)puStack_dc = 0;
      bVar6 = 0xfffffffe < uRam0068c038;
      uRam0068c038 = uRam0068c038 + 1;
      iRam0068c03c = iRam0068c03c + (uint)bVar6;
      if (puStack_dc != auStack_d4) {
        func_0x00091788(puStack_dc,auStack_d4[0] + 1);
      }
      bVar6 = 0xfffffffe < uRam0068c040;
      uRam0068c040 = uRam0068c040 + 1;
      iRam0068c044 = iRam0068c044 + (uint)bVar6;
    }
    _ZN5utils17LSMessageResponseEP8LSHandleP9LSMessagePKcNS_9ReplyTypeEb
              (param_1,param_2,puStack_124,0,0);
    bVar6 = 0xfffffffe < uRam0068c070;
    uRam0068c070 = uRam0068c070 + 1;
    iRam0068c074 = iRam0068c074 + (uint)bVar6;
    if (puStack_f4 != auStack_ec) {
      func_0x00091788(puStack_f4,auStack_ec[0] + 1);
    }
    bVar6 = 0xfffffffe < uRam0068c078;
    uRam0068c078 = uRam0068c078 + 1;
    iRam0068c07c = iRam0068c07c + (uint)bVar6;
    func_0x00091ba8(auStack_130);
    bVar6 = 0xfffffffe < uRam0068c080;
    uRam0068c080 = uRam0068c080 + 1;
    iRam0068c084 = iRam0068c084 + (uint)bVar6;
  }
LAB_001d1dee:
  if (puStack_10c != auStack_104) {
    func_0x00091788(puStack_10c,auStack_104[0] + 1);
  }
  bVar6 = 0xfffffffe < uRam0068c0d8;
  uRam0068c0d8 = uRam0068c0d8 + 1;
  iRam0068c0dc = iRam0068c0dc + (uint)bVar6;
  if (puStack_124 != auStack_11c) {
    func_0x00091788(puStack_124,auStack_11c[0] + 1);
  }
  bVar6 = 0xfffffffe < uRam0068c0e0;
  uRam0068c0e0 = uRam0068c0e0 + 1;
  iRam0068c0e4 = iRam0068c0e4 + (uint)bVar6;
LAB_001d1b76:
  func_0x00091944(auStack_98);
  bVar6 = 0xfffffffe < uRam0068c0e8;
  uRam0068c0e8 = uRam0068c0e8 + 1;
  iRam0068c0ec = iRam0068c0ec + (uint)bVar6;
  func_0x000917d0(auStack_a4);
  bVar6 = 0xfffffffe < uRam0068c0f0;
  uRam0068c0f0 = uRam0068c0f0 + 1;
  iRam0068c0f4 = iRam0068c0f4 + (uint)bVar6;
  return 1;
}



/* @000c6ec4 _ZN10AudioMixer11setSettingsENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueEPFbP8LSHandleP9LSMessagePvEP8envelope */

undefined4
_ZN10AudioMixer11setSettingsENSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEN7pbnjson6JValueEPFbP8LSHandleP9LSMessagePvEP8envelope
          (int param_1,undefined4 *param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5)

{
  int iVar1;
  int *piVar2;
  undefined4 uVar3;
  code *pcVar4;
  uint uVar5;
  undefined1 *puVar6;
  bool bVar7;
  uint auStack_4c [3];
  uint *puStack_40;
  uint uStack_3c;
  uint auStack_38 [5];
  
  bVar7 = 0xfffffffe < uRam00558440;
  uRam00558440 = uRam00558440 + 1;
  iRam00558444 = iRam00558444 + (uint)bVar7;
  iVar1 = _Z15getPmLogContextv();
  if (iVar1 == 0) {
    bVar7 = 0xfffffffe < uRam00558448;
    uRam00558448 = uRam00558448 + 1;
    iRam0055844c = iRam0055844c + (uint)bVar7;
LAB_000c6f30:
    uVar3 = _Z15getPmLogContextv();
    bVar7 = 0xfffffffe < uRam00558468;
    uRam00558468 = uRam00558468 + 1;
    iRam0055846c = iRam0055846c + (uint)bVar7;
    bVar7 = 0xfffffffe < uRam00557780;
    uRam00557780 = uRam00557780 + 1;
    iRam00557784 = iRam00557784 + (uint)bVar7;
    func_0x00091a40(uVar3,6,0,&UNK_0049fb44,0,0,0,&UNK_004a1a50,&UNK_004a132e,0xf3,*param_2);
  }
  else {
    bVar7 = 0xfffffffe < uRam00558450;
    uRam00558450 = uRam00558450 + 1;
    iRam00558454 = iRam00558454 + (uint)bVar7;
    piVar2 = (int *)_Z15getPmLogContextv();
    bVar7 = 0xfffffffe < uRam00558458;
    uRam00558458 = uRam00558458 + 1;
    iRam0055845c = iRam0055845c + (uint)bVar7;
    if (5 < *piVar2) {
      bVar7 = 0xfffffffe < uRam00558460;
      uRam00558460 = uRam00558460 + 1;
      iRam00558464 = iRam00558464 + (uint)bVar7;
      goto LAB_000c6f30;
    }
  }
  piVar2 = *(int **)(param_1 + 4);
  if (piVar2 == (int *)0x0) {
LAB_000c727c:
    bVar7 = 0xfffffffe < uRam00558478;
    uRam00558478 = uRam00558478 + 1;
    iRam0055847c = iRam0055847c + (uint)bVar7;
  }
  else {
    bVar7 = 0xfffffffe < uRam00558470;
    uRam00558470 = uRam00558470 + 1;
    iRam00558474 = iRam00558474 + (uint)bVar7;
    if (*(char *)(param_1 + 0x34) != '\0') {
      bVar7 = 0xfffffffe < uRam00558480;
      uRam00558480 = uRam00558480 + 1;
      iRam00558484 = iRam00558484 + (uint)bVar7;
      bVar7 = 0xfffffffe < uRam00557790;
      uRam00557790 = uRam00557790 + 1;
      iRam00557794 = iRam00557794 + (uint)bVar7;
      pcVar4 = *(code **)(*piVar2 + 0x38);
      puVar6 = (undefined1 *)*param_2;
      uVar5 = param_2[1];
      puStack_40 = auStack_38;
      if (puVar6 != (undefined1 *)0x0) {
        auStack_4c[0] = uVar5;
        if (uVar5 < 0x10) {
          if (uVar5 == 1) {
            auStack_38[0] = CONCAT31(auStack_38[0]._1_3_,*puVar6);
            goto LAB_000c70f6;
          }
          if (uVar5 == 0) goto LAB_000c70f6;
        }
        else {
          bVar7 = 0xfffffffe < uRam005577a8;
          uRam005577a8 = uRam005577a8 + 1;
          iRam005577ac = iRam005577ac + (uint)bVar7;
          puStack_40 = (uint *)func_0x00092130(&puStack_40,auStack_4c,0);
          bVar7 = 0xfffffffe < uRam005577b0;
          uRam005577b0 = uRam005577b0 + 1;
          iRam005577b4 = iRam005577b4 + (uint)bVar7;
          auStack_38[0] = auStack_4c[0];
        }
        func_0x00091ea8(puStack_40,puVar6,uVar5);
        uVar5 = auStack_4c[0];
LAB_000c70f6:
        *(undefined1 *)((int)puStack_40 + uVar5) = 0;
        bVar7 = 0xfffffffe < uRam005577b8;
        uRam005577b8 = uRam005577b8 + 1;
        iRam005577bc = iRam005577bc + (uint)bVar7;
        bVar7 = 0xfffffffe < uRam00558488;
        uRam00558488 = uRam00558488 + 1;
        iRam0055848c = iRam0055848c + (uint)bVar7;
        uStack_3c = uVar5;
        func_0x000923ac(auStack_4c,param_3);
        bVar7 = 0xfffffffe < uRam00558498;
        uRam00558498 = uRam00558498 + 1;
        iRam0055849c = iRam0055849c + (uint)bVar7;
        uVar3 = (*pcVar4)(piVar2,&puStack_40,auStack_4c,param_4,param_5);
        bVar7 = 0xfffffffe < uRam005584a8;
        uRam005584a8 = uRam005584a8 + 1;
        iRam005584ac = iRam005584ac + (uint)bVar7;
        func_0x00091ba8(auStack_4c);
        bVar7 = 0xfffffffe < uRam005584b0;
        uRam005584b0 = uRam005584b0 + 1;
        iRam005584b4 = iRam005584b4 + (uint)bVar7;
        if (puStack_40 != auStack_38) {
          func_0x00091788(puStack_40,auStack_38[0] + 1);
        }
        bVar7 = 0xfffffffe < uRam005584b8;
        uRam005584b8 = uRam005584b8 + 1;
        iRam005584bc = iRam005584bc + (uint)bVar7;
        return uVar3;
      }
      bVar7 = 0xfffffffe < uRam00557798;
      uRam00557798 = uRam00557798 + 1;
      iRam0055779c = iRam0055779c + (uint)bVar7;
      if (uVar5 == 0) goto LAB_000c70f6;
      bVar7 = 0xfffffffe < uRam005577a0;
      uRam005577a0 = uRam005577a0 + 1;
      iRam005577a4 = iRam005577a4 + (uint)bVar7;
      func_0x000917ac(&UNK_0049d590);
      goto LAB_000c727c;
    }
  }
  iVar1 = _Z15getPmLogContextv();
  if (iVar1 == 0) {
    bVar7 = 0xfffffffe < uRam005584c0;
    uRam005584c0 = uRam005584c0 + 1;
    iRam005584c4 = iRam005584c4 + (uint)bVar7;
  }
  else {
    bVar7 = 0xfffffffe < uRam005584c8;
    uRam005584c8 = uRam005584c8 + 1;
    iRam005584cc = iRam005584cc + (uint)bVar7;
    piVar2 = (int *)_Z15getPmLogContextv();
    bVar7 = 0xfffffffe < uRam005584d0;
    uRam005584d0 = uRam005584d0 + 1;
    iRam005584d4 = iRam005584d4 + (uint)bVar7;
    if (*piVar2 < 3) goto LAB_000c706c;
    bVar7 = 0xfffffffe < uRam005584d8;
    uRam005584d8 = uRam005584d8 + 1;
    iRam005584dc = iRam005584dc + (uint)bVar7;
  }
  uVar3 = _Z15getPmLogContextv();
  bVar7 = 0xfffffffe < uRam005584e0;
  uRam005584e0 = uRam005584e0 + 1;
  iRam005584e4 = iRam005584e4 + (uint)bVar7;
  bVar7 = 0xfffffffe < uRam00557780;
  uRam00557780 = uRam00557780 + 1;
  iRam00557784 = iRam00557784 + (uint)bVar7;
  func_0x00091a40(uVar3,3,0,&UNK_0049fb44,0,0,0,&UNK_004a1a80,&UNK_004a132e,0xf9);
LAB_000c706c:
  bVar7 = 0xfffffffe < uRam005584e8;
  uRam005584e8 = uRam005584e8 + 1;
  iRam005584ec = iRam005584ec + (uint)bVar7;
  return 0;
}


