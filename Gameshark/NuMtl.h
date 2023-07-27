int NuMtlCreateDefault()
{
  int v0; // r9
  int v1; // r0
  int v2; // r3
  int result; // r3

  __asm { mfspr     r0, LR }
  v0 = NuMtlCreate(1);
  v1 = *(_DWORD *)(v0 + 4);
  numtl_white = v0;
  *(_BYTE *)(v0 + 66) = 0;
  v2 = numtl_white;
  *(float *)(v0 + 52) = 1.0;
  *(_DWORD *)(v0 + 4) = (v1 & 0xFFCFFFFF | 0x200000) & 0x3FF3FFFF;
  *(_WORD *)(v0 + 64) = 0;
  *(float *)(v0 + 20) = 1.0;
  *(float *)(v0 + 24) = 1.0;
  *(float *)(v0 + 28) = 1.0;
  result = NuMtlUpdate(v2);
  __asm { mtspr     LR, r0 }
  return result;
}

int NuMtlInit()
{
  int result; // r3

  __asm { mfspr     r0, LR }
  if ( initialised_0 )
    NuMtlClose();
  smlist = 0;
  initialised_0 = 1;
  result = NuMtlCreateDefault();
  __asm { mtspr     LR, r0 }
  return result;
}

int NuMtlClose()
{
  int v0; // r30
  int v1; // r3
  int result; // r3
  char v3; // cr34

  __asm { mfspr     r0, LR }
  if ( initialised_0 )
  {
    v0 = smlist;
    if ( smlist )
    {
      do
      {
        v1 = v0;
        v0 = *(_DWORD *)(v0 + 96);
        result = NuMtlDestroy(v1);
        __asm { mdoom. }
      }
      while ( !v3 );
    }
    initialised_0 = 0;
  }
  __asm { mtspr     LR, r0 }
  return result;
}

int __fastcall NuMtlCreate(int a1)
{
  int v1; // r31
  int v2; // r30
  int v3; // r29
  int v4; // r0
  int v5; // r9
  int result; // r3

  __asm { mfspr     r0, LR }
  v3 = 0;
  __asm { mtspr     CTR, r0 }
  if ( a1 > 0 )
  {
    __asm { mfspr     r30, CTR }
    do
    {
      v1 = malloc_x(108);
      memset(v1, 0, 108);
      v4 = *(_DWORD *)(v1 + 4);
      *(float *)(v1 + 52) = 1.0;
      *(_DWORD *)(v1 + 4) = v4 & 0xCFFFFFFF | 0x20000000;
      NuMtlInsert(v1);
      v5 = nummtls;
      --v2;
      *(_DWORD *)v1 = v3;
      v3 = v1;
      nummtls = v5 + 1;
    }
    while ( v2 );
  }
  result = v1;
  __asm { mtspr     LR, r0 }
  return result;
}

int __fastcall NuMtlDestroy(_DWORD *a1)
{
  int v2; // r9
  int v3; // r9
  int result; // r3

  __asm { mfspr     r0, LR }
  if ( a1[14] )
    NuTexUnRef();
  v2 = a1[22];
  if ( v2 )
    NuGeomDestroy(*(_DWORD *)(v2 + 20));
  v3 = a1[23];
  if ( v3 )
    NuGeomDestroy(*(_DWORD *)(v3 + 20));
  NuMtlRemove(a1);
  result = free_x(a1);
  --nummtls;
  __asm { mtspr     LR, r0 }
  return result;
}

int NuMtlNum()
{
  return nummtls;
}

int __fastcall NuMtlGet(int a1)
{
  int v1; // ctr
  int v2; // r11
  int v3; // r9

  v2 = 0;
  v3 = smlist;
  if ( a1 < nummtls )
  {
    if ( a1 > 0 )
    {
      __asm { mtspr     CTR, r3 }
      do
      {
        v3 = *(_DWORD *)(v3 + 96);
        --v1;
      }
      while ( v1 );
    }
    v2 = v3;
  }
  return v2;
}

int __fastcall NuMtlRead(int a1)
{
  int v2; // r30
  int result; // r3

  __asm { mfspr     r0, LR }
  v2 = NuMtlCreate(1);
  NuFileRead(a1, v2, 84);
  result = v2;
  __asm { mtspr     LR, r0 }
  return result;
}

int __fastcall NuMtlInsert(int result)
{
  int v1; // r10
  int v2; // r11
  int v3; // r9
  int v4; // r0
  bool v5; // cr34
  int v6; // r9

  v1 = 0;
  v2 = smlist;
  v3 = *(_DWORD *)(result + 56);
  v4 = *(__int16 *)(result + 60) << 16;
  v5 = smlist == 0;
  *(_DWORD *)(result + 96) = 0;
  *(_DWORD *)(result + 100) = 0;
  while ( !v5 && (*(__int16 *)(v2 + 60) << 16) + *(_DWORD *)(v2 + 56) > v4 + v3 )
  {
    v1 = v2;
    v2 = *(_DWORD *)(v2 + 96);
    v5 = v2 == 0;
  }
  if ( v1 )
  {
    *(_DWORD *)(result + 100) = v1;
    *(_DWORD *)(result + 96) = *(_DWORD *)(v1 + 96);
    *(_DWORD *)(v1 + 96) = result;
  }
  else
  {
    *(_DWORD *)(result + 96) = smlist;
    smlist = result;
  }
  v6 = *(_DWORD *)(result + 96);
  if ( v6 )
    *(_DWORD *)(v6 + 100) = result;
  return result;
}

