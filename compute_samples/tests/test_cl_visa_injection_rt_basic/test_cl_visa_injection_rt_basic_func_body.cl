/*
 * Copyright (C) 2020 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

// Following typedefs and function foo1 were generated. Please do not modify it.
typedef uint T0;
typedef uint T1;
typedef uint T2;
typedef uint T3;
typedef uint T4;
typedef uint T5;
typedef uint T6;
typedef uint T7;
typedef uint T8;
typedef uint T9;
typedef uint T10;
typedef uint T11;
typedef uint T12;

T10 foo1(T0 x0, T9 x1, T11 x2) {
  T10 v0 = 4133464738U;
  T9 v1 = 3030533954U;
  T2 v2 = 2308249305U;
  T11 v3 = 908785887U;
  T12 v4 = 3607982689U;
  T12 v5 = 2644678137U;
  T8 v6 = 267790570U;
  if (((((~6733761529419U) ^ (+v0)) < ((~v0) | (+v0))) &&
       (((+v0) | (-v0)) < ((v0 << 31U) * (v0 << 31U))))) {
    return v0;
  }
  v0 = ((~v6) * ((v4 << 31U) - (v1 / 23U)));
  v1 = ((v4 >> 31U) | ((~v4) & (~v6)));
  v5 = ((-v6) * ((v6 << 31U) * (-v4)));
  v1 = (((-v5) ^ (v5 << 31U)) & (~x2));
  x1 = ((v6 % 43U) - ((v0 << 31U) | (v4 << 31U)));
  v0 = (((v5 << 31U) + (150114145666U << 31U)) + (+v4));
  if (((((v6 >> 31U)) > ((v3 << 31U) + (~v3))))) {
    x2 = (((~v0) & (14967150214U / 49U)) & (v0 % 1U));
    v6 = (((x0 >> 31U) & (v0 << 31U)) ^ (v0 / 33U));
    x1 = ((+x0) & ((~v0) % (((v5 >> 31U) % 40U) | 22U)));
  } else {
    v5 = (((x2 >> 31U) - (+x2)) * (-59963490915U));
    v0 = (((~v4) * (-v5)) - (v4 << 31U));
    v4 = (((v0 << 31U) & (v1 >> 31U)) - (+v4));
    v0 = (((11166088531334U << 31U) + (v4 / 41U)) * (~v5));
    x0 = ((x2 >> 31U) * ((v5 << 31U) + (~v4)));
  }
  v1 = (((+v3) ^ (v3 / 30U)) + (v1 << 31U));
  x1 = ((v1 >> 31U) & ((~x0) / (((v3 >> 31U) % 17U) | 29U)));
  v3 = (((v1 >> 31U) ^ (v4 << 31U)) ^ (-v0));
  v1 = (((-v1) ^ (v4 << 31U)) * (-v1));
  v5 = (((+v1) | (v0 << 31U)) & (-17865442898U));
  x1 = ((140744305U % 35U) * (+v5));
  v4 = (((-x1) & (x1 >> 31U)) | (-v6));
  v4 = ((v3 << 31U) | ((x1 << 31U) | (+v5)));
  return v0;
}

// following function does exactly the same as foo1 does
T10 foo1_in_asm(T0 x0, T9 x1, T11 x2) {
  T10 v0 = 4133464738U;
  __asm__("{\n"
          ".decl tmp v_type=G type=ud num_elts=16 align=GRF alias=<%3, 0>\n"
          "shr (M1, 16) %0(0,0)<1> tmp(0,0)<1;1,0> 0x1f:d\n"
          "add (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0> (-)%3(0,0)<1;1,0>\n"
          "mul (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0> 0x9e5bd9d:d\n"
          "not (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0>\n"
          "mul (M1, 16) %0(0,0)<1> %0(0,0)<1;1,0> 0xffac0a:d\n"
          "}\n"
          : "=rw"(v0)
          : "rw"(x0), "rw"(x1), "rw"(x2));
  return v0;
}

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_call_func1(global uint *const dst, global const uint *const s1,
                global const uint *const s2, global const uint *const s3) {
  const size_t tid = get_global_id(0);
  dst[tid] = foo1(s1[tid], s2[tid], s3[tid]);
}

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_call_func2(global uint *const dst, global const uint *const s1,
                global const uint *const s2, global const uint *const s3) {
  const size_t tid = get_global_id(0);
  dst[tid] = foo1_in_asm(s1[tid], s2[tid], s3[tid]);
}
