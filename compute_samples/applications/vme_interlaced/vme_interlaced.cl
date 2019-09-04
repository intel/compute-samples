/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

__kernel void vme_interlaced(__read_only image2d_t srcImg,
                             __read_only image2d_t refImg, uchar interlaced,
                             uchar polarity,
                             __global short2 *prediction_motion_vector_buffer,
                             __global short2 *motion_vector_buffer,
                             __global ushort *residuals_buffer,
                             __global uchar2 *shapes_buffer,
                             unsigned iterations) {
  unsigned gid_0 = get_group_id(0);
  unsigned gid_1 = 0;
  sampler_t vme_sampler = CLK_AVC_ME_INITIALIZE_INTEL;

  for (unsigned i = 0; i < iterations; i++, gid_1++) {
    ushort2 srcCoord = 0;
    short2 refCoord = 0;
    short2 predMV = 0;

    srcCoord.x = gid_0 * 16;
    srcCoord.y = gid_1 * 16;

    if (prediction_motion_vector_buffer != NULL) {
      predMV =
          prediction_motion_vector_buffer[gid_0 + gid_1 * get_num_groups(0)];
      refCoord.x = predMV.x / 4;
      refCoord.y = predMV.y / 4;
      refCoord.y = refCoord.y & 0xFFFE;
    }

    uchar partition_mask = CLK_AVC_ME_PARTITION_MASK_ALL_INTEL;
    uchar sad_adjustment = CLK_AVC_ME_SAD_ADJUST_MODE_NONE_INTEL;
    uchar pixel_mode = CLK_AVC_ME_SUBPIXEL_MODE_QPEL_INTEL;

    intel_sub_group_avc_ime_payload_t payload =
        intel_sub_group_avc_ime_initialize(srcCoord, partition_mask,
                                           sad_adjustment);
    payload = intel_sub_group_avc_ime_set_single_reference(
        refCoord, CLK_AVC_ME_SEARCH_WINDOW_EXHAUSTIVE_INTEL, payload);
    ulong cost_center = 0;
    uint2 packed_cost_table =
        intel_sub_group_avc_mce_get_default_high_penalty_cost_table();
    uchar search_cost_precision = CLK_AVC_ME_COST_PRECISION_QPEL_INTEL;
    payload = intel_sub_group_avc_ime_set_motion_vector_cost_function(
        cost_center, packed_cost_table, search_cost_precision, payload);
    if (interlaced) {
      payload = intel_sub_group_avc_ime_set_source_interlaced_field_polarity(
          polarity, payload);
      payload =
          intel_sub_group_avc_ime_set_single_reference_interlaced_field_polarity(
              polarity, payload);
    }
    intel_sub_group_avc_ime_result_t result =
        intel_sub_group_avc_ime_evaluate_with_single_reference(
            srcImg, refImg, vme_sampler, payload);

    // Process Results
    long mvs = intel_sub_group_avc_ime_get_motion_vectors(result);
    ushort dists = intel_sub_group_avc_ime_get_inter_distortions(result);
    uchar major_shape = intel_sub_group_avc_ime_get_inter_major_shape(result);
    uchar minor_shapes = intel_sub_group_avc_ime_get_inter_minor_shapes(result);
    uchar2 shapes = {major_shape, minor_shapes};
    uchar directions = intel_sub_group_avc_ime_get_inter_directions(result);

    // Perform FME for non-integer p.ixel mode
    if (pixel_mode != CLK_AVC_ME_SUBPIXEL_MODE_INTEGER_INTEL) {
      intel_sub_group_avc_ref_payload_t payload =
          intel_sub_group_avc_fme_initialize(srcCoord, mvs, major_shape,
                                             minor_shapes, directions,
                                             pixel_mode, sad_adjustment);
      payload = intel_sub_group_avc_ref_set_motion_vector_cost_function(
          cost_center, packed_cost_table, search_cost_precision, payload);
      if (interlaced) {
        payload = intel_sub_group_avc_ref_set_source_interlaced_field_polarity(
            polarity, payload);
        payload =
            intel_sub_group_avc_ref_set_single_reference_interlaced_field_polarity(
                polarity, payload);
      }
      intel_sub_group_avc_ref_result_t result =
          intel_sub_group_avc_ref_evaluate_with_single_reference(
              srcImg, refImg, vme_sampler, payload);
      mvs = intel_sub_group_avc_ref_get_motion_vectors(result);
      dists = intel_sub_group_avc_ref_get_inter_distortions(result);
    }

    // Write out results
    int index =
        (gid_0 * 16 + get_local_id(0)) + (gid_1 * 16 * get_num_groups(0));
    int2 bi_mvs = as_int2(mvs);
    motion_vector_buffer[index] = as_short2(bi_mvs.s0);
    if (residuals_buffer != NULL) {
      residuals_buffer[index] = dists;
    }
    shapes_buffer[gid_0 + gid_1 * get_num_groups(0)] = shapes;
  }
}