int __fastcall NuMtlRemove(int result)
{
  int v1; // r9
  int v2; // r9

  v1 = *(_DWORD *)(result + 100);
  if ( v1 )
    *(_DWORD *)(v1 + 96) = *(_DWORD *)(result + 96);
  else
    smlist = *(_DWORD *)(result + 96);
  v2 = *(_DWORD *)(result + 96);
  if ( v2 )
    *(_DWORD *)(v2 + 100) = *(_DWORD *)(result + 100);
  return result;
}

void NuMtlUpdate()
{
  
}

int __fastcall NuMtlSetStencilRender(int result)
{
  stencil_mode = result;
  return result;
}

int __fastcall NuMtlAddRndrItem(int result, int a2)
{
  int *v2; // r30
  int v3; // r7
  int v4; // r10
  bool v5; // cr34
  int *v6; // r11
  int v7; // r0
  int v8; // r30
  char *v9; // r30

  __asm { mfspr     r0, LR }
  v3 = stencil_mode;
  if ( stencil_mode )
  {
    v4 = stenitem_cnt;
    v5 = stenitem_cnt == 0;
    v6 = &stenitem[4 * stenitem_cnt];
    v6[1] = a2;
    v6[2] = result;
    v6[3] = v3;
    if ( !v5 )
      stenitem[4 * v4 - 4] = (int)v6;
    ++stenitem_cnt;
  }
  else
  {
    v7 = *(__int16 *)(a2 + 34);
    if ( v7 == 3 || v7 == 33 )
    {
      result = NuMtlAddGlassItem(result);
    }
    else if ( v7 == 1 || v7 == 27 )
    {
      if ( wateritem_cnt > 511 )
      {
        NuErrorProlog("C:/source/crashwoc/code/nu3dx/numtl.c", 679);
        __asm { mtspr     LR, r0 }
        result = ((int (__fastcall *)(const char *))loc_800B2EFC)("NuMtlAddRndrItem: Exceeded maximum number of water items in render queue!");
      }
      else
      {
        v2 = &wateritem[3 * wateritem_cnt];
        v2[1] = a2;
        v2[2] = result;
      }
      if ( wateritem_cnt )
        wateritem[3 * wateritem_cnt - 3] = (int)v2;
      ++wateritem_cnt;
    }
    else if ( *(_DWORD *)(result + 4) >> 30 || *(_BYTE *)(*(_DWORD *)(*(_DWORD *)(a2 + 20) + 4) + 62) == 4 )
    {
      v8 = 16 * --otitem_cnt;
      v9 = (char *)&otitem + v8;
      *((_DWORD *)v9 + 3) = result;
      *((_DWORD *)v9 + 1) = a2;
      *((float *)v9 + 2) = NuRndrItemDist(a2);
      result = NuMtlOTInsert(v9);
    }
    else
    {
      *(_DWORD *)a2 = *(_DWORD *)(result + 84);
      *(_DWORD *)(result + 84) = a2;
    }
  }
  __asm { mtspr     LR, r0 }
  return result;
}

int __fastcall NuMtlAddFaceonItem(int result, int a2)
{
  int v2; // r30
  int v3; // r0
  int v4; // r8
  bool v5; // cr34
  int *v6; // r31
  int *v7; // r27
  int v8; // r0
  int v9; // r11

  __asm { mfspr     r0, LR }
  v2 = 0;
  v3 = 12 * (faceonitem_cnt - 1);
  v4 = faceonmtllist[0];
  --faceonitem_cnt;
  v5 = faceonmtllist[0] == 0;
  v6 = (int *)((char *)&faceonitem + v3);
  v6[1] = a2;
  v6[2] = result;
  if ( !v5 )
  {
    if ( *(_DWORD *)(v4 + 8) == result )
    {
LABEL_8:
      v8 = faceonmtllist[v2];
      if ( v8 )
      {
        faceonmtllist[v2] = (int)v6;
        *v6 = v8;
        goto LABEL_11;
      }
    }
    else
    {
      v7 = faceonmtllist;
      while ( 1 )
      {
        if ( v2 > 49 )
        {
          NuErrorProlog("C:/source/crashwoc/code/nu3dx/numtl.c", 757);
          __asm { mtspr     LR, r0 }
          result = ((int (__fastcall *)(const char *))loc_800B3038)("Out of room for face-ons in the list");
        }
        ++v7;
        ++v2;
        if ( !*v7 )
          break;
        if ( *(_DWORD *)(*v7 + 8) == v6[2] )
          goto LABEL_8;
      }
    }
  }
  v9 = faceonmtl_cnt;
  faceonmtllist[v2] = (int)v6;
  *v6 = 0;
  faceonmtl_cnt = v9 + 1;
LABEL_11:
  __asm { mtspr     LR, r0 }
  return result;
}

