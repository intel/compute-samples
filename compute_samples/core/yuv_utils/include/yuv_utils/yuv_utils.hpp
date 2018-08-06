/*
 * Copyright(c) 2018 Intel Corporation
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
  static PlanarImage *create_planar_image(int width, int height,
                                          int pitch_y = 0);
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

  int get_pitch_y() const { return pitch_y_; }
  int get_pitch_u() const { return pitch_u_; }
  int get_pitch_v() const { return pitch_v_; }

  int get_width() const { return width_; }
  int get_height() const { return height_; }

private:
  PlanarImage()
      : y_(NULL), u_(NULL), v_(NULL), width_(0), height_(0), pitch_y_(0),
        pitch_u_(0), pitch_v_(0) {}
  ~PlanarImage() {}
  void draw_pixel(int32_t x, int32_t y, uint8_t *pic, int pic_width,
                  int pic_height, uint8_t u8_pixel);
  void draw_line(int32_t x0, int32_t y0, int32_t dx, int32_t dy, uint8_t *pic,
                 int pic_width, int pic_height, uint8_t u8_pixel);

  void compute_num_mvs(int pic_width, int pic_height, int &mv_image_width,
                       int &mv_image_height, int &mb_image_width,
                       int &mb_image_height) const;

  uint8_t *y_;
  uint8_t *u_;
  uint8_t *v_;
  int width_;
  int height_;
  int pitch_y_;
  int pitch_u_;
  int pitch_v_;
};

class Capture {
public:
  static Capture *create_file_capture(const std::string &fn, int width,
                                      int height, int frames);
  static void release(Capture *cap);

  virtual ~Capture() {}
  virtual void get_sample(int frameNum, PlanarImage &im) = 0;
  virtual void get_sample(int frameNum, PlanarImage &im, bool interlaced,
                          int polarity) = 0;

  int get_width() const { return width_; }
  int get_height() const { return height_; }
  int get_num_frames() const { return num_frames_; }

protected:
  Capture() : width_(0), height_(0), num_frames_(0){};

  int width_;
  int height_;
  int num_frames_;

private:
  Capture(const Capture &);
};

class FrameWriter {
public:
  static FrameWriter *create_frame_writer(int width, int height,
                                          int frameNumHint = 0,
                                          bool bFormatBMPHint = false);
  static void release(FrameWriter *cap);

  virtual void append_frame(const PlanarImage &im) = 0;
  virtual void write_to_file(const char *fn) = 0;

  int get_width() const { return width_; }
  int get_height() const { return height_; }

protected:
  FrameWriter(int width, int height)
      : width_(width), height_(height), curr_frame_(0) {}
  virtual ~FrameWriter() {}

  int width_;
  int height_;

  int curr_frame_;

private:
  FrameWriter(const FrameWriter &);
};
} // namespace compute_samples
