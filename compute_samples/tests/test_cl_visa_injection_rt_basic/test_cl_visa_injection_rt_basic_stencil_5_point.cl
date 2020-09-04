/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_stencil_5_point(const int size, global const TYPE *input,
                     global TYPE *output) {
  const int i = get_global_id(0);
  const int j = get_global_id(1);

  if ((i >= 5) && (i < size - 5) && (j >= 5) && (j < size - 5)) {
    output[i * size + j] =
        0.02f * (input[i * size + (j + 5)] + input[(i + 5) * size + j] -
                 input[(i - 5) * size + j] - input[i * size + (j - 5)]) +
        0.025f * (input[i * size + (j + 4)] + input[(i + 4) * size + j] -
                  input[(i - 4) * size + j] - input[i * size + (j - 4)]) +
        0.0333333333333f *
            (input[i * size + (j + 3)] + input[(i + 3) * size + j] -
             input[(i - 3) * size + j] - input[i * size + (j - 3)]) +
        0.05f * (input[i * size + (j + 2)] + input[(i + 2) * size + j] -
                 input[(i - 2) * size + j] - input[i * size + (j - 2)]) +
        0.1f * (input[i * size + (j + 1)] + input[(i + 1) * size + j] -
                input[(i - 1) * size + j] - input[i * size + (j - 1)]);
  }
}