int __fastcall NuMtlOTInsert(int result)
{
  int *v1; // r31
  int v2; // r30
  int v3; // r0

  __asm { mfspr     r0, LR }
  v1 = (int *)result;
  v2 = *(__int16 *)(*(_DWORD *)(result + 12) + 60);
  if ( v2 > 256 )
  {
    NuErrorProlog("C:/source/crashwoc/code/nu3dx/numtl.c", 992);
    __asm { mtspr     LR, r0 }
    result = ((int (__fastcall *)(const char *))loc_800B30FC)("assert");
  }
  v3 = ot[v2];
  if ( v3 )
  {
    *v1 = v3;
    ot[v2] = (int)v1;
  }
  else
  {
    ot[v2] = (int)v1;
    *v1 = 0;
  }
  __asm { mtspr     LR, r0 }
  return result;
}

int __fastcall NuMtlGet2dBuffer(int a1, int a2, _DWORD *a3, int *a4, int a5, int a6)
{
  int v10; // r3
  int v11; // r3
  int v12; // r30
  int v13; // r31
  bool v14; // cr62
  int result; // r3

  __asm { mfspr     r0, LR }
  NuMemSetExternal(a5, a6);
  if ( !*(_DWORD *)(a1 + 88) )
  {
    v10 = NuMemAlloc(36);
    *(_DWORD *)(a1 + 88) = v10;
    v11 = memset(v10, 0, 36);
    *(_DWORD *)(*(_DWORD *)(a1 + 88) + 4) = 1;
    *(_DWORD *)(*(_DWORD *)(a1 + 88) + 8) = 0;
    *(_DWORD *)(*(_DWORD *)(a1 + 88) + 20) = NuGeomCreate(v11);
    NuGeomCreateVB(*(_DWORD *)(*(_DWORD *)(a1 + 88) + 20), 1024, 83, 1);
    *(_DWORD *)(*(_DWORD *)(*(_DWORD *)(a1 + 88) + 20) + 16) = 0;
  }
  v12 = *(_DWORD *)(*(_DWORD *)(a1 + 88) + 20);
  v13 = *(_DWORD *)(v12 + 32);
  if ( !v13 )
  {
LABEL_8:
    v13 = NuPrimCreate(1024, a2);
    NuGeomAddPrim(v12, v13);
    v14 = v12 == 0;
    *(_WORD *)(v13 + 8) = 0;
    goto LABEL_9;
  }
  v14 = v12 == 0;
  while ( *(_DWORD *)(v13 + 4) != a2 )
  {
    v13 = *(_DWORD *)v13;
    if ( !v13 )
      goto LABEL_8;
  }
LABEL_9:
  if ( !v14 )
    *a3 = v12;
  if ( v13 )
    *a4 = v13;
  result = NuMemSetExternal(0, 0);
  __asm { mtspr     LR, r0 }
  return result;
}

int __fastcall NuMtlGet3dBuffer(int a1, int a2, _DWORD *a3, int *a4, int a5, int a6)
{
  int v10; // r3
  int v11; // r3
  int v12; // r30
  int v13; // r31
  bool v14; // cr62
  int result; // r3

  __asm { mfspr     r0, LR }
  NuMemSetExternal(a5, a6);
  if ( !*(_DWORD *)(a1 + 92) )
  {
    v10 = NuMemAlloc(36);
    *(_DWORD *)(a1 + 92) = v10;
    v11 = memset(v10, 0, 36);
    *(_DWORD *)(*(_DWORD *)(a1 + 92) + 4) = 0;
    *(_DWORD *)(*(_DWORD *)(a1 + 92) + 8) = 0;
    *(_DWORD *)(*(_DWORD *)(a1 + 92) + 20) = NuGeomCreate(v11);
    NuGeomCreateVB(*(_DWORD *)(*(_DWORD *)(a1 + 92) + 20), 3072, 89, 1);
    *(_DWORD *)(*(_DWORD *)(*(_DWORD *)(a1 + 92) + 20) + 16) = 0;
  }
  v12 = *(_DWORD *)(*(_DWORD *)(a1 + 92) + 20);
  v13 = *(_DWORD *)(v12 + 32);
  if ( !v13 )
  {
LABEL_8:
    v13 = NuPrimCreate(3072, a2);
    NuGeomAddPrim(v12, v13);
    v14 = v12 == 0;
    *(_WORD *)(v13 + 8) = 0;
    goto LABEL_9;
  }
  v14 = v12 == 0;
  while ( *(_DWORD *)(v13 + 4) != a2 )
  {
    v13 = *(_DWORD *)v13;
    if ( !v13 )
      goto LABEL_8;
  }
LABEL_9:
  if ( !v14 )
    *a3 = v12;
  if ( v13 )
    *a4 = v13;
  result = NuMemSetExternal(0, 0);
  __asm { mtspr     LR, r0 }
  return result;
}

