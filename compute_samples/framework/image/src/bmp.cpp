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

#include "bmp.hpp"

#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

namespace compute_samples {

#pragma pack(push, 1)

struct BMPFileHeader {
  uint16_t bf_type_; // 'BM' for BitMap
  uint32_t bf_size_; // file size in bytes
  uint16_t bf_reserved1_;
  uint16_t bf_reserved2_;
  uint32_t bf_off_bits_; // offset of bitmap in file
};

struct BMPInfoHeader {
  uint32_t bi_size_;      // length of info header
  int32_t bi_width_;      // width of bitmap in pixels
  int32_t bi_height_;     // height of bitmap in pixels
  uint16_t bi_planes_;    // number of color planes - must be 1
  uint16_t bi_bit_count_; // bit depth
  uint32_t bi_compression_;
  uint32_t bi_size_image_; // size of picture in bytes
  int32_t bi_x_pels_per_meter_;
  int32_t bi_y_pels_per_meter_;
  uint32_t bi_clr_used_;
  uint32_t bi_clr_important_;
};

// sturcture for reading in palettes of 8bpp BMP files
struct BMPRGBQUAD_t {
  uint8_t b_;
  uint8_t g_;
  uint8_t r_;
  uint8_t reserved_;
};

#pragma pack(pop)

const uint32_t BI_RGB = 0;

bool BmpUtils::save_image_as_bmp(uint32_t *ptr, size_t width, size_t height,
                                 const char *file_name) {
  FILE *stream = NULL;
  uint32_t *ppix = ptr;
  stream = fopen(file_name, "wb");

  if (NULL == stream)
    return false;

  BMPFileHeader file_header;
  BMPInfoHeader info_header;

  int align_size = width * 4;
  align_size ^= 0x03;
  align_size++;
  align_size &= 0x03;

  int rowLength = width * 4 + align_size;

  file_header.bf_type_ = 0x4D42; // 'BM'
  file_header.bf_size_ =
      sizeof(file_header) + sizeof(info_header) + rowLength * height;
  file_header.bf_off_bits_ = sizeof(file_header) + sizeof(info_header);
  file_header.bf_reserved1_ = 0;
  file_header.bf_reserved2_ = 0;

  info_header.bi_size_ = sizeof(BMPInfoHeader);
  info_header.bi_width_ = width;
  info_header.bi_height_ = height;
  info_header.bi_planes_ = 1;
  info_header.bi_bit_count_ = 32;
  info_header.bi_compression_ = BI_RGB;
  info_header.bi_size_image_ = rowLength * height;
  info_header.bi_x_pels_per_meter_ = 0;
  info_header.bi_y_pels_per_meter_ = 0;
  info_header.bi_clr_used_ = 0;
  info_header.bi_clr_important_ = 0;

  if (sizeof(file_header) !=
      fwrite(&file_header, 1, sizeof(file_header), stream)) {
    goto error_exit;
  }

  if (sizeof(info_header) !=
      fwrite(&info_header, 1, sizeof(info_header), stream)) {
    goto error_exit;
  }

  unsigned char buffer[4];
  memset(buffer, 0, sizeof(buffer));
  for (unsigned y = 0; y < height; y++) {
    for (unsigned x = 0; x < width; x++, ppix++) {
      if (4 != fwrite(ppix, 1, 4, stream)) {
        goto error_exit;
      }
    }
    fwrite(buffer, 1, align_size, stream);
  }

  fclose(stream);
  return true;

error_exit:
  fclose(stream);
  return false;
}

bool BmpUtils::save_image_as_bmp_32fc4(float *ptr, float scale, size_t width,
                                       size_t height, const char *file_name) {
  // save results in bitmap files
  float tmp_f_val = 0.0f;
  uint32_t *out_u_int_buf =
      (uint32_t *)malloc(width * height * sizeof(uint32_t));
  if (!out_u_int_buf) {
    printf("Failed to allocate memory for output BMP image!\n");
    return false;
  }

  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      // Ensure that no value is greater than 255.0
      uint32_t ui_tmp[4];
      tmp_f_val = (scale * ptr[(y * width + x) * 4 + 0]);
      if (tmp_f_val > 255.0f)
        tmp_f_val = 255.0f;
      ui_tmp[0] = (uint32_t)(tmp_f_val);

      tmp_f_val = (scale * ptr[(y * width + x) * 4 + 1]);
      if (tmp_f_val > 255.0f)
        tmp_f_val = 255.0f;
      ui_tmp[1] = (uint32_t)(tmp_f_val);

      tmp_f_val = (scale * ptr[(y * width + x) * 4 + 2]);
      if (tmp_f_val > 255.0f)
        tmp_f_val = 255.0f;
      ui_tmp[2] = (uint32_t)(tmp_f_val);

      tmp_f_val = (scale * ptr[(y * width + x) * 4 + 3]);
      if (tmp_f_val > 255.0f)
        tmp_f_val = 255.0f;
      ui_tmp[3] = 1; // Alfa

      out_u_int_buf[(height - 1 - y) * width + x] = 0x000000FF & ui_tmp[2];
      out_u_int_buf[(height - 1 - y) * width + x] |=
          0x0000FF00 & ((ui_tmp[1]) << 8);
      out_u_int_buf[(height - 1 - y) * width + x] |=
          0x00FF0000 & ((ui_tmp[0]) << 16);
      out_u_int_buf[(height - 1 - y) * width + x] |=
          0xFF000000 & ((ui_tmp[3]) << 24);
    }
  }

  bool res = save_image_as_bmp(out_u_int_buf, width, height, file_name);
  free(out_u_int_buf);
  return res;
}

