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

inline void vme_wpp_process(int2 mbid, int2 mbsize,
                            __read_only image2d_t src_img,
                            __read_only image2d_t ref_img,
                            __global short2 *prediction_motion_vector_buffer,
                            __global short2 *motion_vector_buffer,
                            __global ushort *residuals_buffer,
                            __global uchar2 *shapes_buffer, uchar qp,
                            uchar sad_adjustment, uchar sub_pixel_mode) {
  sampler_t media_sampler = CLK_AVC_ME_INITIALIZE_INTEL;
  uchar partition_mask = CLK_AVC_ME_PARTITION_MASK_ALL_INTEL;

  uchar slice_type = CLK_AVC_ME_SLICE_TYPE_PRED_INTEL;
  uint2 packed_cost_table =
      intel_sub_group_avc_mce_get_default_inter_motion_vector_cost_table(
          slice_type, qp);
  ulong shape_penalty =
      intel_sub_group_avc_mce_get_default_inter_shape_penalty(slice_type, qp);
  uchar ref_penalty =
      intel_sub_group_avc_mce_get_default_inter_base_multi_reference_penalty(
          slice_type, qp);
  uchar cost_precision = CLK_AVC_ME_COST_PRECISION_QPEL_INTEL;
  ulong cost_center = get_cost_center(0);

  ushort2 src_coord;
  src_coord.x = mbid.x * 16;
  src_coord.y = mbid.y * 16;

  // Set up predictors - zero, tier1 mv, left mv, top-left mv, and top mv.

  bool ref_coord_valid[5] = {true, false, false, false, false};
  short2 pred_zero = 0;
  short2 ref_coord[5];
  ref_coord[0] = pred_zero;
  ref_coord[1] =
      prediction_motion_vector_buffer[mbid.x + mbid.y * mbsize.x] >> 2;
  ref_coord_valid[1] = check_ref_coord_valid(ref_coord_valid, 1);
  if (mbid.x == 0 && mbid.y == 0) {
    // Do nothing.
  } else if (mbid.x == 0) {
    ref_coord[2] =
        motion_vector_buffer[mbid.x * 16 + (mbid.y - 1) * 16 * mbsize.x] >> 2;
    ref_coord_valid[2] = check_ref_coord_valid(ref_coord_valid, 2);
  } else if (mbid.y == 0) {
    ref_coord[2] =
        motion_vector_buffer[(mbid.x - 1) * 16 + mbid.y * 16 * mbsize.x] >> 2;
    ref_coord_valid[2] = check_ref_coord_valid(ref_coord_valid, 2);
  } else {
    ref_coord[2] = motion_vector_buffer[(mbid.x - 1) * 16 +
                                        (mbid.y - 1) * 16 * mbsize.x] >>
                   2;
    ref_coord_valid[2] = check_ref_coord_valid(ref_coord_valid, 2);
    ref_coord[3] =
        motion_vector_buffer[mbid.x * 16 + (mbid.y - 1) * 16 * mbsize.x] >> 2;
    ref_coord_valid[3] = check_ref_coord_valid(ref_coord_valid, 3);
    ref_coord[4] =
        motion_vector_buffer[(mbid.x - 1) * 16 + mbid.y * 16 * mbsize.x] >> 2;
    ref_coord_valid[4] = check_ref_coord_valid(ref_coord_valid, 4);
  }

  intel_sub_group_avc_ime_result_single_reference_streamout_t resultsout;
  resultsout =
      ime_streamout(src_img, ref_img, src_coord, ref_coord[0], partition_mask,
                    sad_adjustment, cost_center, cost_precision,
                    packed_cost_table, shape_penalty, media_sampler);

  for (int i = 0; i < 5; i++) {
    if (ref_coord_valid[i]) {
      resultsout = ime_streaminout(src_img, ref_img, src_coord, ref_coord[i],
                                   partition_mask, sad_adjustment, cost_center,
                                   cost_precision, packed_cost_table,
                                   shape_penalty, resultsout, media_sampler);
    }
  }

  intel_sub_group_avc_ime_result_t result;
  result = intel_sub_group_avc_ime_strip_single_reference_streamout(resultsout);

  // Extract results.
  long mvs = intel_sub_group_avc_ime_get_motion_vectors(result);
  ushort sads = intel_sub_group_avc_ime_get_inter_distortions(result);
  uchar major_shape = intel_sub_group_avc_ime_get_inter_major_shape(result);
  uchar minor_shapes = intel_sub_group_avc_ime_get_inter_minor_shapes(result);
  uchar2 shapes = {major_shape, minor_shapes};
  uchar directions = intel_sub_group_avc_ime_get_inter_directions(result);

  // Perform FME for sub-pixel mode.
  if (sub_pixel_mode != CLK_AVC_ME_SUBPIXEL_MODE_INTEGER_INTEL) {
    intel_sub_group_avc_ref_payload_t payload =
        intel_sub_group_avc_fme_initialize(src_coord, mvs, major_shape,
                                           minor_shapes, directions,
                                           sub_pixel_mode, sad_adjustment);
    payload = intel_sub_group_avc_ref_set_motion_vector_cost_function(
        cost_center, packed_cost_table, cost_precision, payload);
    payload =
        intel_sub_group_avc_ref_set_inter_shape_penalty(shape_penalty, payload);
    intel_sub_group_avc_ref_result_t result;
    result = intel_sub_group_avc_ref_evaluate_with_single_reference(
        src_img, ref_img, media_sampler, payload);

    mvs = intel_sub_group_avc_ref_get_motion_vectors(result);
    sads = intel_sub_group_avc_ref_get_inter_distortions(result);
  }

  // Write out result.
  int index =
      (mbid.x * 16 + get_sub_group_local_id()) + (mbid.y * 16 * mbsize.x);
  int2 bi_mvs = as_int2(mvs);
  motion_vector_buffer[index] = as_short2(bi_mvs.s0);
  residuals_buffer[index] = sads;
  shapes_buffer[mbid.x + mbid.y * mbsize.x] = shapes;
}