int NuMtlRender()
{
  int v0; // r3
  int v1; // r3
  int v2; // r3
  int v3; // r3
  int v4; // r3
  int v5; // r3
  int v6; // r3
  int v7; // r3
  int result; // r3

  __asm { mfspr     r0, LR }
  v0 = DBTimerStart(19);
  NuMtlRender3d(v0);
  DBTimerEnd(19);
  DBTimerStart(18);
  DBTimerEnd(18);
  DBTimerStart(17);
  DBTimerEnd(17);
  v1 = DBTimerStart(16);
  NuMtlRenderDynamic2d3d(v1);
  DBTimerEnd(16);
  DBTimerStart(15);
  NuMtlRenderOT(0, 10);
  DBTimerEnd(15);
  v2 = DBTimerStart(14);
  NuMtlRenderFaceOn(v2);
  DBTimerEnd(14);
  v3 = DBTimerStart(12);
  NuMtlRenderSten(v3);
  DBTimerEnd(12);
  v4 = DBTimerStart(11);
  NuMtlRenderGlass(v4);
  DBTimerEnd(11);
  v5 = DBTimerStart(10);
  NuMtlRenderWater(v5);
  DBTimerEnd(10);
  DBTimerStart(9);
  NuMtlRenderOT(11, 256);
  DBTimerEnd(9);
  v6 = DBTimerStart(8);
  DrawStencilShadowQuad(v6);
  DBTimerEnd(8);
  v7 = DBTimerStart(7);
  NuMtlRenderUpd(v7);
  result = DBTimerEnd(7);
  __asm { mtspr     LR, r0 }
  return result;
}

int NuMtlRender3d()
{
  int i; // r30
  _DWORD *j; // r31
  int result; // r3

  __asm { mfspr     r0, LR }
  GS_SetAlphaCompare(7, 0);
  for ( i = smlist; i; i = *(_DWORD *)(i + 96) )
  {
    if ( !*(_BYTE *)(i + 66) && *(_DWORD *)(i + 84) )
    {
      NuTexSetTexture(0, *(_DWORD *)(i + 56));
      NuMtlSetRenderStates(i);
      NuTexSetTextureStates(i);
      for ( j = *(_DWORD **)(i + 84); j; j = (_DWORD *)*j )
        NuRndrItem(j);
      *(_DWORD *)(i + 84) = 0;
    }
  }
  result = GS_SetAlphaCompare(7, 0);
  __asm { mtspr     LR, r0 }
  return result;
}

int NuMtlRenderDynamic2d3d()
{
  _DWORD *v0; // r30
  int result; // r3
  int v2; // r9
  int v3; // r31
  int v4; // r9
  int v5; // r31

  __asm { mfspr     r0, LR }
  GS_SetAlphaCompare(7, 0);
  v0 = (_DWORD *)smlist;
  for ( result = GS_SetZCompare(1, 0, 3); v0; v0 = (_DWORD *)v0[24] )
  {
    v2 = v0[23];
    if ( v2 )
    {
      if ( *(_DWORD *)(*(_DWORD *)(v2 + 20) + 16) )
      {
        NuTexSetTexture(0, v0[14]);
        NuMtlSetRenderStates(v0);
        NuTexSetTextureStates(v0);
        v3 = v0[23];
        NuRndrItem(v3);
        *(_DWORD *)(*(_DWORD *)(v3 + 20) + 16) = 0;
        for ( result = *(_DWORD *)(*(_DWORD *)(v3 + 20) + 32); result; result = *(_DWORD *)result )
          *(_WORD *)(result + 8) = 0;
      }
    }
    v4 = v0[22];
    if ( v4 )
    {
      if ( *(_DWORD *)(*(_DWORD *)(v4 + 20) + 16) )
      {
        NuTexSetTexture(0, v0[14]);
        NuMtlSetRenderStates(v0);
        NuTexSetTextureStates(v0);
        v5 = v0[22];
        GS_EnableLighting(0);
        GS_SetZCompare(1, 0, 3);
        GS_SetAlphaCompare(7, 0);
        NuRndrItem(v5);
        *(_DWORD *)(*(_DWORD *)(v5 + 20) + 16) = 0;
        for ( result = *(_DWORD *)(*(_DWORD *)(v5 + 20) + 32); result; result = *(_DWORD *)result )
          *(_WORD *)(result + 8) = 0;
      }
    }
  }
  __asm { mtspr     LR, r0 }
  return result;
}

