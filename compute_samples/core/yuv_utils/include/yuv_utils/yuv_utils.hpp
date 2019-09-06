/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */
#pragma once
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>
#include <fstream>

#include "align_utils/align_utils.hpp"

namespace compute_samples {

typedef struct {
  int16_t x, y;
} motion_vector;
typedef uint16_t residual;
typedef struct {
  uint8_t x, y;
} inter_shape;
typedef uint8_t intra_shape;

class PlanarImage {
public:
  PlanarImage()
      : y_(nullptr), u_(nullptr), v_(nullptr), width_(0), height_(0),
        pitch_y_(0), pitch_u_(0), pitch_v_(0) {}
  PlanarImage(int width, int height, int pitch_y = 0);
  ~PlanarImage() = default;

  void overlay_vectors(const motion_vector *mvs, const inter_shape *shapes);
  void overlay_vectors(const motion_vector *mvs,
                       const inter_shape *inter_shapes,
                       const intra_shape *intra_shapes,
                       const residual *inter_residuals,
                       const residual *intra_residuals);

  uint8_t *get_y() const { return y_; }
  uint8_t *get_u() const { return u_; }
  uint8_t *get_v() const { return v_; }

  int get_pitch_y() const { return pitch_y_; }
  int get_pitch_u() const { return pitch_u_; }
  int get_pitch_v() const { return pitch_v_; }

  int get_width() const { return width_; }
  int get_height() const { return height_; }

private:
  void draw_pixel(int32_t x, int32_t y, uint8_t *pic, int pic_width,
                  int pic_height, uint8_t u8_pixel);
  void draw_line(int32_t x0, int32_t y0, int32_t dx, int32_t dy, uint8_t *pic,
                 int pic_width, int pic_height, uint8_t u8_pixel);

  void compute_num_mvs(int pic_width, int pic_height, int &mv_image_width,
                       int &mv_image_height, int &mb_image_width,
                       int &mb_image_height) const;

  align_utils::AlignedVector<uint8_t, 0x1000> data_;
  uint8_t *y_;
  uint8_t *u_;
  uint8_t *v_;
  int width_;
  int height_;
  int pitch_y_;
  int pitch_u_;
  int pitch_v_;
};

class YuvCapture {
public:
  YuvCapture() = default;
  YuvCapture(const std::string &fn, int width, int height, int frames = 0);
  ~YuvCapture() = default;

  void get_sample(int frame_num, PlanarImage &im);
  void get_sample(int frame_num, PlanarImage &im, bool interlaced,
                  int polarity);

  int get_width() const { return width_; }
  int get_height() const { return height_; }
  int get_num_frames() const { return num_frames_; }

private:
  std::ifstream file_;
  int width_;
  int height_;
  int num_frames_;
};

class YuvWriter {
public:
  YuvWriter() = default;
  YuvWriter(int width, int height, int frame_num_hint, bool b_to_bmps = false);
  ~YuvWriter() = default;

  void append_frame(const PlanarImage &im);
  void write_to_file(const char *fn);

  int get_width() const { return width_; }
  int get_height() const { return height_; }

private:
  int width_;
  int height_;
  int curr_frame_;
  bool b_to_bmps_;
  std::vector<uint8_t> data_;
};

} // namespace compute_samples