ushort block_skip_check(int2 mbid, int2 mb_size, ushort2 src_coord,
                        long mvs_skc, __read_only image2d_t src_img,
                        __read_only image2d_t ref_img) {
  uint skip_block_partition_type = CLK_AVC_ME_SKIP_BLOCK_PARTITION_16x16_INTEL;
  uint skip_motion_vector_mask =
      CLK_AVC_ME_SKIP_BLOCK_16x16_FORWARD_ENABLE_INTEL;
  uint bidirectional_weight = CLK_AVC_ME_BIDIR_WEIGHT_HALF_INTEL;
  uchar sad_adjustment = CLK_AVC_ME_SAD_ADJUST_MODE_NONE_INTEL;
  uchar slice_type = CLK_AVC_ME_SLICE_TYPE_PRED_INTEL;

  intel_sub_group_avc_sic_payload_t check_payload =
      intel_sub_group_avc_sic_initialize(src_coord);
  check_payload = intel_sub_group_avc_sic_configure_skc(
      skip_block_partition_type, skip_motion_vector_mask, mvs_skc,
      bidirectional_weight, sad_adjustment, check_payload);
  sampler_t accelerator = CLK_AVC_ME_INITIALIZE_INTEL;
  intel_sub_group_avc_sic_result_t check_result;
  check_result = intel_sub_group_avc_sic_evaluate_with_single_reference(
      src_img, ref_img, accelerator, check_payload);
  ushort checked_dists =
      intel_sub_group_avc_sic_get_inter_distortions(check_result);

  return intel_sub_group_shuffle(checked_dists, 0);
}