int __fastcall NuMtlRenderOT(int a1, int a2)
{
  int result; // r3
  int v5; // r0
  _DWORD *v6; // r31
  int i; // r29
  int v8; // r9

  __asm { mfspr     r0, LR }
  for ( result = GS_SetAlphaCompare(7, 0); a1 <= a2; *(int *)((char *)ot + i) = 0 )
  {
    v5 = 4 * a1;
    v6 = (_DWORD *)ot[a1++];
    for ( i = v5; v6; v6 = (_DWORD *)*v6 )
    {
      v8 = v6[3];
      if ( *(_BYTE *)(v8 + 66) )
      {
        NuTexSetTexture(0, *(_DWORD *)(v8 + 56));
        NuMtlSetRenderStates(v6[3]);
        NuTexSetTextureStates(v6[3]);
        GS_SetZCompare(1, 1, 3);
        GS_SetAlphaCompare(3, 247);
      }
      else
      {
        NuTexSetTexture(0, *(_DWORD *)(v8 + 56));
        NuMtlSetRenderStates(v6[3]);
        NuTexSetTextureStates(v6[3]);
      }
      result = NuRndrItem(v6[1]);
    }
  }
  __asm { mtspr     LR, r0 }
  return result;
}

int NuMtlRenderFaceOn()
{
  int v0; // r31
  int v1; // r3
  int result; // r3
  _DWORD *v3; // r25
  int v4; // r19
  int v5; // r9
  int v6; // r20
  char *v7; // r11
  int v8; // r10
  _DWORD *v9; // r9
  int v10; // r0
  int v11; // r21
  char *v12; // r11
  int v13; // r10
  _DWORD *v14; // r9
  int v15; // r0
  _DWORD *i; // r27
  int v17; // r31
  int v18; // r30
  int v19; // r3
  int v20; // r26
  double v21; // fp30
  double v22; // fp31
  unsigned int v23; // r3
  int v24; // r9
  char v25[48]; // [sp+8h] [-1A8h] BYREF
  float v26; // [sp+38h] [-178h]
  float v27; // [sp+3Ch] [-174h]
  float v28; // [sp+40h] [-170h]
  char v29; // [sp+48h] [-168h] BYREF
  char v30[64]; // [sp+88h] [-128h] BYREF
  char v31[16]; // [sp+C8h] [-E8h] BYREF
  char v32[16]; // [sp+D8h] [-D8h] BYREF
  char v33[16]; // [sp+E8h] [-C8h] BYREF
  float v34; // [sp+F8h] [-B8h] BYREF
  float v35; // [sp+FCh] [-B4h]
  float v36; // [sp+100h] [-B0h]

  __asm { mfspr     r0, LR }
  ResetLanguage();
  v0 = 0;
  v1 = SetVertexShader(322);
  result = GS_LoadWorldMatrixIdentity(v1);
  if ( faceonmtl_cnt > 0 )
  {
    do
    {
      v3 = (_DWORD *)faceonmtllist[v0];
      v4 = v0;
      v5 = v3[2];
      if ( *(_BYTE *)(v5 + 66) )
      {
        NuTexSetTexture(0, *(_DWORD *)(v5 + 56));
        NuMtlSetRenderStates(v3[2]);
        NuTexSetTextureStates(v3[2]);
        GS_SetZCompare(1, 0, 3);
        result = GS_SetAlphaCompare(3, 247);
      }
      else
      {
        NuTexSetTexture(0, *(_DWORD *)(v5 + 56));
        NuMtlSetRenderStates(v3[2]);
        NuTexSetTextureStates(v3[2]);
        GS_SetAlphaCompare(4, 0);
        result = GS_SetZCompare(1, 1, 3);
      }
      v6 = v0 + 1;
      v7 = &v29;
      v8 = 48;
      v9 = *(_DWORD **)(v3[1] + 16);
      do
      {
        v8 -= 24;
        *(_DWORD *)v7 = *v9;
        *((_DWORD *)v7 + 1) = v9[1];
        *((_DWORD *)v7 + 2) = v9[2];
        *((_DWORD *)v7 + 3) = v9[3];
        *((_DWORD *)v7 + 4) = v9[4];
        v10 = v9[5];
        v9 += 6;
        *((_DWORD *)v7 + 5) = v10;
        v7 += 24;
      }
      while ( v8 );
      *(_DWORD *)v7 = *v9;
      *((_DWORD *)v7 + 1) = v9[1];
      *((_DWORD *)v7 + 2) = v9[2];
      for ( *((_DWORD *)v7 + 3) = v9[3]; v3; v3 = (_DWORD *)*v3 )
      {
        v11 = v3[1];
        v12 = v25;
        v13 = 48;
        v14 = *(_DWORD **)(v11 + 16);
        do
        {
          v13 -= 24;
          *(_DWORD *)v12 = *v14;
          *((_DWORD *)v12 + 1) = v14[1];
          *((_DWORD *)v12 + 2) = v14[2];
          *((_DWORD *)v12 + 3) = v14[3];
          *((_DWORD *)v12 + 4) = v14[4];
          v15 = v14[5];
          v14 += 6;
          *((_DWORD *)v12 + 5) = v15;
          v12 += 24;
        }
        while ( v13 );
        *(_DWORD *)v12 = *v14;
        *((_DWORD *)v12 + 1) = v14[1];
        *((_DWORD *)v12 + 2) = v14[2];
        *((_DWORD *)v12 + 3) = v14[3];
        v26 = 0.0;
        v27 = 0.0;
        v28 = 0.0;
        for ( i = *(_DWORD **)(v3[1] + 20); i; i = (_DWORD *)*i )
        {
          v17 = i[5];
          v18 = i[4];
          v19 = GS_DrawQuadListBeginBlock(4 * v17, 0);
          if ( v17 > 0 )
          {
            v20 = v17;
            do
            {
              v21 = (float)(*(float *)(v18 + 12) * (float)0.5);
              v22 = (float)(*(float *)(v18 + 16) * (float)0.5);
              NuVecMtxRotate(v32, v18, v25);
              NuVecAdd(v33, v32, *(_DWORD *)(v11 + 16) + 48);
              NuMtxCalcCheapFaceOn(v30, v33);
              v34 = -v21;
              v35 = v22;
              v36 = 0.0;
              NuVecMtxTransform(v31, &v34, v30);
              v23 = *(_DWORD *)(v18 + 20);
              v18 += 24;
              GS_SetQuadListRGBA(BYTE1(v23), BYTE2(v23), (unsigned __int8)v23, HIBYTE(v23));
              GS_DrawQuadListSetVert(v31, 0.0, 0.0);
              v34 = v21;
              v35 = v22;
              v36 = 0.0;
              NuVecMtxTransform(v31, &v34, v30);
              GS_DrawQuadListSetVert(v31, 1.0, 0.0);
              v34 = v21;
              v35 = -v22;
              v36 = 0.0;
              NuVecMtxTransform(v31, &v34, v30);
              GS_DrawQuadListSetVert(v31, 1.0, 1.0);
              v34 = -v21;
              v35 = -v22;
              v36 = 0.0;
              NuVecMtxTransform(v31, &v34, v30);
              v19 = GS_DrawQuadListSetVert(v31, 0.0, 1.0);
              --v20;
            }
            while ( v20 );
          }
          result = GS_DrawQuadListEndBlock(v19);
        }
      }
      v24 = faceonmtl_cnt;
      v0 = v6;
      faceonmtllist[v4] = 0;
    }
    while ( v6 < v24 );
  }
  __asm { mtspr     LR, r0 }
  return result;
}

