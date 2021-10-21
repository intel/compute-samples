/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_for_loop(const global uint *pSrc, global uint *pDst) {
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
  if (((iPrev + x - 1) >= 0) && ((iNext + x + 1) < (height * width))) {

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

    __asm__ volatile(
        // clang-format off
        ".decl ch_069 v_type=G type=d num_elts=1 align=dword\n"
        "mov (M1_NM, 1) ch_069(0,0)<1> 0x0:d\n"
        "my_label4:\n"
        ".decl and_ v_type=G type=d num_elts=16 align=hword\n"
        "    and (M1, 16) and_(0,0)<1> %[uiRGBA0](0,0)<1;1,0> %[uiMask](0,0)<0;1,0>\n"
        ".decl and59 v_type=G type=d num_elts=16 align=hword\n"
        "    and (M1, 16) and59(0,0)<1> %[uiRGBA1](0,0)<1;1,0> %[uiMask](0,0)<0;1,0>\n"
        ".decl and61 v_type=G type=d num_elts=16 align=hword\n"
        "    and (M1, 16) and61(0,0)<1> %[uiRGBA2](4,0)<1;1,0> %[uiMask](0,0)<0;1,0>\n"
        ".decl and63 v_type=G type=d num_elts=16 align=hword\n"
        "    and (M1, 16) and63(0,0)<1> %[uiRGBA3](0,0)<1;1,0> %[uiMask](0,0)<0;1,0>\n"
        ".decl and65 v_type=G type=d num_elts=16 align=hword\n"
        "    and (M1, 16) and65(0,0)<1> %[uiRGBA4](0,0)<1;1,0> %[uiMask](0,0)<0;1,0>\n"
        ".decl and67 v_type=G type=d num_elts=16 align=hword\n"
        "    and (M1, 16) and67(0,0)<1> %[uiRGBA5](4,0)<1;1,0> %[uiMask](0,0)<0;1,0>\n"
        ".decl and69 v_type=G type=d num_elts=16 align=hword\n"
        "    and (M1, 16) and69(0,0)<1> %[uiRGBA6](0,0)<1;1,0> %[uiMask](0,0)<0;1,0>\n"
        ".decl and71 v_type=G type=d num_elts=16 align=hword\n"
        "    and (M1, 16) and71(0,0)<1> %[uiRGBA7](0,0)<1;1,0> %[uiMask](0,0)<0;1,0>\n"
        ".decl and73 v_type=G type=d num_elts=16 align=hword\n"
        "    and (M1, 16) and73(0,0)<1> %[uiRGBA8](4,0)<1;1,0> %[uiMask](0,0)<0;1,0>\n"
        ".decl cond_i v_type=G type=d num_elts=16 align=hword\n"
        ".decl and59_0 v_type=G type=ud num_elts=16 align=hword alias=<and59, 0>\n"
        ".decl and__0 v_type=G type=ud num_elts=16 align=hword alias=<and_, 0>\n"
        ".decl cond_i_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i, 0>\n"
        "    min (M1, 16) cond_i_0(0,0)<1> and59_0(0,0)<1;1,0> and__0(0,0)<1;1,0>\n"
        ".decl cond_i64 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i64_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i64, 0>\n"
        "    max (M1, 16) cond_i64_0(0,0)<1> and59_0(0,0)<1;1,0> and__0(0,0)<1;1,0>\n"
        ".decl cond_i66 v_type=G type=d num_elts=16 align=hword\n"
        ".decl and61_0 v_type=G type=ud num_elts=16 align=hword alias=<and61, 0>\n"
        ".decl and63_0 v_type=G type=ud num_elts=16 align=hword alias=<and63, 0>\n"
        ".decl cond_i66_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i66, 0>\n"
        "    min (M1, 16) cond_i66_0(0,0)<1> and61_0(0,0)<1;1,0> and63_0(0,0)<1;1,0>\n"
        ".decl cond_i68 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i68_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i68, 0>\n"
        "    max (M1, 16) cond_i68_0(0,0)<1> and61_0(0,0)<1;1,0> and63_0(0,0)<1;1,0>\n"
        ".decl cond_i70 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i70_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i70, 0>\n"
        "    min (M1, 16) cond_i70_0(0,0)<1> cond_i_0(0,0)<1;1,0> cond_i68_0(0,0)<1;1,0>\n"
        ".decl cond_i72 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i72_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i72, 0>\n"
        "    max (M1, 16) cond_i72_0(0,0)<1> cond_i_0(0,0)<1;1,0> cond_i68_0(0,0)<1;1,0>\n"
        ".decl cond_i74 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i74_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i74, 0>\n"
        "    min (M1, 16) cond_i74_0(0,0)<1> cond_i64_0(0,0)<1;1,0> cond_i66_0(0,0)<1;1,0>\n"
        ".decl cond_i76 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i76_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i76, 0>\n"
        "    max (M1, 16) cond_i76_0(0,0)<1> cond_i64_0(0,0)<1;1,0> cond_i66_0(0,0)<1;1,0>\n"
        ".decl cond_i80 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i80_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i80, 0>\n"
        "    max (M1, 16) cond_i80_0(0,0)<1> cond_i72_0(0,0)<1;1,0> cond_i76_0(0,0)<1;1,0>\n"
        ".decl cond_i84 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i84_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i84, 0>\n"
        "    max (M1, 16) cond_i84_0(0,0)<1> cond_i70_0(0,0)<1;1,0> cond_i74_0(0,0)<1;1,0>\n"
        ".decl cond_i86 v_type=G type=d num_elts=16 align=hword\n"
        ".decl and65_0 v_type=G type=ud num_elts=16 align=hword alias=<and65, 0>\n"
        ".decl and67_0 v_type=G type=ud num_elts=16 align=hword alias=<and67, 0>\n"
        ".decl cond_i86_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i86, 0>\n"
        "    min (M1, 16) cond_i86_0(0,0)<1> and65_0(0,0)<1;1,0> and67_0(0,0)<1;1,0>\n"
        ".decl cond_i88 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i88_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i88, 0>\n"
        "    max (M1, 16) cond_i88_0(0,0)<1> and65_0(0,0)<1;1,0> and67_0(0,0)<1;1,0>\n"
        ".decl cond_i127 v_type=G type=d num_elts=16 align=hword\n"
        ".decl and73_0 v_type=G type=ud num_elts=16 align=hword alias=<and73, 0>\n"
        ".decl and71_0 v_type=G type=ud num_elts=16 align=hword alias=<and71, 0>\n"
        ".decl cond_i127_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i127, 0>\n"
        "    min (M1, 16) cond_i127_0(0,0)<1> and73_0(0,0)<1;1,0> and71_0(0,0)<1;1,0>\n"
        ".decl cond_i125 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i125_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i125, 0>\n"
        "    max (M1, 16) cond_i125_0(0,0)<1> and73_0(0,0)<1;1,0> and71_0(0,0)<1;1,0>\n"
        ".decl cond_i123 v_type=G type=d num_elts=16 align=hword\n"
        ".decl and69_0 v_type=G type=ud num_elts=16 align=hword alias=<and69, 0>\n"
        ".decl cond_i123_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i123, 0>\n"
        "    min (M1, 16) cond_i123_0(0,0)<1> cond_i125_0(0,0)<1;1,0> and69_0(0,0)<1;1,0>\n"
        ".decl cond_i121 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i121_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i121, 0>\n"
        "    max (M1, 16) cond_i121_0(0,0)<1> cond_i125_0(0,0)<1;1,0> and69_0(0,0)<1;1,0>\n"
        ".decl cond_i119 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i119_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i119, 0>\n"
        "    min (M1, 16) cond_i119_0(0,0)<1> cond_i127_0(0,0)<1;1,0> cond_i123_0(0,0)<1;1,0>\n"
        ".decl cond_i117 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i117_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i117, 0>\n"
        "    max (M1, 16) cond_i117_0(0,0)<1> cond_i127_0(0,0)<1;1,0> cond_i123_0(0,0)<1;1,0>\n"
        ".decl cond_i115 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i115_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i115, 0>\n"
        "    min (M1, 16) cond_i115_0(0,0)<1> cond_i121_0(0,0)<1;1,0> cond_i88_0(0,0)<1;1,0>\n"
        ".decl cond_i113 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i113_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i113, 0>\n"
        "    max (M1, 16) cond_i113_0(0,0)<1> cond_i121_0(0,0)<1;1,0> cond_i88_0(0,0)<1;1,0>\n"
        ".decl cond_i111 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i111_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i111, 0>\n"
        "    min (M1, 16) cond_i111_0(0,0)<1> cond_i119_0(0,0)<1;1,0> cond_i115_0(0,0)<1;1,0>\n"
        ".decl cond_i109 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i109_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i109, 0>\n"
        "    max (M1, 16) cond_i109_0(0,0)<1> cond_i119_0(0,0)<1;1,0> cond_i115_0(0,0)<1;1,0>\n"
        ".decl cond_i107 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i107_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i107, 0>\n"
        "    min (M1, 16) cond_i107_0(0,0)<1> cond_i117_0(0,0)<1;1,0> cond_i86_0(0,0)<1;1,0>\n"
        ".decl cond_i105 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i105_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i105, 0>\n"
        "    max (M1, 16) cond_i105_0(0,0)<1> cond_i117_0(0,0)<1;1,0> cond_i86_0(0,0)<1;1,0>\n"
        ".decl cond_i103 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i103_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i103, 0>\n"
        "    min (M1, 16) cond_i103_0(0,0)<1> cond_i107_0(0,0)<1;1,0> cond_i111_0(0,0)<1;1,0>\n"
        ".decl cond_i101 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i101_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i101, 0>\n"
        "    min (M1, 16) cond_i101_0(0,0)<1> cond_i105_0(0,0)<1;1,0> cond_i109_0(0,0)<1;1,0>\n"
        ".decl cond_i99 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i99_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i99, 0>\n"
        "    min (M1, 16) cond_i99_0(0,0)<1> cond_i113_0(0,0)<1;1,0> cond_i80_0(0,0)<1;1,0>\n"
        "    max (M1, 16) cond_i99_0(0,0)<1> cond_i103_0(0,0)<1;1,0> cond_i99_0(0,0)<1;1,0>\n"
        ".decl cond_i95 v_type=G type=d num_elts=16 align=hword\n"
        ".decl cond_i95_0 v_type=G type=ud num_elts=16 align=hword alias=<cond_i95, 0>\n"
        "    max (M1, 16) cond_i95_0(0,0)<1> cond_i101_0(0,0)<1;1,0> cond_i84_0(0,0)<1;1,0>\n"
        "    min (M1, 16) cond_i95_0(0,0)<1> cond_i95_0(0,0)<1;1,0> cond_i99_0(0,0)<1;1,0>\n"
        "    or (M1, 16) %[uiResult](0,0)<1> %[uiResult](0,0)<1;1,0> cond_i95(0,0)<1;1,0>\n"
        ".decl my_P3 v_type=P num_elts=16\n"
        ".decl ch_069_0 v_type=G type=ud num_elts=1 align=dword alias=<ch_069, 0>\n"
        "    cmp.lt (M1_NM, 16) my_P3 ch_069_0(0,0)<0;1,0> 0x3:ud\n"
        "    (!my_P3) goto (M1, 1) my_label5\n"
        "my_label6:\n"
        "    shl (M1_NM, 1) %[uiMask](0,0)<1> %[uiMask](0,0)<0;1,0> 0x8:d\n"
        "    add (M1_NM, 1) ch_069(0,0)<1> ch_069(0,0)<0;1,0> 0x1:w\n"
        "    goto (M1, 1) my_label4\n"
        // clang-format on
        : [uiResult] "+rw"(uiResult)
        : [uiMask] "rw.u"(uiMask), [uiRGBA0] "rw"(uiRGBA[0]),
          [uiRGBA1] "rw"(uiRGBA[1]), [uiRGBA2] "rw"(uiRGBA[2]),
          [uiRGBA3] "rw"(uiRGBA[3]), [uiRGBA4] "rw"(uiRGBA[4]),
          [uiRGBA5] "rw"(uiRGBA[5]), [uiRGBA6] "rw"(uiRGBA[6]),
          [uiRGBA7] "rw"(uiRGBA[7]), [uiRGBA8] "rw"(uiRGBA[8]));

    // store result into memory
    __asm__ volatile("my_label5:");
    pDst[iOffset + x] = uiResult;
  } else {
    pDst[iOffset + x] = pSrc[iOffset + x];
  }
}
