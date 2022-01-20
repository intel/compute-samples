/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_tf32_cvt(global int *const d, global const float *const s) {
  const size_t tid = get_global_id(0);

  d[tid] = intel_convert_f32_to_tf32(s[tid]);
}

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_asm_tf32_cvt(global int *const d, global const float *const s) {
  const size_t tid = get_global_id(0);

  float sf = s[tid];
  int tf = 0;

  __asm__ volatile(
      // clang-format off
      "{\n"
      ".decl my_tf32 v_type=G type=ud num_elts=16 alias=<%0, 0>\n"
      "fcvt (M1, 16) my_tf32(0,0)<1> %1(0,0)<1;1,0>\n"
      "}\n"
      // clang-format on
      : "=rw"(tf)
      : "rw"(sf));

  d[tid] = tf;
}