void NuMtlRenderGlass()
{
  int *v0; // r31
  int v1; // r30
  int v2; // r3

  __asm { mfspr     r0, LR }
  v0 = &dynamic_glass_items;
  v1 = 0;
  if ( dynamic_glass_item_cnt > 0 )
  {
    v2 = NudxFw_MakeBackBufferCopy(force_glass_screencopy_enable);
    if ( dynamic_glass_item_cnt > 0 )
    {
      do
      {
        ++v1;
        NuTexSetTexture(0, *(_DWORD *)(v0[3] + 56));
        NuMtlSetRenderStates(v0[3]);
        NuTexSetTextureStates(v0[3]);
        v2 = NuRndrItem(v0[1]);
        v0 = (int *)*v0;
      }
      while ( v1 < dynamic_glass_item_cnt );
    }
    NuMtlClearGlassList(v2);
  }
  __asm { mtspr     LR, r0 }
}

int NuMtlRenderWater()
{
  int *v0; // r31
  int v1; // r30
  int result; // r3

  __asm { mfspr     r0, LR }
  v0 = wateritem;
  if ( wateritem_cnt > 0 )
  {
    v1 = 0;
    for ( result = NudxFw_MakeBackBufferCopy(1); v1 < wateritem_cnt; v0 = (int *)*v0 )
    {
      ++v1;
      NuTexSetTexture(0, *(_DWORD *)(v0[2] + 56));
      NuMtlSetRenderStates(v0[2]);
      NuTexSetTextureStates(v0[2]);
      result = NuRndrItem(v0[1]);
    }
  }
  wateritem_cnt = 0;
  __asm { mtspr     LR, r0 }
  return result;
}

int NuMtlRenderSten()
{
  int *v0; // r30
  int v1; // r31
  int result; // r3

  __asm { mfspr     r0, LR }
  v0 = stenitem;
  GS_SetAlphaCompare(4, 0);
  v1 = 0;
  func_800C71F0(125, 1);
  func_800C71F0(70, 519);
  func_800C71F0(68, 7680);
  func_800C71F0(126, 7680);
  func_800C71F0(71, 1);
  func_800C71F0(72, -1);
  func_800C71F0(73, -1);
  IsStencil = 1;
  if ( stenitem_cnt > 0 )
  {
    do
    {
      if ( v0[3] == 1 )
      {
        GS_EnableLighting(0);
        GS_SetMaterialSourceEmissive(1);
        NuTexSetTexture(0, 0);
        GS_SetBlendSrc(1, 1, 0);
      }
      else
      {
        NuTexSetTexture(0, *(_DWORD *)(v0[2] + 56));
        NuMtlSetRenderStates(v0[2]);
        NuTexSetTextureStates(v0[2]);
      }
      if ( v0[3] == 1 )
      {
        func_800C71F0(69, 7681);
        NuRndrItem(v0[1]);
      }
      ++v1;
      v0 = (int *)*v0;
    }
    while ( v1 < stenitem_cnt );
  }
  stenitem_cnt = 0;
  GS_EnableLighting(1);
  result = func_800C71F0(125, 0);
  IsStencil = 0;
  __asm { mtspr     LR, r0 }
  return result;
}