__kernel void vme_wpp(__read_only image2d_t src_img,
                      __read_only image2d_t ref_img,
                      __global short2 *prediction_motion_vector_buffer,
                      __global short2 *motion_vector_buffer,
                      __global ushort *residuals_buffer,
                      __global uchar2 *shapes_buffer,
                      __global atomic_int *scoreboard, uchar qp,
                      uchar sad_adjustment, uchar sub_pixel_mode) {
  int2 mbid = {get_group_id(0), 0};
  int2 img_size = get_image_dim(src_img);
  int2 mb_size = (img_size + (int2)(15, 15)) / 16;

  // Process blocks cyclically.
  do {
    ushort2 src_coord;
    src_coord.x = mbid.x * 16;
    src_coord.y = mbid.y * 16;
    bool skip_block = false;

    if (mbid.x > 0) {
      poll_scoreboard(scoreboard + mbid.x - 1, mbid.y + 1, mbid);
    }

    // Check SKIP mode for (0,0), (LEFT_PRED), (TOP_PRED)

    ulong mv_skip = 0;
    ushort skip_dist =
        block_skip_check(mbid, mb_size, src_coord, mv_skip, src_img, ref_img);
    enum { ZERO, LEFT, TOP } mode = ZERO;

    if (mbid.x == 0 && mbid.y == 0) {
      // Do nothing.
    } else if (mbid.x == 0) {
      short2 mv_skip_short =
          motion_vector_buffer[mbid.x * 16 + (mbid.y - 1) * 16 * mb_size.x];
      int2 mv_skip_uint = {as_uint(mv_skip_short), 0};
      ushort skip_dist_neighbor = block_skip_check(
          mbid, mb_size, src_coord, as_ulong(mv_skip_uint), src_img, ref_img);
      if (skip_dist_neighbor < skip_dist) {
        skip_dist = skip_dist_neighbor;
        mode = TOP;
      }
    } else if (mbid.y == 0) {
      short2 mv_skip_short =
          motion_vector_buffer[(mbid.x - 1) * 16 + mbid.y * 16 * mb_size.x];
      int2 mv_skip_uint = 0;
      mv_skip_uint.x = as_uint(mv_skip_short);
      ushort skip_dist_neighbor = block_skip_check(
          mbid, mb_size, src_coord, as_ulong(mv_skip_uint), src_img, ref_img);
      if (skip_dist_neighbor < skip_dist) {
        skip_dist = skip_dist_neighbor;
        mode = LEFT;
      }
    } else {
      short2 mv_skip_short =
          motion_vector_buffer[mbid.x * 16 + (mbid.y - 1) * 16 * mb_size.x];
      int2 mv_skip_uint = 0;
      mv_skip_uint.x = as_uint(mv_skip_short);
      ushort skip_dist_neighbor = block_skip_check(
          mbid, mb_size, src_coord, as_ulong(mv_skip_uint), src_img, ref_img);
      if (skip_dist_neighbor < skip_dist) {
        skip_dist = skip_dist_neighbor;
        mode = TOP;
      }
      mv_skip_short =
          motion_vector_buffer[(mbid.x - 1) * 16 + mbid.y * 16 * mb_size.x];
      mv_skip_uint = 0;
      mv_skip_uint.x = as_uint(mv_skip_short);
      skip_dist_neighbor = block_skip_check(
          mbid, mb_size, src_coord, as_ulong(mv_skip_uint), src_img, ref_img);
      if (skip_dist_neighbor < skip_dist) {
        skip_dist = skip_dist_neighbor;
        mode = LEFT;
      }
    }

#define SKIP_DISTORTION_THRESHOLD (30)

    if (skip_dist < SKIP_DISTORTION_THRESHOLD) {
      skip_block = true;
      int index =
          (mbid.x * 16 + get_sub_group_local_id()) + (mbid.y * 16 * mb_size.x);
      residuals_buffer[index] = skip_dist;
      uchar2 shapes = {CLK_AVC_ME_MAJOR_16x16_INTEL, 0};
      shapes_buffer[mbid.x + mbid.y * mb_size.x] = shapes;
      motion_vector_buffer[mbid.x * 16 + mbid.y * 16 * mb_size.x] = 0;
    } else {
      vme_wpp_process(mbid, mb_size, src_img, ref_img,
                      prediction_motion_vector_buffer, motion_vector_buffer,
                      residuals_buffer, shapes_buffer, qp, sad_adjustment,
                      sub_pixel_mode);
    }

    signal_scoreboard(scoreboard + mbid.x);

    mbid.y += 1;

    if (mbid.y == mb_size.y) {
      mbid.y = 0;
      mbid.x += get_num_groups(0);
    }
  } while (mbid.x < mb_size.x);
}
