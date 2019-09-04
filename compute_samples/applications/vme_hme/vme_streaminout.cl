/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifdef SEARCH_WIN_CFG
#undef SEARCH_WIN_CFG
#endif
#define SEARCH_WIN_CFG CLK_AVC_ME_SEARCH_WINDOW_EXHAUSTIVE_INTEL

inline intel_sub_group_avc_ime_result_single_reference_streamout_t
ime_streamout(__read_only image2d_t src_img, __read_only image2d_t ref_img,
              ushort2 src_coord, short2 ref_coord, uint partition_mask,
              uchar sad_adjustment, ulong cost_center, uchar cost_precision,
              uint2 packed_cost_table, ulong shape_penalty,
              sampler_t media_sampler) {
  // Ensure that the reference search window is atleast partially within the
  // reference frame.
  ushort2 frame_size = convert_ushort2(get_image_dim(src_img));
  ushort2 search_size = intel_sub_group_ime_ref_window_size(SEARCH_WIN_CFG, 0);
  ref_coord = intel_sub_group_avc_ime_adjust_ref_offset(
      ref_coord, src_coord, search_size, frame_size);

  intel_sub_group_avc_ime_payload_t payload;
  payload = intel_sub_group_avc_ime_initialize(src_coord, partition_mask,
                                               sad_adjustment);
  payload = intel_sub_group_avc_ime_set_single_reference(
      ref_coord, SEARCH_WIN_CFG, payload);
  payload = intel_sub_group_avc_ime_set_motion_vector_cost_function(
      cost_center, packed_cost_table, cost_precision, payload);
  payload =
      intel_sub_group_avc_ime_set_inter_shape_penalty(shape_penalty, payload);

  intel_sub_group_avc_ime_result_single_reference_streamout_t resultsout;
  resultsout = intel_sub_group_avc_ime_evaluate_with_single_reference_streamout(
      src_img, ref_img, media_sampler, payload);

  return resultsout;
}

inline intel_sub_group_avc_ime_result_single_reference_streamout_t
ime_streaminout(
    __read_only image2d_t src_img, __read_only image2d_t ref_img,
    ushort2 src_coord, short2 ref_coord, uint partition_mask,
    uchar sad_adjustment, ulong cost_center, uchar cost_precision,
    uint2 packed_cost_table, ulong shape_penalty,
    intel_sub_group_avc_ime_result_single_reference_streamout_t resultsout,
    sampler_t media_sampler)

{
  // Ensure that the reference search window is atleast partially within the
  // reference frame.
  ushort2 frame_size = convert_ushort2(get_image_dim(src_img));
  ushort2 search_size = intel_sub_group_ime_ref_window_size(SEARCH_WIN_CFG, 0);
  ref_coord = intel_sub_group_avc_ime_adjust_ref_offset(
      ref_coord, src_coord, search_size, frame_size);

  intel_sub_group_avc_ime_single_reference_streamin_t resultsin;
  resultsin = intel_sub_group_avc_ime_get_single_reference_streamin(resultsout);

  intel_sub_group_avc_ime_payload_t payload;
  payload = intel_sub_group_avc_ime_initialize(src_coord, partition_mask,
                                               sad_adjustment);
  payload = intel_sub_group_avc_ime_set_single_reference(
      ref_coord, SEARCH_WIN_CFG, payload);
  payload = intel_sub_group_avc_ime_set_motion_vector_cost_function(
      cost_center, packed_cost_table, cost_precision, payload);
  payload =
      intel_sub_group_avc_ime_set_inter_shape_penalty(shape_penalty, payload);

  resultsout =
      intel_sub_group_avc_ime_evaluate_with_single_reference_streaminout(
          src_img, ref_img, media_sampler, payload, resultsin);

  return resultsout;
}
