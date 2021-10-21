/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

// A[M:N], B[N:P], C[M:P], alpha, beta
// global_size -> [M:P]
// C <- alpha * AB + beta * C

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_gemm(const global TYPE *A, const global TYPE *B, global TYPE *C,
          const TYPE alpha, const TYPE beta, const uint N, const uint M) {
  size_t glob_row = get_global_id(0);
  size_t glob_col = get_global_id(1);

  TYPE acc = 0;
  for (size_t n = 0; n < N; ++n)
    acc += A[n * M + glob_row] * B[glob_col * N + n];

  C[glob_col * M + glob_row] = alpha * acc + beta * C[glob_col * M + glob_row];
}

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_asm_sgemm(const global float *A, const global float *B, global float *C,
               const float alpha, const float beta, const uint N,
               const uint M) {
  size_t glob_row = get_global_id(0);
  size_t glob_col = get_global_id(1);

  __asm__ volatile(
      // clang-format off
      "{\n"
      ".decl tmp1 v_type=G type=q num_elts=1 align=qword\n"
      ".decl m_0 v_type=G type=ud num_elts=1 align=dword alias=<%[M], 0>\n"
      "    mov (M1_NM, 1) tmp1(0,0)<1> m_0(0,0)<0;1,0>\n"
      ".decl tmp1_0 v_type=G type=ud num_elts=2 align=qword alias=<tmp1, 0>\n"
      ".decl tmp1_1 v_type=G type=d num_elts=2 align=qword alias=<tmp1, 0>\n"

      ".decl my_P1 v_type=P num_elts=16\n"
      "    cmp.eq (M1_NM, 16) my_P1 %[N](0,0)<0;1,0> 0x0:d\n"
      "    (!my_P1) goto (M1, 1) my_label0\n"

      "my_label1:\n"
      ".decl acc v_type=G type=f num_elts=16 align=hword\n"
      "    mov (M1, 16) acc(0,0)<1> 0x0:f\n"
      "    goto (M1, 1) my_label2\n"

      "my_label0:\n"
      ".decl n v_type=G type=q num_elts=1 align=qword\n"
      "    mov (M1_NM, 1) n(0,0)<1> 0x0:q\n"
      "    mov (M1, 16) acc(0,0)<1> 0x0:f\n"

      "my_label3:\n"
      ".decl n_M v_type=G type=q num_elts=1 align=qword\n"
      ".decl n_0 v_type=G type=ud num_elts=2 align=qword alias=<n, 0>\n"
      ".decl n_1 v_type=G type=d num_elts=2 align=qword alias=<n, 0>\n"
      ".decl n_M_Lo v_type=G type=ud num_elts=1 align=qword\n"
      ".decl n_M_Hi v_type=G type=d num_elts=1 align=qword\n"
      ".decl n_M_Tmp v_type=G type=d num_elts=1 align=qword\n"
      "    mul (M1_NM, 1) n_M_Lo(0,0)<1> n_0(0,0)<0;1,0> tmp1_0(0,0)<0;1,0>\n"
      "    mulh (M1_NM, 1) n_M_Hi(0,0)<1> n_0(0,0)<0;1,0> tmp1_0(0,0)<0;1,0>\n"
      "    mul (M1_NM, 1) n_M_Tmp(0,0)<1> n_0(0,0)<0;1,0> tmp1_1(0,1)<0;1,0>\n"
      "    add (M1_NM, 1) n_M_Hi(0,0)<1> n_M_Hi(0,0)<0;1,0> n_M_Tmp(0,0)<0;1,0>\n"
      "    mul (M1_NM, 1) n_M_Tmp(0,0)<1> n_1(0,1)<0;1,0> tmp1_0(0,0)<0;1,0>\n"
      "    add (M1_NM, 1) n_M_Hi(0,0)<1> n_M_Hi(0,0)<0;1,0> n_M_Tmp(0,0)<0;1,0>\n"
      ".decl n_M_0 v_type=G type=ud num_elts=2 align=qword alias=<n_M, 0>\n"
      "    mov (M1_NM, 1) n_M_0(0,0)<2> n_M_Lo(0,0)<0;1,0>\n"
      ".decl n_M_1 v_type=G type=d num_elts=2 align=qword alias=<n_M, 0>\n"
      "    mov (M1_NM, 1) n_M_1(0,1)<2> n_M_Hi(0,0)<0;1,0>\n"
      "    shl (M1_NM, 1) n_M(0,0)<1> n_M(0,0)<0;1,0> 0x2:q\n"
      ".decl add1 v_type=G type=q num_elts=16 align=hword\n"
      "    add (M1, 8) add1(0,0)<1> %[A_glob_row](0,0)<1;1,0> n_M(0,0)<0;1,0>\n"
      "    add (M3, 8) add1(2,0)<1> %[A_glob_row](2,0)<1;1,0> n_M(0,0)<0;1,0>\n"
      ".decl A_n_M_glob_row_addr v_type=G type=uq num_elts=16 align=hword alias=<add1, 0>\n"
      ".decl A_n_M_glob_row_val v_type=G type=f num_elts=16 align=hword\n"
      "    svm_gather.4.1 (M1, 16) A_n_M_glob_row_addr.0 A_n_M_glob_row_val.0\n"

      ".decl n_shift v_type=G type=q num_elts=1 align=qword\n"
      "    shl (M1_NM, 1) n_shift(0,0)<1> n(0,0)<0;1,0> 0x2:q\n"
      "    add (M1, 8) add1(0,0)<1> %[B_glob_col_N](0,0)<1;1,0> n_shift(0,0)<0;1,0>\n"
      "    add (M3, 8) add1(2,0)<1> %[B_glob_col_N](2,0)<1;1,0> n_shift(0,0)<0;1,0>\n"
      ".decl B_glob_col_N_n_addr v_type=G type=uq num_elts=16 align=hword alias=<add1, 0>\n"
      ".decl B_glob_col_N_n_val v_type=G type=f num_elts=16 align=hword\n"
      "    svm_gather.4.1 (M1, 16) B_glob_col_N_n_addr.0 B_glob_col_N_n_val.0\n"
      "    mad (M1, 16) acc(0,0)<1> B_glob_col_N_n_val(0,0)<1;1,0> A_n_M_glob_row_val(0,0)<1;1,0> acc(0,0)<1;1,0>\n"
      "    add (M1_NM, 1) n(0,0)<1> n(0,0)<0;1,0> 0x1:q\n"
      ".decl my_P2 v_type=P num_elts=16\n"
      ".decl my_P3 v_type=P num_elts=16\n"
      ".decl n_2 v_type=G type=ud num_elts=2 align=qword alias=<n, 0>\n"
      ".decl N_00 v_type=G type=ud num_elts=1 align=dword alias=<%[N], 0>\n"
      "    cmp.lt (M1_NM, 16) my_P2 n_2(0,0)<0;1,0> N_00(0,0)<0;1,0>\n"
      "    cmp.eq (M1_NM, 16) my_P3 n_2(0,1)<0;1,0> 0x0:d\n"
      "    and (M1_NM, 16) my_P3 my_P3 my_P2\n"
      "    (my_P3) goto (M1, 1) my_label3\n"

      "my_label2:\n"
      ".decl beta_C v_type=G type=f num_elts=16 align=hword\n"
      "    svm_gather.4.1 (M1, 16) %[C_glob_col_M_glob_row].0 beta_C.0\n"
      "    mul (M1, 16) beta_C(0,0)<1> beta_C(0,0)<1;1,0> %[beta](0,0)<0;1,0>\n"
      "    mad (M1, 16) beta_C(0,0)<1> %[alpha](0,0)<0;1,0> acc(0,0)<1;1,0> beta_C(0,0)<1;1,0>\n"
      "    svm_scatter.4.1 (M1, 16) %[C_glob_col_M_glob_row].0 beta_C.0\n"
      "}"
      // clang-format on
      :
      : [alpha] "rw"(alpha), [beta] "rw"(beta), [N] "rw"(N), [M] "rw"(M),
        [C_glob_col_M_glob_row] "rw"(&C[glob_col * M + glob_row]),
        [B_glob_col_N] "rw"(&B[glob_col * N]), [A_glob_row] "rw"(&A[glob_row]));
}