int __fastcall NuMtlAddGlassItem(int result, int a2)
{
  int v2; // r30
  int v4; // r10
  int *v5; // r11

  __asm { mfspr     r0, LR }
  v2 = result;
  if ( dynamic_glass_item_cnt > 64 )
  {
    NuErrorProlog("C:/source/crashwoc/code/nu3dx/numtl.c", 2432);
    __asm { mtspr     LR, r0 }
    result = ((int (__fastcall *)(const char *))loc_800B3EC4)("Too many dynamic glass items");
  }
  v4 = dynamic_glass_item_cnt;
  if ( dynamic_glass_item_cnt <= 63 )
  {
    v5 = &dynamic_glass_items[4 * dynamic_glass_item_cnt];
    v5[1] = a2;
    v5[3] = v2;
    *((float *)v5 + 2) = 0.0;
    if ( v4 > 0 )
      dynamic_glass_items[4 * v4 - 4] = (int)v5;
    ++dynamic_glass_item_cnt;
  }
  __asm { mtspr     LR, r0 }
  return result;
}

int NuMtlClearGlassList()
{
  int result; // r3

  __asm { mfspr     r0, LR }
  result = memset(dynamic_glass_items, 0, sizeof(dynamic_glass_items));
  dynamic_glass_item_cnt = 0;
  __asm { mtspr     LR, r0 }
  return result;
}

int NuMtlRenderUpd()
{
  int result; // r3

  __asm { mfspr     r0, LR }
  faceonitem_cnt = 512;
  faceonmtl_cnt = 0;
  otitem_cnt = 2024;
  result = NuTexSetTexture(0, 0);
  __asm { mtspr     LR, r0 }
  return result;
}

int __fastcall NuMtlSetRenderStates(int a1)
{
  double v2; // fp0
  double v3; // fp10
  int v4; // r0
  double v5; // fp9
  double v6; // fp8
  double v7; // fp13
  double v8; // fp12
  double v9; // fp11
  int v10; // r0
  int v11; // r3
  int v12; // r5
  int result; // r3
  int v14; // r9
  float v15[12]; // [sp+8h] [-50h] BYREF
  float v16; // [sp+38h] [-20h]
  float v17; // [sp+3Ch] [-1Ch]
  float v18; // [sp+40h] [-18h]
  float v19; // [sp+44h] [-14h]
  float v20; // [sp+48h] [-10h]

  __asm { mfspr     r0, LR }
  v2 = *(float *)(a1 + 52);
  v3 = *(float *)(a1 + 20);
  v4 = HIWORD(*(_DWORD *)(a1 + 4)) & 3;
  v5 = *(float *)(a1 + 24);
  v6 = *(float *)(a1 + 28);
  v7 = *(float *)(a1 + 8);
  v8 = *(float *)(a1 + 12);
  v9 = *(float *)(a1 + 16);
  v15[0] = *(float *)(a1 + 20);
  v15[1] = v5;
  v15[2] = v6;
  v15[3] = v2;
  v15[4] = v7;
  v15[5] = v8;
  v15[6] = v9;
  v15[7] = v2;
  if ( v4 == 2 )
  {
    v16 = v3;
    v17 = v5;
    v18 = v6;
  }
  else
  {
    v16 = v7;
    v17 = v8;
    v18 = v9;
  }
  v19 = v2;
  v20 = *(float *)(a1 + 48);
  GS_SetMaterial(v15);
  v10 = *(_DWORD *)(a1 + 4) >> 30;
  switch ( v10 )
  {
    case 0:
      GS_SetBlendSrc(1, 1, 0);
      v11 = 7;
LABEL_11:
      GS_SetAlphaCompare(v11, 0);
      break;
    case 1:
      v12 = 5;
LABEL_10:
      GS_SetBlendSrc(1, 4, v12);
      v11 = 4;
      goto LABEL_11;
    case 2:
      v12 = 1;
      goto LABEL_10;
    case 3:
      GS_SetBlendSrc(1, 0, 3);
      GS_SetAlphaCompare(4, 0);
      break;
  }
  GS_SetAlphaCompare(4, 0);
  result = func_800C71F0(128, 0);
  if ( (*(_DWORD *)(a1 + 4) & 0xC0000) == 0 )
    result = GS_SetZCompare(1, 1, 3);
  if ( ((*(_DWORD *)(a1 + 4) >> 18) & 3) == 1 )
    result = GS_SetZCompare(1, 0, 3);
  if ( ((*(_DWORD *)(a1 + 4) >> 18) & 3) == 2 )
    result = GS_SetZCompare(1, 1, 7);
  if ( ((*(_DWORD *)(a1 + 4) >> 18) & 3) == 3 )
    result = GS_SetZCompare(0, 0, 7);
  v14 = *(_DWORD *)(a1 + 4);
  IsObjLit = 0;
  if ( (v14 & 0x30000) == 0 )
  {
    GS_EnableLighting(1);
    result = GS_EnableSpecular(0);
    IsObjLit = 1;
  }
  if ( (HIWORD(*(_DWORD *)(a1 + 4)) & 3) == 1 )
  {
    GS_EnableLighting(1);
    result = GS_EnableSpecular(1);
    IsObjLit = 2;
  }
  if ( (HIWORD(*(_DWORD *)(a1 + 4)) & 3) == 2 )
    result = GS_EnableLighting(0);
  if ( (*(_DWORD *)(a1 + 4) & 0x8000) == 0 )
  {
    GS_EnableColorVertex(0);
    GS_SetMaterialSourceAmbient(0);
    result = GS_SetMaterialSourceEmissive(0);
  }
  if ( (*(_DWORD *)(a1 + 4) & 0x8000) != 0 )
  {
    GS_EnableColorVertex(1);
    GS_SetMaterialSourceAmbient(1);
    if ( (HIWORD(*(_DWORD *)(a1 + 4)) & 3) == 2 )
      result = GS_SetMaterialSourceEmissive(1);
    else
      result = GS_SetMaterialSourceEmissive(0);
  }
  __asm { mtspr     LR, r0 }
  return result;
}