bool BmpUtils::save_image_as_bmp_8u(unsigned char *ptr, size_t width,
                                    size_t height, const char *file_name) {
  uint32_t *out_u_int_buf =
      (uint32_t *)malloc(width * height * sizeof(uint32_t));
  if (!out_u_int_buf) {
    printf("Failed to allocate memory for output BMP image!\n");
    return false;
  }

  for (size_t y = 0; y < height; y++) {
    for (size_t x = 0; x < width; x++) {
      unsigned char uc_data = ptr[y * width + x];

      uint32_t ui_data =
          0xFF000000 + (uc_data << 16) + (uc_data << 8) + (uc_data);

      out_u_int_buf[(height - 1 - y) * width + x] = ui_data;
    }
  }

  bool res = save_image_as_bmp(out_u_int_buf, width, height, file_name);
  free(out_u_int_buf);
  return res;
}

bool BmpUtils::load_bmp_image(unsigned char *&data, size_t &width,
                              size_t &height, size_t &pitch,
                              uint16_t &bits_per_pixel, const char *file_name) {
  FILE *stream = NULL;

  stream = fopen(file_name, "rb");

  if (NULL == stream)
    return false;

  BMPFileHeader file_header;
  BMPInfoHeader info_header;
  uint32_t gap = 0;

  if (sizeof(file_header) !=
      fread(&file_header, 1, sizeof(file_header), stream)) {
    goto error_exit;
  }

  if (sizeof(info_header) !=
      fread(&info_header, 1, sizeof(info_header), stream)) {
    goto error_exit;
  }

  gap = file_header.bf_off_bits_ - sizeof(file_header) - sizeof(info_header);
  fseek(stream, gap, SEEK_CUR);

  width = info_header.bi_width_;
  height = info_header.bi_height_ > 0 ? info_header.bi_height_
                                      : -info_header.bi_height_;
  bits_per_pixel = info_header.bi_bit_count_;
  pitch = info_header.bi_width_ * ((info_header.bi_bit_count_ + 7) / 8);

  // Pitch is a multiple of four bytes:
  pitch = (pitch + (4 - 1)) & ~(4 - 1);

  if ((height > (1 << 16)) || (pitch > (1 << 16)))
    goto error_exit;

  data = new unsigned char[height * pitch];
  if (NULL == data)
    goto error_exit;

  if (info_header.bi_height_ > 0) {
    for (size_t h = 0; h < height; h++) {
      unsigned char *dst = data + (height - h - 1) * pitch;
      if (pitch != fread(dst, 1, pitch, stream)) {
        goto error_exit;
      }
    }
  } else {
    if (height * pitch != fread(data, 1, height * pitch, stream)) {
      goto error_exit;
    }
  }

  fclose(stream);
  return true;

error_exit:
  fclose(stream);
  return false;
}

bool BmpUtils::load_bmp_image_8u(unsigned char *&data, size_t &width,
                                 size_t &height, const char *file_name) {
  // First, load the BMP image generally:
  size_t src_pitch = 0;
  uint16_t src_bits_per_pixel = 0;

  bool success = load_bmp_image(data, width, height, src_pitch,
                                src_bits_per_pixel, file_name);

  if (src_bits_per_pixel != 8 && src_bits_per_pixel != 24 &&
      src_bits_per_pixel != 32) {
    success = false;
  }

  if ((width > (1 << 16)) || (height > (1 << 16))) {
    success = false;
  }

  size_t dst_pitch = width;
  unsigned char *data8u = NULL;

  if (success) {
    data8u = new unsigned char[height * dst_pitch];
    if (NULL == data8u) {
      success = false;
    }
    if ((width > (1 << 16)) || (height > (1 << 16))) {
      success = false;
    }
  }

  if (success) {
    unsigned char *src_start = data;
    unsigned char *dst_start = data8u;
    uint16_t src_bytes_per_pixel = (src_bits_per_pixel + 7) / 8;
    uint16_t dst_bytes_per_pixel = 1;

    for (size_t h = 0; h < height;
         h++, src_start += src_pitch, dst_start += dst_pitch) {
      unsigned char *src_pixel = src_start;
      unsigned char *dst_pixel = dst_start;

      for (size_t w = 0; w < width; w++, src_pixel += src_bytes_per_pixel,
                  dst_pixel += dst_bytes_per_pixel) {
        switch (src_bits_per_pixel) {
        case 8:
          dst_pixel[0] = src_pixel[0];
          break;
        case 24:
        case 32:
          dst_pixel[0] = (char)(0.21f * src_pixel[0] + 0.72f * src_pixel[1] +
                                0.07f * src_pixel[2]);
          break;
        default:
          printf("Unexpected bits per pixel %d!\n", src_bits_per_pixel);
        }
      }
    }
  }

  delete[] data;
  data = data8u;

  return success;
}
} // namespace compute_samples
