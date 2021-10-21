/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_region(const global uint *pSrc, global uint *pDst) {
  const int x = get_global_id(0);
  const int y = get_global_id(1);

  const int width = get_global_size(0);
  const int height = get_global_size(1);

  const int iOffset = y * width;
  const int iPrev = iOffset - width;
  const int iNext = iOffset + width;
  uint uiRGBA[9];

  // Bound Check
  // DMGS
  // if (((iPrev + x - 1) >= 0) && ((iNext + x + 1) < (height * width)))
  __asm__ volatile(
      "{\n"
      ".decl my_P1 v_type=P num_elts=16\n"
      ".decl my_P2 v_type=P num_elts=16\n"
      ".decl iprev_plus_x v_type=G type=d num_elts=16 align=hword\n"
      "add (M1, 16) iprev_plus_x(0,0)<1> %0(0,0)<1;1,0> %2(0,0)<1;1,0>\n"
      "cmp.gt (M1, 16) my_P1 iprev_plus_x(0,0)<1;1,0> 0x0:d\n"
      "(!my_P1) goto (M1, 16) else_label\n"
      ".decl inext_plus_x v_type=G type=d num_elts=16 align=hword\n"
      "add (M1, 16) inext_plus_x(0,0)<1> %1(0,0)<1;1,0> %2(0,0)<1;1,0>\n"
      "add (M1, 16) inext_plus_x(0,0)<1> inext_plus_x(0,0)<1;1,0> 0x1:w\n"
      ".decl hxw v_type=G type=d num_elts=1 align=dword\n"
      "mul (M1_NM, 1) hxw(0,0)<1> %3(0,1)<0;1,0> %4(0,0)<0;1,0>\n"
      "cmp.lt (M1, 16) my_P2 inext_plus_x(0,0)<1;1,0> hxw(0,0)<0;1,0>\n"
      "(!my_P2) goto (M1, 16) else_label\n"
      // "goto (M1, 16) if_label\n"
      "}\n"
      :
      : "rw"(iPrev), "rw"(iNext), "rw"(x), "rw"(height), "rw"(width),
        "rw"(iOffset), "rw"(&pDst[iOffset + x]), "rw"(&pSrc[iOffset + x]));
  {
    __asm__ volatile("if_label:\n");
    // get pixels within aperture
    uiRGBA[0] = pSrc[iPrev + x - 1];
    uiRGBA[1] = pSrc[iPrev + x];
    uiRGBA[2] = pSrc[iPrev + x + 1];

    uiRGBA[3] = pSrc[iOffset + x - 1];
    uiRGBA[4] = pSrc[iOffset + x];
    uiRGBA[5] = pSrc[iOffset + x + 1];

    uiRGBA[6] = pSrc[iNext + x - 1];
    uiRGBA[7] = pSrc[iNext + x];
    uiRGBA[8] = pSrc[iNext + x + 1];

    uint uiResult = 0;
    uint uiMask = 0xFF;

    for (int ch = 0; ch < 4; ch++) {

      // extract next color channel
      uint r0, r1, r2, r3, r4, r5, r6, r7, r8;
      r0 = uiRGBA[0] & uiMask;
      r1 = uiRGBA[1] & uiMask;
      r2 = uiRGBA[2] & uiMask;
      r3 = uiRGBA[3] & uiMask;
      r4 = uiRGBA[4] & uiMask;
      r5 = uiRGBA[5] & uiMask;
      r6 = uiRGBA[6] & uiMask;
      r7 = uiRGBA[7] & uiMask;
      r8 = uiRGBA[8] & uiMask;

      // perform partial bitonic sort to find current channel median
      uint uiMin = min(r0, r1);
      uint uiMax = max(r0, r1);
      r0 = uiMin;
      r1 = uiMax;

      uiMin = min(r3, r2);
      uiMax = max(r3, r2);
      r3 = uiMin;
      r2 = uiMax;

      uiMin = min(r2, r0);
      uiMax = max(r2, r0);
      r2 = uiMin;
      r0 = uiMax;

      uiMin = min(r3, r1);
      uiMax = max(r3, r1);
      r3 = uiMin;
      r1 = uiMax;

      uiMin = min(r1, r0);
      uiMax = max(r1, r0);
      r1 = uiMin;
      r0 = uiMax;

      uiMin = min(r3, r2);
      uiMax = max(r3, r2);
      r3 = uiMin;
      r2 = uiMax;

      uiMin = min(r5, r4);
      uiMax = max(r5, r4);
      r5 = uiMin;
      r4 = uiMax;

      uiMin = min(r7, r8);
      uiMax = max(r7, r8);
      r7 = uiMin;
      r8 = uiMax;

      uiMin = min(r6, r8);
      uiMax = max(r6, r8);
      r6 = uiMin;
      r8 = uiMax;

      uiMin = min(r6, r7);
      uiMax = max(r6, r7);
      r6 = uiMin;
      r7 = uiMax;

      uiMin = min(r4, r8);
      uiMax = max(r4, r8);
      r4 = uiMin;
      r8 = uiMax;

      uiMin = min(r4, r6);
      uiMax = max(r4, r6);
      r4 = uiMin;
      r6 = uiMax;

      uiMin = min(r5, r7);
      uiMax = max(r5, r7);
      r5 = uiMin;
      r7 = uiMax;

      uiMin = min(r4, r5);
      uiMax = max(r4, r5);
      r4 = uiMin;
      r5 = uiMax;

      uiMin = min(r6, r7);
      uiMax = max(r6, r7);
      r6 = uiMin;
      r7 = uiMax;

      uiMin = min(r0, r8);
      uiMax = max(r0, r8);
      r0 = uiMin;
      r8 = uiMax;

      r4 = max(r0, r4);
      r5 = max(r1, r5);

      r6 = max(r2, r6);
      r7 = max(r3, r7);

      r4 = min(r4, r6);
      r5 = min(r5, r7);

      // store found median into result
      uiResult |= r4;

      // update channel mask
      uiMask <<= 8;
    }

    // store result into memory
    pDst[iOffset + x] = uiResult;
    __asm__ volatile("goto (M1, 1) ret_label\n");
  }
  // } else {
  __asm__ volatile("else_label:\n");
  pDst[iOffset + x] = pSrc[iOffset + x];
  //}
  __asm__ volatile("ret_label:\n");
}