__attribute__((intel_reqd_sub_group_size(16))) kernel void
test_asm_stencil_5_point(const int size, global const float *input,
                         global float *output) {
  const int i = get_global_id(0);
  const int j = get_global_id(1);

  __asm__ volatile(
      // clang-format off
      "{\n"
      ".decl size_minus_5 v_type=G type=d num_elts=1 align=dword\n"
      "    add (M1_NM, 1) size_minus_5(0,0)<1> %[size](0,0)<0;1,0> -5:w\n"
      ".decl P1 v_type=P num_elts=16\n"
      "    cmp.lt (M1, 16) P1 %[i](0,0)<1;1,0> size_minus_5(0,0)<0;1,0>\n"
      ".decl P2 v_type=P num_elts=16\n"
      "    cmp.gt (M1, 16) P2 %[i](0,0)<1;1,0> 0x4:d\n"
      "    and (M1, 16) P2 P2 P1\n"
      ".decl P3 v_type=P num_elts=16\n"
      "    cmp.gt (M1, 16) P3 %[j](0,0)<1;1,0> 0x4:d\n"
      "    and (M1, 16) P3 P3 P2\n"
      ".decl P4 v_type=P num_elts=16\n"
      "    cmp.lt (M1, 16) P4 %[j](0,0)<1;1,0> size_minus_5(0,0)<0;1,0>\n"
      "    and (M1, 16) P4 P4 P3\n"
      "    (!P4) goto (M1, 16) my_label0\n"

      ".decl result v_type=G type=f num_elts=16 align=hword\n"
      ".decl OO2f_acc v_type=G type=f num_elts=16 align=hword\n"
      "    svm_gather.4.1 (M1, 16) %[input_i_size_jp5_addr].0 OO2f_acc.0\n"
      ".decl tmp0 v_type=G type=f num_elts=16 align=hword\n"
      "    svm_gather.4.1 (M1, 16) %[input_ip5_size_j_addr].0 tmp0.0\n"
      "    add (M1, 16) OO2f_acc(0,0)<1> OO2f_acc(0,0)<1;1,0> tmp0(0,0)<1;1,0>\n"
      "    svm_gather.4.1 (M1, 16) %[input_im5_size_j_addr].0 tmp0.0\n"
      "    add (M1, 16) OO2f_acc(0,0)<1> OO2f_acc(0,0)<1;1,0> (-)tmp0(0,0)<1;1,0>\n"
      "    svm_gather.4.1 (M1, 16) %[input_i_size_jm5_addr].0 tmp0.0\n"
      "    add (M1, 16) OO2f_acc(0,0)<1> OO2f_acc(0,0)<1;1,0> (-)tmp0(0,0)<1;1,0>\n"
      "    mul (M1, 16) OO2f_acc(0,0)<1> OO2f_acc(0,0)<1;1,0> %[OO2f]\n"

      ".decl OO25f_acc v_type=G type=f num_elts=16 align=hword\n"
      "    svm_gather.4.1 (M1, 16) %[input_i_size_jp4_addr].0 OO25f_acc.0\n"
      "    svm_gather.4.1 (M1, 16) %[input_ip4_size_j_addr].0 tmp0.0\n"
      "    add (M1, 16) OO25f_acc(0,0)<1> OO25f_acc(0,0)<1;1,0> tmp0(0,0)<1;1,0>\n"
      "    svm_gather.4.1 (M1, 16) %[input_im4_size_j_addr].0 tmp0.0\n"
      "    add (M1, 16) OO25f_acc(0,0)<1> OO25f_acc(0,0)<1;1,0> (-)tmp0(0,0)<1;1,0>\n"
      "    svm_gather.4.1 (M1, 16) %[input_i_size_jm4_addr].0 tmp0.0\n"
      "    add (M1, 16) OO25f_acc(0,0)<1> OO25f_acc(0,0)<1;1,0> (-)tmp0(0,0)<1;1,0>\n"
      ".decl itmp0 v_type=G type=f num_elts=1 align=dword\n"
      "    mov (M1_NM, 1) itmp0(0,0)<1> %[OO25f]\n"
      "    mad (M1, 16) result(0,0)<1> itmp0(0,0)<0;1,0> OO25f_acc(0,0)<1;1,0> OO2f_acc(0,0)<1;1,0>\n"

      ".decl OO3f_acc v_type=G type=f num_elts=16 align=hword\n"
      "    svm_gather.4.1 (M1, 16) %[input_i_size_jp3_addr].0 OO3f_acc.0\n"
      "    svm_gather.4.1 (M1, 16) %[input_ip3_size_j_addr].0 tmp0.0\n"
      "    add (M1, 16) OO3f_acc(0,0)<1> OO3f_acc(0,0)<1;1,0> tmp0(0,0)<1;1,0>\n"
      "    svm_gather.4.1 (M1, 16) %[input_im3_size_j_addr].0 tmp0.0\n"
      "    add (M1, 16) OO3f_acc(0,0)<1> OO3f_acc(0,0)<1;1,0> (-)tmp0(0,0)<1;1,0>\n"
      "    svm_gather.4.1 (M1, 16) %[input_i_size_jm3_addr].0 tmp0.0\n"
      "    add (M1, 16) OO3f_acc(0,0)<1> OO3f_acc(0,0)<1;1,0> (-)tmp0(0,0)<1;1,0>\n"
      "    mov (M1_NM, 1) itmp0(0,0)<1> %[OO3f]\n"
      "    mad (M1, 16) result(0,0)<1> itmp0(0,0)<0;1,0> OO3f_acc(0,0)<1;1,0> result(0,0)<1;1,0>\n"

      ".decl OO5f_acc v_type=G type=f num_elts=16 align=hword\n"
      "    svm_gather.4.1 (M1, 16) %[input_i_size_jp2_addr].0 OO5f_acc.0\n"
      "    svm_gather.4.1 (M1, 16) %[input_ip2_size_j_addr].0 tmp0.0\n"
      "    add (M1, 16) OO5f_acc(0,0)<1> OO5f_acc(0,0)<1;1,0> tmp0(0,0)<1;1,0>\n"
      "    svm_gather.4.1 (M1, 16) %[input_im2_size_j_addr].0 tmp0.0\n"
      "    add (M1, 16) OO5f_acc(0,0)<1> OO5f_acc(0,0)<1;1,0> (-)tmp0(0,0)<1;1,0>\n"
      "    svm_gather.4.1 (M1, 16) %[input_i_size_jm2_addr].0 tmp0.0\n"
      "    add (M1, 16) OO5f_acc(0,0)<1> OO5f_acc(0,0)<1;1,0> (-)tmp0(0,0)<1;1,0>\n"
      "    mov (M1_NM, 1) itmp0(0,0)<1> %[OO5f]\n"
      "    mad (M1, 16) result(0,0)<1> itmp0(0,0)<0;1,0> OO5f_acc(0,0)<1;1,0> result(0,0)<1;1,0>\n"

      ".decl O1f_acc v_type=G type=f num_elts=16 align=hword\n"
      "    svm_gather.4.1 (M1, 16) %[input_i_size_jp1_addr].0 O1f_acc.0\n"
      "    svm_gather.4.1 (M1, 16) %[input_ip1_size_j_addr].0 tmp0.0\n"
      "    add (M1, 16) O1f_acc(0,0)<1> O1f_acc(0,0)<1;1,0> tmp0(0,0)<1;1,0>\n"
      "    svm_gather.4.1 (M1, 16) %[input_im1_size_j_addr].0 tmp0.0\n"
      "    add (M1, 16) O1f_acc(0,0)<1> O1f_acc(0,0)<1;1,0> (-)tmp0(0,0)<1;1,0>\n"
      "    svm_gather.4.1 (M1, 16) %[input_i_size_jm1_addr].0 tmp0.0\n"
      "    add (M1, 16) O1f_acc(0,0)<1> O1f_acc(0,0)<1;1,0> (-)tmp0(0,0)<1;1,0>\n"
      "    mov (M1_NM, 1) itmp0(0,0)<1> %[O1f]\n"
      "    mad (M1, 16) result(0,0)<1> itmp0(0,0)<0;1,0> O1f_acc(0,0)<1;1,0> result(0,0)<1;1,0>\n"

      "    svm_scatter.4.1 (M1, 16) %[output_i_size_j].0 result.0\n"

      "my_label0:\n"
      "}"
      // clang-format on
      :
      : [size] "rw"(size), [i] "rw"(i), [j] "rw"(j),
        [input_i_size_jp1_addr] "rw"(&input[i * size + (j + 1)]),
        [input_i_size_jp2_addr] "rw"(&input[i * size + (j + 2)]),
        [input_i_size_jp3_addr] "rw"(&input[i * size + (j + 3)]),
        [input_i_size_jp4_addr] "rw"(&input[i * size + (j + 4)]),
        [input_i_size_jp5_addr] "rw"(&input[i * size + (j + 5)]),
        [input_i_size_jm1_addr] "rw"(&input[i * size + (j - 1)]),
        [input_i_size_jm2_addr] "rw"(&input[i * size + (j - 2)]),
        [input_i_size_jm3_addr] "rw"(&input[i * size + (j - 3)]),
        [input_i_size_jm4_addr] "rw"(&input[i * size + (j - 4)]),
        [input_i_size_jm5_addr] "rw"(&input[i * size + (j - 5)]),

        [input_ip1_size_j_addr] "rw"(&input[(i + 1) * size + j]),
        [input_ip2_size_j_addr] "rw"(&input[(i + 2) * size + j]),
        [input_ip3_size_j_addr] "rw"(&input[(i + 3) * size + j]),
        [input_ip4_size_j_addr] "rw"(&input[(i + 4) * size + j]),
        [input_ip5_size_j_addr] "rw"(&input[(i + 5) * size + j]),
        [input_im1_size_j_addr] "rw"(&input[(i - 1) * size + j]),
        [input_im2_size_j_addr] "rw"(&input[(i - 2) * size + j]),
        [input_im3_size_j_addr] "rw"(&input[(i - 3) * size + j]),
        [input_im4_size_j_addr] "rw"(&input[(i - 4) * size + j]),
        [input_im5_size_j_addr] "rw"(&input[(i - 5) * size + j]),
        [output_i_size_j] "rw"(&output[i * size + j]), [OO2f] "i"(0.02f),
        [OO25f] "i"(0.025f), [O1f] "i"(0.1f), [OO3f] "i"(0.0333333333333f),
        [OO5f] "i"(0.05f));
}
