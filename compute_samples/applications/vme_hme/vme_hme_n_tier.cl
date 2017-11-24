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

#ifdef SEARCH_WIN_CFG
#undef SEARCH_WIN_CFG
#endif
#define SEARCH_WIN_CFG CLK_AVC_ME_SEARCH_WINDOW_EXHAUSTIVE_INTEL

ulong get_cost_center(short2 value) {
  short2 cost_center;
  // Cost coords are in QPEL resolution.
  cost_center.s0 = value.s0 << 2;
  cost_center.s1 = value.s1 << 2;
  uint2 cost_center_int;
  cost_center_int.s0 = as_uint(cost_center);
  cost_center_int.s1 = 0;
  return as_ulong(cost_center_int);
}

inline intel_sub_group_avc_ime_result_t
tier_downscaled_ime(__read_only image2d_t src_img,
                    __read_only image2d_t ref_img, ushort2 src_coord,
                    short2 ref_coord) {
  sampler_t accelerator = CLK_AVC_ME_INITIALIZE_INTEL;
  uchar sad_adjustment = CLK_AVC_ME_SAD_ADJUST_MODE_NONE_INTEL;
  uchar partition_mask = CLK_AVC_ME_PARTITION_MASK_8x8_INTEL;

  uchar slice_type = CLK_AVC_ME_SLICE_TYPE_PRED_INTEL;
  uchar qp = 25;
  uint2 packed_cost_table =
      intel_sub_group_avc_mce_get_default_inter_motion_vector_cost_table(
          slice_type, qp);
  uchar cost_precision = CLK_AVC_ME_COST_PRECISION_HPEL_INTEL;
  ulong cost_center = get_cost_center(0);
  sampler_t media_sampler = 0;

  // Evaluate IME operation for [0]: (predictor_tiern_1_x, predictor_tiern_1_y)
  intel_sub_group_avc_ime_result_single_reference_streamout_t resultsout;
  ulong shape_penalty = 0;

  intel_sub_group_avc_ime_result_t result;
  resultsout =
      ime_streamout(src_img, ref_img, src_coord, ref_coord, partition_mask,
                    sad_adjustment, cost_center, cost_precision,
                    packed_cost_table, shape_penalty, media_sampler);

  // Evaluate IME operation for [1]: (0, 0) if not already done so
  if (ref_coord.x != 0 || ref_coord.y != 0) {
    ref_coord = 0;
    resultsout = ime_streaminout(src_img, ref_img, src_coord, ref_coord,
                                 partition_mask, sad_adjustment, cost_center,
                                 cost_precision, packed_cost_table,
                                 shape_penalty, resultsout, media_sampler);
  }

  result = intel_sub_group_avc_ime_strip_single_reference_streamout(resultsout);
  return result;
}

__kernel __attribute__((reqd_work_group_size(16, 1, 1))) void
tier_n_hme(__read_only image2d_t src_img, __read_only image2d_t ref_img,
           __global short2 *prediction_motion_vector_buffer,
           __global short2 *motion_vector_buffer) {
  int2 gid = {get_group_id(0), get_group_id(1)};
  int2 ngrp = {get_num_groups(0), get_num_groups(1)};
  ushort2 search_size = intel_sub_group_ime_ref_window_size(SEARCH_WIN_CFG, 0);

  ushort2 src_coord = 0;
  short2 ref_coord = 0;

  src_coord.x = gid.x * 16;
  src_coord.y = gid.y * 16;

  if (prediction_motion_vector_buffer != NULL) {
    ref_coord =
        prediction_motion_vector_buffer[gid.x + gid.y * get_num_groups(0)] >> 2;
  } else {
    ref_coord = 0;
  }

  intel_sub_group_avc_ime_result_t result;

  // Evaluate tier IME on downscaled src & ref.
  result = tier_downscaled_ime(src_img, ref_img, src_coord, ref_coord);
  long mvs = intel_sub_group_avc_ime_get_motion_vectors(result);
  ushort dists = intel_sub_group_avc_ime_get_inter_distortions(result);

  // Perform 1/2 pixel resolution on downscaled image (corresponds to full pixel
  // for next image).
  sampler_t accelerator = CLK_AVC_ME_INITIALIZE_INTEL;
  uchar sub_pixel_mode = CLK_AVC_ME_SUBPIXEL_MODE_HPEL_INTEL;
  uchar sad_adjustment = CLK_AVC_ME_SAD_ADJUST_MODE_NONE_INTEL;
  uchar major_shape = CLK_AVC_ME_MAJOR_8x8_INTEL;
  uchar minor_shapes = CLK_AVC_ME_MINOR_8x8_INTEL;
  uchar directions = 0;

  intel_sub_group_avc_ref_payload_t payload;
  payload = intel_sub_group_avc_fme_initialize(src_coord, mvs, major_shape,
                                               minor_shapes, directions,
                                               sub_pixel_mode, sad_adjustment);

  intel_sub_group_avc_ref_result_t result_ref;
  result_ref = intel_sub_group_avc_ref_evaluate_with_single_reference(
      src_img, ref_img, accelerator, payload);

  mvs = intel_sub_group_avc_ref_get_motion_vectors(result_ref);
  dists = intel_sub_group_avc_ref_get_inter_distortions(result_ref);

  // Order the 8x8 block results properly scaled to use as predictors for the
  // next set of 16x16 blocks at next 2x tier.
  int sub_rows = (get_sub_group_local_id() / 8);
  int sub_cols = (get_sub_group_local_id() % 8) >> 2;
  int index = (gid.x * 2 + sub_cols) + (gid.y * 2 + sub_rows) * 2 * ngrp.x;
  int2 bi_mvs = as_int2(mvs);

  // Writing out (2x2) scaled predictors per block for next 2x tier.
  if ((get_sub_group_local_id() % 4) == 0) {
    motion_vector_buffer[index] = as_short2(bi_mvs.s0) << 1;
  }
}
