/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "bmp.hpp"

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory.h>

#include "logging/logging.hpp"

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

const uint32_t bi_rgb = 0;

bool BmpUtils::save_image_as_bmp(uint32_t *ptr, int width, int height,
                                 const char *file_name) {
  FILE *stream = nullptr;
  uint32_t *ppix = ptr;
  stream = fopen(file_name, "wb");

  if (nullptr == stream) {
    return false;
  }

  BMPFileHeader file_header;
  BMPInfoHeader info_header;

  int align_size = width * 4;
  align_size ^= 0x03;
  align_size++;
  align_size &= 0x03;

  int row_length = width * 4 + align_size;

  file_header.bf_type_ = 0x4D42; // 'BM'
  file_header.bf_size_ =
      sizeof(file_header) + sizeof(info_header) + row_length * height;
  file_header.bf_off_bits_ = sizeof(file_header) + sizeof(info_header);
  file_header.bf_reserved1_ = 0;
  file_header.bf_reserved2_ = 0;

  info_header.bi_size_ = sizeof(BMPInfoHeader);
  info_header.bi_width_ = width;
  info_header.bi_height_ = height;
  info_header.bi_planes_ = 1;
  info_header.bi_bit_count_ = 32;
  info_header.bi_compression_ = bi_rgb;
  info_header.bi_size_image_ = row_length * height;
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

  uint8_t buffer[4];
  memset(buffer, 0, sizeof(buffer));
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      ppix = ptr + (height - 1 - y) * width + x;
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

bool BmpUtils::save_image_as_bmp_32fc4(const float *ptr, float scale, int width,
                                       int height, const char *file_name) {
  // save results in bitmap files
  float tmp_f_val = 0.0f;
  std::vector<uint32_t> out_u_int_buf(width * height);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      // Ensure that no value is greater than 255.0
      uint32_t ui_tmp[4];
      tmp_f_val = (scale * ptr[(y * width + x) * 4 + 0]);
      if (tmp_f_val > 255.0f) {
        tmp_f_val = 255.0f;
      }
      ui_tmp[0] = (uint32_t)(tmp_f_val);

      tmp_f_val = (scale * ptr[(y * width + x) * 4 + 1]);
      if (tmp_f_val > 255.0f) {
        tmp_f_val = 255.0f;
      }
      ui_tmp[1] = (uint32_t)(tmp_f_val);

      tmp_f_val = (scale * ptr[(y * width + x) * 4 + 2]);
      if (tmp_f_val > 255.0f) {
        tmp_f_val = 255.0f;
      }
      ui_tmp[2] = (uint32_t)(tmp_f_val);

      tmp_f_val = (scale * ptr[(y * width + x) * 4 + 3]);
      if (tmp_f_val > 255.0f) {
        tmp_f_val = 255.0f;
      }
      ui_tmp[3] = 1; // Alfa

      out_u_int_buf[y * width + x] = 0x000000FF & ui_tmp[2];
      out_u_int_buf[y * width + x] |= 0x0000FF00 & ((ui_tmp[1]) << 8);
      out_u_int_buf[y * width + x] |= 0x00FF0000 & ((ui_tmp[0]) << 16);
      out_u_int_buf[y * width + x] |= 0xFF000000 & ((ui_tmp[3]) << 24);
    }
  }

  return save_image_as_bmp(out_u_int_buf.data(), width, height, file_name);
}

bool BmpUtils::save_image_as_bmp_8u(const uint8_t *ptr, int width, int height,
                                    const char *file_name) {
  std::vector<uint32_t> out_u_int_buf(width * height);

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      uint8_t uc_data = ptr[y * width + x];

      uint32_t ui_data =
          0xFF000000 + (uc_data << 16) + (uc_data << 8) + (uc_data);

      out_u_int_buf[y * width + x] = ui_data;
    }
  }

  return save_image_as_bmp(out_u_int_buf.data(), width, height, file_name);
}

bool BmpUtils::load_bmp_image(uint8_t *&data, int &width, int &height,
                              int &pitch, uint16_t &bits_per_pixel,
                              const char *file_name) {
  FILE *stream = nullptr;

  stream = fopen(file_name, "rb");

  if (nullptr == stream) {
    return false;
  }

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

  if ((height > (1 << 16)) || (pitch > (1 << 16))) {
    goto error_exit;
  }

  data = new uint8_t[height * pitch];
  if (nullptr == data) {
    goto error_exit;
  }

  if (info_header.bi_height_ > 0) {
    for (int h = 0; h < height; h++) {
      uint8_t *dst = data + (height - h - 1) * pitch;
      if (pitch != static_cast<int>(fread(dst, 1, pitch, stream))) {
        goto error_exit;
      }
    }
  } else {
    if (height * pitch !=
        static_cast<int>(fread(data, 1, height * pitch, stream))) {
      goto error_exit;
    }
  }

  fclose(stream);
  return true;

error_exit:
  fclose(stream);
  return false;
}

bool BmpUtils::load_bmp_image_8u(uint8_t *&data, int &width, int &height,
                                 const char *file_name) {
  // First, load the BMP image generally:
  int src_pitch = 0;
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

  int dst_pitch = width;
  uint8_t *data8u = nullptr;

  if (success) {
    data8u = new uint8_t[height * dst_pitch];
    if (nullptr == data8u) {
      success = false;
    }
    if ((width > (1 << 16)) || (height > (1 << 16))) {
      success = false;
    }
  }

  if (success) {
    uint8_t *src_start = data;
    uint8_t *dst_start = data8u;
    uint16_t src_bytes_per_pixel = (src_bits_per_pixel + 7) / 8;
    uint16_t dst_bytes_per_pixel = 1;

    for (int h = 0; h < height;
         h++, src_start += src_pitch, dst_start += dst_pitch) {
      uint8_t *src_pixel = src_start;
      uint8_t *dst_pixel = dst_start;

      for (int w = 0; w < width; w++, src_pixel += src_bytes_per_pixel,
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
          LOG_WARNING << "Unexpected bits per pixel: " << src_bits_per_pixel;
        }
      }
    }
  }

  delete[] data;
  data = data8u;

  return success;
}
} // namespace compute_samples