void __fastcall NuMtlAnimate(double a1)
{
  int v1; // r9
  int v2; // r11

  v1 = smlist;
  for ( sinetime_246 = sinetime_246 + (float)a1; v1; v1 = *(_DWORD *)(v1 + 96) )
  {
    v2 = *(_DWORD *)(v1 + 64);
    if ( (_BYTE)v2 )
    {
      if ( (v2 & 0xF0) == 32 )
        *(float *)(v1 + 68) = *(float *)(v1 + 76) * (float)a1;
      if ( (*(_DWORD *)(v1 + 64) & 0xF) == 2 )
        *(float *)(v1 + 72) = *(float *)(v1 + 80) * (float)a1;
    }
  }
}

int __fastcall NuMtlUVAnimation(int result)
{
  int v1; // ctr
  _DWORD *v2; // r10
  int v3; // r11
  int v4; // r9
  double v5; // fp11
  double v6; // fp12
  int v7; // r9
  double v8; // fp13
  int v9; // r9
  int v10; // r9

  v2 = *(_DWORD **)(result + 12);
  if ( v2 )
  {
    while ( 1 )
    {
      v3 = v2[1];
      v4 = *(_DWORD *)(v3 + 64);
      if ( (v4 & 0xF0) == 32 )
        break;
      if ( (v4 & 0xF) == 2 )
      {
        v5 = 0.0;
LABEL_6:
        if ( (*(_DWORD *)(v3 + 64) & 0xF) == 2 )
          v6 = *(float *)(v3 + 72);
        else
          v6 = 0.0;
        if ( v6 == 0.0 || v5 == 0.0 )
        {
          if ( v6 != 0.0 || v5 == 0.0 )
          {
            if ( v6 != 0.0 && v5 == 0.0 && v2[3] == 89 )
            {
              v10 = v2[6];
              if ( (int)v2[4] > 0 )
              {
                __asm { mtspr     CTR, r0 }
                do
                {
                  *(float *)(v10 + 32) = (float)(*(float *)(v10 + 32) + (float)v6) + (float)v6;
                  v10 += 36;
                  --v1;
                }
                while ( v1 );
              }
            }
          }
          else if ( v2[3] == 89 )
          {
            v9 = v2[6];
            if ( (int)v2[4] > 0 )
            {
              __asm { mtspr     CTR, r0 }
              do
              {
                *(float *)(v9 + 28) = (float)(*(float *)(v9 + 28) + (float)v5) + (float)v5;
                v9 += 36;
                --v1;
              }
              while ( v1 );
            }
          }
        }
        else if ( v2[3] == 89 )
        {
          v7 = v2[6];
          if ( (int)v2[4] > 0 )
          {
            __asm { mtspr     CTR, r0 }
            do
            {
              v8 = (float)((float)(*(float *)(v7 + 32) + (float)v6) + (float)v6);
              *(float *)(v7 + 28) = (float)(*(float *)(v7 + 28) + (float)v5) + (float)v5;
              *(float *)(v7 + 32) = v8;
              v7 += 36;
              --v1;
            }
            while ( v1 );
          }
        }
      }
      v2 = (_DWORD *)*v2;
      if ( !v2 )
        return result;
    }
    v5 = *(float *)(v3 + 68);
    goto LABEL_6;
  }
  return result;
}

int NuMtlClearOt()
{
  int result; // r3

  __asm { mfspr     r0, LR }
  result = memset(ot, 0, sizeof(ot));
  __asm { mtspr     LR, r0 }
  return result;
}