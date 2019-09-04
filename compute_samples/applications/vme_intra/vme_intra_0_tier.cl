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

uchar8 block_get_intra_edges(int2 mbid, int2 mb_size, ushort2 src_coord,
                             __read_only image2d_t src_luma_img) {
  uchar intra_edge_mask =
      CLK_AVC_ME_INTRA_NEIGHBOR_LEFT_MASK_ENABLE_INTEL |
      CLK_AVC_ME_INTRA_NEIGHBOR_UPPER_MASK_ENABLE_INTEL |
      CLK_AVC_ME_INTRA_NEIGHBOR_UPPER_LEFT_MASK_ENABLE_INTEL |
      CLK_AVC_ME_INTRA_NEIGHBOR_UPPER_RIGHT_MASK_ENABLE_INTEL;

  // If this is a left-edge MB, then disable left edges.
  if (mbid.x == 0) {
    intra_edge_mask &= ~CLK_AVC_ME_INTRA_NEIGHBOR_LEFT_MASK_ENABLE_INTEL;
    intra_edge_mask &= ~CLK_AVC_ME_INTRA_NEIGHBOR_UPPER_LEFT_MASK_ENABLE_INTEL;
  }
  // If this is a right edge MB then disable right edges.
  if (mbid.x == mb_size.x - 1) {
    intra_edge_mask &= ~CLK_AVC_ME_INTRA_NEIGHBOR_UPPER_RIGHT_MASK_ENABLE_INTEL;
  }
  // If this is a top-edge MB, then disable top edges.
  if (mbid.y == 0) {
    intra_edge_mask &= ~CLK_AVC_ME_INTRA_NEIGHBOR_UPPER_LEFT_MASK_ENABLE_INTEL;
    intra_edge_mask &= ~CLK_AVC_ME_INTRA_NEIGHBOR_UPPER_RIGHT_MASK_ENABLE_INTEL;
    intra_edge_mask &= ~CLK_AVC_ME_INTRA_NEIGHBOR_UPPER_MASK_ENABLE_INTEL;
  }

  int2 edge_coord = 0;
  uchar left_luma_edge = 0, left_upper_luma_pixel = 0, upper_luma_edge = 0,
        upper_right_luma_edge = 0;

  // Read left luma edge from luma plane.
  edge_coord.x = src_coord.x - 4;
  edge_coord.y = src_coord.y;
  uint left_luma_edge_dw =
      intel_sub_group_media_block_read_ui(edge_coord, 1, 16, src_luma_img);
  left_luma_edge = as_uchar4(left_luma_edge_dw).s3;

  // Read upper left luma corner from luma plane.
  edge_coord.x = src_coord.x - 4;
  edge_coord.y = src_coord.y - 1;
  uint left_upper_luma_pixel_dw =
      intel_sub_group_media_block_read_ui(edge_coord, 1, 16, src_luma_img);
  left_upper_luma_pixel = as_uchar4(left_upper_luma_pixel_dw).s3;
  left_upper_luma_pixel = intel_sub_group_shuffle(left_upper_luma_pixel, 0);

  // Read upper luma edge from luma plane.
  edge_coord.x = src_coord.x;
  edge_coord.y = src_coord.y - 1;
  upper_luma_edge =
      intel_sub_group_media_block_read_uc(edge_coord, 16, 1, src_luma_img);

  // Read upper right luma edge from luma plane.
  edge_coord.x = src_coord.x + 16;
  edge_coord.y = src_coord.y - 1;
  upper_right_luma_edge =
      intel_sub_group_media_block_read_uc(edge_coord, 16, 1, src_luma_img);

  uchar8 edges = 0;
  edges.s0 = left_luma_edge;
  edges.s1 = left_upper_luma_pixel;
  edges.s2 = upper_luma_edge;
  edges.s3 = upper_right_luma_edge;
  edges.s4 = intra_edge_mask;

  return edges;
}

