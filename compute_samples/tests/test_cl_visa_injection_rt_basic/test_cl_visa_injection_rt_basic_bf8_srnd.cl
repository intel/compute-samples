/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_bf8_srnd(global char *const dst, global const half *const src,
              global const half *const rnd) {
  const size_t tid = get_global_id(0);

  dst[tid] = intel_convert_f16_to_bf8_srnd(src[tid], rnd[tid]);
}

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_asm_bf8_srnd(global char *const dst, global const half *const src,
                  global const half *const rnd) {
  const size_t tid = get_global_id(0);

  __asm__ volatile(
      // clang-format off
      "{\n"
      ".decl my_bf8 v_type=G type=ub num_elts=16 alias=<%0, 0>\n"
      "srnd (M1, 16) my_bf8(0,0)<1> %1(0,0)<1;1,0> %2(0,0)<1;1,0>\n"
      "}\n"
      // clang-format on
      : "=rw"(dst[tid])
      : "rw"(src[tid]), "rw"(rnd[tid]));
}
