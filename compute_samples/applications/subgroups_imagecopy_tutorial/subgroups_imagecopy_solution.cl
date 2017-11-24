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

__kernel void ImageCopy(write_only image2d_t dstImage,
                        read_only image2d_t srcImage) {
  // This solution uses the cl_intel_subgroups image block
  // built-ins, which have an implicit block width based
  // on the subgroup size.  It also reads and writes four
  // pixels per work item, similar to the original kernel,
  // but because the subgroup block read built-in loads and
  // stores 32-bit quantities, we can process all four pixels
  // at once.

  // Remember that the coordinate specifies the byte offset
  // of the block read for the entire subgroup.  So, the
  // work group byte offset is the same as it was for the
  // kernel using the standard OpenCL built-in functions,
  // but the subgroup byte offset (and hence the block
  // coordinate) is is computed differently.

  int work_group_byte_offset = get_group_id(0) * get_enqueued_local_size(0) * 4;
  int sub_group_byte_offset = work_group_byte_offset +
                              get_sub_group_id() * get_max_sub_group_size() * 4;

  int2 coord = (int2)(sub_group_byte_offset, get_global_id(1));

  uint color = intel_sub_group_block_read(srcImage, coord);

  intel_sub_group_block_write(dstImage, coord, color);
}

#if 0

// It is also possible to use the cl_intel_media_block_io built-
// ins.  Because these built-ins require the block size to be
// compile-time constant, it is convenient to require a specific
// subgroup size when using these built-ins, and in particular
// it is convenient to require a subgroup size of eight, because
// the maximum block width is 32-bytes.  This can be done with 
// the kernel attribute:

__attribute__((intel_reqd_sub_group_size(8)))

// The kernel changes to use the cl_intel_media_block_io built-
// ins are below:

    uint    color = intel_sub_group_media_block_read_ui(
                coord, 
                8,      // block width is 8 uints
                1,      // block height is 1 row
                srcImage );

    intel_sub_group_media_block_write_ui(
        coord,
        8,
        1,
        color,
        dstImage );

#endif

#if 0

// You can get even better performance by processing multiple rows
// of pixels per work item!  This requires a minor change to the
// kernel above.  You will also need to modify the host code to
// modify the global work size.  The kernel changes to process
// multiple rows of pixels per work item are below:

    int2    coord = (int2)(
                sub_group_byte_offset,
                get_global_id(1) * 4 ); // four rows at a time

    uint4   color = intel_sub_group_block_read4( srcImage, coord );

    intel_sub_group_block_write4( dstImage, coord, color );

// The host code changes are specifically for the second
// NDRange component:

    cl::NDRange globalSize( imageWidth / 4, imageHeight / 4 );

// Note that after modifying the host code this kernel cannot
// be used interchangeably with the kernel using standard
// OpenCL image read and write built-in functions, unless it
// is updated to process multiple rows of pixels as well.

#endif
