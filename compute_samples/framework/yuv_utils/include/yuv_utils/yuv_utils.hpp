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

#pragma once
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>

namespace compute_samples {

typedef struct { int16_t x, y; } motion_vector;
typedef uint16_t residual;
typedef struct { uint8_t x, y; } inter_shape;
typedef uint8_t intra_shape;

class PlanarImage {
public:
  static PlanarImage *create_planar_image(size_t width, size_t height,
                                          size_t pitch_y = 0);
  static void release_image(PlanarImage *im);

  void overlay_vectors(const motion_vector *mvs, const inter_shape *shapes);
  void overlay_vectors(const motion_vector *mvs,
                       const inter_shape *inter_shapes,
                       const intra_shape *intra_shapes,
                       const residual *inter_residuals,
                       const residual *intra_residuals);

  uint8_t *get_y() const { return y_; }
  uint8_t *get_u() const { return u_; }
  uint8_t *get_v() const { return v_; }

  size_t get_pitch_y() const { return pitch_y_; }
  size_t get_pitch_u() const { return pitch_u_; }
  size_t get_pitch_v() const { return pitch_v_; }

  size_t get_width() const { return width_; }
  size_t get_height() const { return height_; }

private:
  PlanarImage()
      : y_(NULL), u_(NULL), v_(NULL), width_(0), height_(0), pitch_y_(0),
        pitch_u_(0), pitch_v_(0) {}
  ~PlanarImage() {}
  void draw_pixel(int32_t x, int32_t y, uint8_t *pic, size_t pic_width,
                  size_t pic_height, uint8_t u8_pixel);
  void draw_line(int32_t x0, int32_t y0, int32_t dx, int32_t dy, uint8_t *pic,
                 size_t pic_width, size_t pic_height, uint8_t u8_pixel);

  void compute_num_mvs(size_t pic_width, size_t pic_height,
                       size_t &mv_image_width, size_t &mv_image_height,
                       size_t &mb_image_width, size_t &mb_image_height) const;

  uint8_t *y_;
  uint8_t *u_;
  uint8_t *v_;
  size_t width_;
  size_t height_;
  size_t pitch_y_;
  size_t pitch_u_;
  size_t pitch_v_;
};

class Capture {
public:
  static Capture *create_file_capture(const std::string &fn, size_t width,
                                      size_t height, size_t frames);
  static void release(Capture *cap);

  virtual ~Capture() {}
  virtual void get_sample(size_t frameNum, PlanarImage &im) = 0;
  virtual void get_sample(size_t frameNum, PlanarImage &im, bool interlaced,
                          unsigned char polarity) = 0;

  size_t get_width() const { return width_; }
  size_t get_height() const { return height_; }
  size_t get_num_frames() const { return num_frames_; }

protected:
  Capture() : width_(0), height_(0), num_frames_(0){};

  size_t width_;
  size_t height_;
  size_t num_frames_;

private:
  Capture(const Capture &);
};

class FrameWriter {
public:
  static FrameWriter *create_frame_writer(size_t width, size_t height,
                                          size_t frameNumHint = 0,
                                          bool bFormatBMPHint = false);
  static void release(FrameWriter *cap);

  virtual void append_frame(const PlanarImage &im) = 0;
  virtual void write_to_file(const char *fn) = 0;

  size_t get_width() const { return width_; }
  size_t get_height() const { return height_; }

protected:
  FrameWriter(size_t width, size_t height)
      : width_(width), height_(height), curr_frame_(0) {}
  virtual ~FrameWriter() {}

  size_t width_;
  size_t height_;

  size_t curr_frame_;

private:
  FrameWriter(const FrameWriter &);
};
} // namespace compute_samples
