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

inline intel_sub_group_avc_ime_result_single_reference_streamout_t
ime_streamout(__read_only image2d_t src_img, __read_only image2d_t ref_img,
              ushort2 src_coord, short2 ref_coord, uint partition_mask,
              uchar sad_adjustment, ulong cost_center, uchar cost_precision,
              uint2 packed_cost_table, ulong shape_penalty,
              sampler_t media_sampler)

{
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

__kernel void
vme_larger_search(__read_only image2d_t src_img, __read_only image2d_t ref_img,
                  __global short2 *prediction_motion_vector_buffer,
                  __global short2 *motion_vector_buffer,
                  __global ushort *residuals_buffer,
                  __global uchar2 *shapes_buffer, uchar qp,
                  uchar sad_adjustment, uchar pixel_mode, uint iterations) {
  int gid_0 = get_group_id(0);
  int gid_1 = 0;
  sampler_t media_sampler = 0;

  for (uint i = 0; i < iterations; i++, gid_1++) {
    ushort2 src_coord = 0;
    short2 ref_coord = 0;
    short2 predictor = 0;

    src_coord.x = gid_0 * 16;
    src_coord.y = gid_1 * 16;

    uchar search_window_config = SEARCH_WIN_CFG;
    ushort2 frame_size = convert_ushort2(get_image_dim(src_img));
    ushort2 search_size =
        intel_sub_group_ime_ref_window_size(search_window_config, 0);

    // Offset the search window using the input predictor.
    if (prediction_motion_vector_buffer != NULL) {
      predictor =
          prediction_motion_vector_buffer[gid_0 + gid_1 * get_num_groups(0)] >>
          2;
      ref_coord = predictor;
    }

    // Initialize IME payload.
    uchar partition_mask = CLK_AVC_ME_PARTITION_MASK_16x16_INTEL &
                           CLK_AVC_ME_PARTITION_MASK_16x8_INTEL &
                           CLK_AVC_ME_PARTITION_MASK_8x16_INTEL &
                           CLK_AVC_ME_PARTITION_MASK_8x8_INTEL &
                           CLK_AVC_ME_PARTITION_MASK_4x4_INTEL;
    intel_sub_group_avc_ime_payload_t payload =
        intel_sub_group_avc_ime_initialize(src_coord, partition_mask,
                                           sad_adjustment);

    // Reference window configuration.
    payload = intel_sub_group_avc_ime_set_single_reference(
        ref_coord, search_window_config, payload);

    // Configure cost heuristics.
    uchar slice_type = CLK_AVC_ME_SLICE_TYPE_PRED_INTEL;
    uint2 packed_cost_table =
        intel_sub_group_avc_mce_get_default_inter_motion_vector_cost_table(
            slice_type, qp);
    uchar cost_precision = CLK_AVC_ME_COST_PRECISION_QPEL_INTEL;
    ulong shape_penalty =
        intel_sub_group_avc_mce_get_default_inter_shape_penalty(slice_type, qp);
    ulong cost_center = get_cost_center(0);

    payload = intel_sub_group_avc_ime_set_motion_vector_cost_function(
        cost_center, packed_cost_table, cost_precision, payload);
    payload =
        intel_sub_group_avc_ime_set_inter_shape_penalty(shape_penalty, payload);

    // Evaluate IME operation for [0]: (predictor_x, predictor_y)
    intel_sub_group_avc_ime_result_single_reference_streamout_t resultsout;
    resultsout =
        ime_streamout(src_img, ref_img, src_coord, ref_coord, partition_mask,
                      sad_adjustment, cost_center, cost_precision,
                      packed_cost_table, shape_penalty, media_sampler);

    // Evaluate IME operation for [1]: (predictor_x, predictor_y-40+16)
    ref_coord.x = predictor.x;
    ref_coord.y = predictor.y - search_size.y + 16;
    resultsout = ime_streaminout(src_img, ref_img, src_coord, ref_coord,
                                 partition_mask, sad_adjustment, cost_center,
                                 cost_precision, packed_cost_table,
                                 shape_penalty, resultsout, media_sampler);

    // Evaluate IME operation for [2]: (predictor_x, predictor_y+40-16)
    ref_coord.x = predictor.x;
    ref_coord.y = predictor.y + search_size.y - 16;
    resultsout = ime_streaminout(src_img, ref_img, src_coord, ref_coord,
                                 partition_mask, sad_adjustment, cost_center,
                                 cost_precision, packed_cost_table,
                                 shape_penalty, resultsout, media_sampler);

    // Evaluate IME operation for [3]: (predictor_x-48+16, predictor_y)
    ref_coord.x = predictor.x - search_size.x + 16;
    ref_coord.y = predictor.y;
    resultsout = ime_streaminout(src_img, ref_img, src_coord, ref_coord,
                                 partition_mask, sad_adjustment, cost_center,
                                 cost_precision, packed_cost_table,
                                 shape_penalty, resultsout, media_sampler);

    // Evaluate IME operation for [4]: (predictor_x+48-16, predictor_y)
    ref_coord.x = predictor.x + search_size.x - 16;
    ref_coord.y = predictor.y;
    resultsout = ime_streaminout(src_img, ref_img, src_coord, ref_coord,
                                 partition_mask, sad_adjustment, cost_center,
                                 cost_precision, packed_cost_table,
                                 shape_penalty, resultsout, media_sampler);

    // Evaluate IME operation for [5]: (predictor_x-48+16, predictor_y-40+16)
    ref_coord.x = predictor.x - search_size.x + 16;
    ref_coord.y = predictor.y - search_size.y + 16;
    resultsout = ime_streaminout(src_img, ref_img, src_coord, ref_coord,
                                 partition_mask, sad_adjustment, cost_center,
                                 cost_precision, packed_cost_table,
                                 shape_penalty, resultsout, media_sampler);

    // Evaluate IME operation for [6]: (predictor_x-48+16, predictor_y+40-16)
    ref_coord.x = predictor.x - search_size.x + 16;
    ref_coord.y = predictor.y + search_size.y - 16;
    resultsout = ime_streaminout(src_img, ref_img, src_coord, ref_coord,
                                 partition_mask, sad_adjustment, cost_center,
                                 cost_precision, packed_cost_table,
                                 shape_penalty, resultsout, media_sampler);

    // Evaluate IME operation for [7]: (predictor_x+48-16, predictor_y-40+16)
    ref_coord.x = predictor.x + search_size.x - 16;
    ref_coord.y = predictor.y - search_size.y + 16;
    resultsout = ime_streaminout(src_img, ref_img, src_coord, ref_coord,
                                 partition_mask, sad_adjustment, cost_center,
                                 cost_precision, packed_cost_table,
                                 shape_penalty, resultsout, media_sampler);

    // Evaluate IME operation for [8]: (predictor_x+48-16, predictor_y+40-16)
    ref_coord.x = predictor.x + search_size.x - 16;
    ref_coord.y = predictor.y + search_size.y - 16;
    resultsout = ime_streaminout(src_img, ref_img, src_coord, ref_coord,
                                 partition_mask, sad_adjustment, cost_center,
                                 cost_precision, packed_cost_table,
                                 shape_penalty, resultsout, media_sampler);

    intel_sub_group_avc_ime_result_t result;
    result =
        intel_sub_group_avc_ime_strip_single_reference_streamout(resultsout);

    // Extract results.
    long mvs = intel_sub_group_avc_ime_get_motion_vectors(result);
    ushort sads = intel_sub_group_avc_ime_get_inter_distortions(result);
    uchar major_shape = intel_sub_group_avc_ime_get_inter_major_shape(result);
    uchar minor_shapes = intel_sub_group_avc_ime_get_inter_minor_shapes(result);
    uchar2 shapes = {major_shape, minor_shapes};
    uchar directions = intel_sub_group_avc_ime_get_inter_directions(result);

    // Perform FME for sub-pixel mode.
    if (pixel_mode != CLK_AVC_ME_SUBPIXEL_MODE_INTEGER_INTEL) {
      // Initialize FME payload.
      intel_sub_group_avc_ref_payload_t payload =
          intel_sub_group_avc_fme_initialize(src_coord, mvs, major_shape,
                                             minor_shapes, directions,
                                             pixel_mode, sad_adjustment);

      // Configure cost heuristics.
      payload = intel_sub_group_avc_ref_set_motion_vector_cost_function(
          cost_center, packed_cost_table, cost_precision, payload);
      payload = intel_sub_group_avc_ref_set_inter_shape_penalty(shape_penalty,
                                                                payload);

      // Evaluate FME operation.
      intel_sub_group_avc_ref_result_t result =
          intel_sub_group_avc_ref_evaluate_with_single_reference(
              src_img, ref_img, media_sampler, payload);

      // Extract results.
      mvs = intel_sub_group_avc_ref_get_motion_vectors(result);
      sads = intel_sub_group_avc_ref_get_inter_distortions(result);
    }

    // Write out result.
    int index = (gid_0 * 16 + get_sub_group_local_id()) +
                (gid_1 * 16 * get_num_groups(0));
    int2 bi_mvs = as_int2(mvs);
    motion_vector_buffer[index] = as_short2(bi_mvs.s0);
    residuals_buffer[index] = sads;
    shapes_buffer[gid_0 + gid_1 * get_num_groups(0)] = shapes;
  }
}