__kernel void vme_intra(
    __read_only image2d_t src_img, __read_only image2d_t ref_img,
    __read_only image2d_t src_luma_img,
    __global short2 *prediction_motion_vector_buffer,
    __global short2 *motion_vector_buffer, __global uchar2 *shapes_buffer,
    __global ushort *residuals_buffer, __global ushort *best_residual_buffer,
    __global uchar *intra_luma_shape, __global ushort *intra_luma_residuals,
    __global ulong *intra_luma_modes, uchar qp, uchar sad_adjustment,
    uchar pixel_mode, uchar intra_only, int iterations) {
  int2 img_size = get_image_dim(src_img);
  int2 mb_size = (img_size + (int2)(15, 15)) / 16;
  int gid_0 = get_group_id(0);
  int gid_1 = 0;
  sampler_t media_sampler = 0;

  uchar slice_type = CLK_AVC_ME_SLICE_TYPE_PRED_INTEL;

  for (int i = 0; i < iterations; i++, gid_1++) {
    ushort2 src_coord = 0;
    short2 ref_coord = 0;
    short2 predictor_tier1 = 0;

    long mvs = 0;
    ushort dist = 0;
    ushort best_dist = 0;
    uchar major_shape = 0;
    uchar minor_shapes = 0;
    uchar directions = 0;

    src_coord.x = gid_0 * 16;
    src_coord.y = gid_1 * 16;

    int2 mbid = {gid_0, gid_1};
    uchar8 edges =
        block_get_intra_edges(mbid, mb_size, src_coord, src_luma_img);

    //------------------- Perfrom inter estimation -------------------------

    if (!intra_only) {
      uchar search_window_config = SEARCH_WIN_CFG;
      ushort2 frame_size = convert_ushort2(get_image_dim(src_img));
      ushort2 search_size =
          intel_sub_group_ime_ref_window_size(search_window_config, 0);

      // Offset the search window using the input predictor_tier1.
      if (prediction_motion_vector_buffer != NULL) {
        predictor_tier1 =
            prediction_motion_vector_buffer[gid_0 +
                                            gid_1 * get_num_groups(0)] >>
            2;
      }

      uchar partition_mask = CLK_AVC_ME_PARTITION_MASK_ALL_INTEL;
      intel_sub_group_avc_ime_payload_t payload =
          intel_sub_group_avc_ime_initialize(src_coord, partition_mask,
                                             sad_adjustment);
      payload = intel_sub_group_avc_ime_set_single_reference(
          ref_coord, search_window_config, payload);

      uint2 packed_cost_table =
          intel_sub_group_avc_mce_get_default_inter_motion_vector_cost_table(
              slice_type, qp);
      uchar cost_precision = CLK_AVC_ME_COST_PRECISION_QPEL_INTEL;
      ulong shape_penalty =
          intel_sub_group_avc_mce_get_default_inter_shape_penalty(slice_type,
                                                                  qp);
      ulong cost_center = get_cost_center(0);
      payload = intel_sub_group_avc_ime_set_motion_vector_cost_function(
          cost_center, packed_cost_table, cost_precision, payload);
      payload = intel_sub_group_avc_ime_set_inter_shape_penalty(shape_penalty,
                                                                payload);

      // Evaluate IME operation for [0]: (0, 0)
      intel_sub_group_avc_ime_result_single_reference_streamout_t resultsout;
      ref_coord = 0;
      resultsout =
          ime_streamout(src_img, ref_img, src_coord, ref_coord, partition_mask,
                        sad_adjustment, cost_center, cost_precision,
                        packed_cost_table, shape_penalty, media_sampler);

      // Evaluate IME operation for [1]: (predictor_tier1__x,
      // predictor_tier1__y)
      ref_coord = predictor_tier1;
      resultsout = ime_streaminout(src_img, ref_img, src_coord, ref_coord,
                                   partition_mask, sad_adjustment, cost_center,
                                   cost_precision, packed_cost_table,
                                   shape_penalty, resultsout, media_sampler);

      intel_sub_group_avc_ime_result_t result;
      result =
          intel_sub_group_avc_ime_strip_single_reference_streamout(resultsout);

      // Extract results.
      mvs = intel_sub_group_avc_ime_get_motion_vectors(result);
      dist = intel_sub_group_avc_ime_get_inter_distortions(result);
      best_dist = intel_sub_group_avc_ime_get_best_inter_distortion(result);
      major_shape = intel_sub_group_avc_ime_get_inter_major_shape(result);
      minor_shapes = intel_sub_group_avc_ime_get_inter_minor_shapes(result);
      directions = intel_sub_group_avc_ime_get_inter_directions(result);

      // Perform FME for sub-pixel mode.
      if (pixel_mode != CLK_AVC_ME_SUBPIXEL_MODE_INTEGER_INTEL) {
        intel_sub_group_avc_ref_payload_t payload =
            intel_sub_group_avc_fme_initialize(src_coord, mvs, major_shape,
                                               minor_shapes, directions,
                                               pixel_mode, sad_adjustment);
        payload = intel_sub_group_avc_ref_set_motion_vector_cost_function(
            cost_center, packed_cost_table, cost_precision, payload);
        payload = intel_sub_group_avc_ref_set_inter_shape_penalty(shape_penalty,
                                                                  payload);
        intel_sub_group_avc_ref_result_t result;
        result = intel_sub_group_avc_ref_evaluate_with_single_reference(
            src_img, ref_img, media_sampler, payload);

        mvs = intel_sub_group_avc_ref_get_motion_vectors(result);
        dist = intel_sub_group_avc_ref_get_inter_distortions(result);
        best_dist = intel_sub_group_avc_ref_get_best_inter_distortion(result);
      }
    }

    //------------------- Perfrom intra estimation -------------------------

    uchar intra_partition_mask =
        CLK_AVC_ME_INTRA_LUMA_PARTITION_MASK_16x16_INTEL &
        CLK_AVC_ME_INTRA_LUMA_PARTITION_MASK_8x8_INTEL &
        CLK_AVC_ME_INTRA_LUMA_PARTITION_MASK_4x4_INTEL;
    uchar left_luma_edge = edges.s0;
    uchar left_upper_luma_pixel = edges.s1;
    uchar upper_luma_edge = edges.s2;
    uchar upper_right_luma_edge = edges.s3;
    uchar intra_edge_mask = edges.s4;

    intel_sub_group_avc_sic_payload_t sic_payload;
    sic_payload = intel_sub_group_avc_sic_initialize(src_coord);
    sic_payload = intel_sub_group_avc_sic_configure_ipe(
        intra_partition_mask, intra_edge_mask, left_luma_edge,
        left_upper_luma_pixel, upper_luma_edge, upper_right_luma_edge,
        sad_adjustment, sic_payload);

    uint intra_luma_shape_penalty =
        intel_sub_group_avc_mce_get_default_intra_luma_shape_penalty(slice_type,
                                                                     qp);
    sic_payload = intel_sub_group_avc_sic_set_intra_luma_shape_penalty(
        intra_luma_shape_penalty, sic_payload);

    intel_sub_group_avc_sic_result_t ipe_result;
    ipe_result = intel_sub_group_avc_sic_evaluate_ipe(src_img, media_sampler,
                                                      sic_payload);

    uchar intra_shape = intel_sub_group_avc_sic_get_ipe_luma_shape(ipe_result);
    ushort intra_distortions =
        intel_sub_group_avc_sic_get_best_ipe_luma_distortion(ipe_result);
    ulong intra_modes =
        intel_sub_group_avc_sic_get_packed_ipe_luma_modes(ipe_result);

    uchar2 shapes = {major_shape, minor_shapes};

    // Write out results.
    if (!intra_only) {
      int index = (gid_0 * 16 + get_sub_group_local_id()) +
                  (gid_1 * 16 * get_num_groups(0));
      int2 bi_mvs = as_int2(mvs);
      motion_vector_buffer[index] = as_short2(bi_mvs.s0);
      residuals_buffer[index] = dist;
      best_residual_buffer[gid_0 + gid_1 * get_num_groups(0)] = best_dist;
      shapes_buffer[gid_0 + gid_1 * get_num_groups(0)] = shapes;
    } else {
      int index = (gid_0 * 16 + get_sub_group_local_id()) +
                  (gid_1 * 16 * get_num_groups(0));
      residuals_buffer[index] = 0xFFFF;
      best_residual_buffer[gid_0 + gid_1 * get_num_groups(0)] = 0xFFFF;
    }

    intra_luma_shape[gid_0 + gid_1 * get_num_groups(0)] = intra_shape;
    intra_luma_residuals[gid_0 + gid_1 * get_num_groups(0)] = intra_distortions;
    intra_luma_modes[gid_0 + gid_1 * get_num_groups(0)] = intra_modes;
  }
}
