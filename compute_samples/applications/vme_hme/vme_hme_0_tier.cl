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

__kernel void vme_hme(__read_only image2d_t src_img,
                      __read_only image2d_t ref_img,
                      __global short2 *prediction_motion_vector_buffer,
                      __global short2 *motion_vector_buffer,
                      __global ushort *residuals_buffer,
                      __global uchar2 *shapes_buffer, uchar qp,
                      uchar sad_adjustment, uchar pixel_mode, int iterations) {
  int gid_0 = get_group_id(0);
  int gid_1 = 0;
  sampler_t media_sampler = 0;

  for (int i = 0; i < iterations; i++, gid_1++) {
    ushort2 src_coord = 0;
    short2 ref_coord = 0;
    short2 predictor_tier1 = 0;

    src_coord.x = gid_0 * 16;
    src_coord.y = gid_1 * 16;

    uchar search_window_config = SEARCH_WIN_CFG;
    ushort2 frame_size = convert_ushort2(get_image_dim(src_img));
    ushort2 search_size =
        intel_sub_group_ime_ref_window_size(search_window_config, 0);

    // Offset the search window using the input predictor_tier1.
    if (prediction_motion_vector_buffer != NULL) {
      predictor_tier1 =
          prediction_motion_vector_buffer[gid_0 + gid_1 * get_num_groups(0)] >>
          2;
    }

    uchar partition_mask = CLK_AVC_ME_PARTITION_MASK_ALL_INTEL;

    uchar slice_type = CLK_AVC_ME_SLICE_TYPE_PRED_INTEL;
    uint2 packed_cost_table =
        intel_sub_group_avc_mce_get_default_inter_motion_vector_cost_table(
            slice_type, qp);
    uchar cost_precision = CLK_AVC_ME_COST_PRECISION_QPEL_INTEL;
    ulong shape_penalty =
        intel_sub_group_avc_mce_get_default_inter_shape_penalty(slice_type, qp);
    ulong cost_center = get_cost_center(0);

    // Evaluate IME operation for [0]: (0, 0)
    intel_sub_group_avc_ime_result_single_reference_streamout_t resultsout;
    ref_coord = 0;
    resultsout =
        ime_streamout(src_img, ref_img, src_coord, ref_coord, partition_mask,
                      sad_adjustment, cost_center, cost_precision,
                      packed_cost_table, shape_penalty, media_sampler);

    // Evaluate IME operation for [1]: (predictor_tier1__x, predictor_tier1__y)
    ref_coord = predictor_tier1;
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
