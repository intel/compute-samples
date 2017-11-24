/*
 * Copyright(c) 2017 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define SEARCH_WIN_CFG CLK_AVC_ME_SEARCH_WINDOW_EXHAUSTIVE_INTEL

__kernel __attribute__((intel_reqd_sub_group_size(16))) void
downsample_3_tier(__read_only image2d_t input, __write_only image2d_t out2,
                  __write_only image2d_t out4, __write_only image2d_t out8) {
  int x = get_group_id(0) * 16 * 4;
  int y = get_group_id(1) * 16;

  uint8 p0, p1;
  p0 = intel_sub_group_block_read8(input, (int2)(x, y));
  p1 = intel_sub_group_block_read8(input, (int2)(x, y + 8));

  uchar4 r0 = as_uchar4(p0.s0);
  uchar4 r1 = as_uchar4(p0.s1);
  uchar4 r2 = as_uchar4(p0.s2);
  uchar4 r3 = as_uchar4(p0.s3);
  uchar4 r4 = as_uchar4(p0.s4);
  uchar4 r5 = as_uchar4(p0.s5);
  uchar4 r6 = as_uchar4(p0.s6);
  uchar4 r7 = as_uchar4(p0.s7);
  uchar4 r8 = as_uchar4(p1.s0);
  uchar4 r9 = as_uchar4(p1.s1);
  uchar4 r10 = as_uchar4(p1.s2);
  uchar4 r11 = as_uchar4(p1.s3);
  uchar4 r12 = as_uchar4(p1.s4);
  uchar4 r13 = as_uchar4(p1.s5);
  uchar4 r14 = as_uchar4(p1.s6);
  uchar4 r15 = as_uchar4(p1.s7);

  // Tier 1...

  uchar16 t1r;
  t1r.s0 = (r0.s0 + r0.s1 + r1.s0 + r1.s1 + 2) / 4;
  t1r.s1 = (r0.s2 + r0.s3 + r1.s2 + r1.s3 + 2) / 4;
  t1r.s2 = (r2.s0 + r2.s1 + r3.s0 + r3.s1 + 2) / 4;
  t1r.s3 = (r2.s2 + r2.s3 + r3.s2 + r3.s3 + 2) / 4;
  t1r.s4 = (r4.s0 + r4.s1 + r5.s0 + r5.s1 + 2) / 4;
  t1r.s5 = (r4.s2 + r4.s3 + r5.s2 + r5.s3 + 2) / 4;
  t1r.s6 = (r6.s0 + r6.s1 + r7.s0 + r7.s1 + 2) / 4;
  t1r.s7 = (r6.s2 + r6.s3 + r7.s2 + r7.s3 + 2) / 4;
  t1r.s8 = (r8.s0 + r8.s1 + r9.s0 + r9.s1 + 2) / 4;
  t1r.s9 = (r8.s2 + r8.s3 + r9.s2 + r9.s3 + 2) / 4;
  t1r.sa = (r10.s0 + r10.s1 + r11.s0 + r11.s1 + 2) / 4;
  t1r.sb = (r10.s2 + r10.s3 + r11.s2 + r11.s3 + 2) / 4;
  t1r.sc = (r12.s0 + r12.s1 + r13.s0 + r13.s1 + 2) / 4;
  t1r.sd = (r12.s2 + r12.s3 + r13.s2 + r13.s3 + 2) / 4;
  t1r.se = (r14.s0 + r14.s1 + r15.s0 + r15.s1 + 2) / 4;
  t1r.sf = (r14.s2 + r14.s3 + r15.s2 + r15.s3 + 2) / 4;
  int xg = x / 2;
  int yg = y / 2;

  ushort8 t1 = as_ushort8(t1r);
  intel_sub_group_media_block_write_us8((int2)(xg, yg), 16, 8, t1, out2);

  // Tier 2...

  uchar t2r0 = (t1r.s0 + t1r.s1 + t1r.s2 + t1r.s3 + 2) / 4;
  uchar t2r1 = (t1r.s4 + t1r.s5 + t1r.s6 + t1r.s7 + 2) / 4;
  uchar t2r2 = (t1r.s8 + t1r.s9 + t1r.sa + t1r.sb + 2) / 4;
  uchar t2r3 = (t1r.sc + t1r.sd + t1r.se + t1r.sf + 2) / 4;

  uchar4 t2;
  t2.s0 = t2r0;
  t2.s1 = t2r1;
  t2.s2 = t2r2;
  t2.s3 = t2r3;

  xg = get_group_id(0) * 16;
  yg = get_global_id(1) * 4;

  intel_sub_group_media_block_write_uc4((int2)(xg, yg), 16, 4, t2, out4);

  // Tier 3...

  uchar t3r0;
  uchar t3r1;

  int ind0 = (get_local_id(0) * 2) % 16;
  int ind1 = (get_local_id(0) * 2 + 1) % 16;

  uchar t2r0_frompos = intel_sub_group_shuffle(t2r0, ind0);
  uchar t2r1_frompos = intel_sub_group_shuffle(t2r1, ind0);
  uchar t2r2_frompos = intel_sub_group_shuffle(t2r2, ind0);
  uchar t2r3_frompos = intel_sub_group_shuffle(t2r3, ind0);

  uchar t2r0_fromleft = intel_sub_group_shuffle(t2r0, ind1);
  uchar t2r1_fromleft = intel_sub_group_shuffle(t2r1, ind1);
  uchar t2r2_fromleft = intel_sub_group_shuffle(t2r2, ind1);
  uchar t2r3_fromleft = intel_sub_group_shuffle(t2r3, ind1);

  t3r0 = (t2r0_frompos + t2r0_fromleft + t2r1_frompos + t2r1_fromleft + 2) / 4;
  t3r1 = (t2r2_frompos + t2r2_fromleft + t2r3_frompos + t2r3_fromleft + 2) / 4;

  if (get_local_id(0) >= 8) {
    t3r0 = 0;
  } else {
    t3r1 = 0;
  }

  t3r0 += t3r1;

  intel_sub_group_media_block_write_uc((int2)(xg / 2, yg / 2), 8, 2, t3r0,
                                       out8);
}
