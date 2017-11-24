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

#include "yuv_utils/yuv_utils.hpp"

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string.h>
#include <vector>

#include "image/image.hpp"

namespace compute_samples {
PlanarImage *PlanarImage::create_planar_image(size_t width, size_t height,
                                              size_t pitch_y) {
  PlanarImage *im = new PlanarImage;

  if (pitch_y == 0) {
    pitch_y = width;
  }

  const size_t num_pixels = pitch_y * height + width * height / 2;
#ifdef __linux__
  int ret = 0;
  ret = posix_memalign((void **)(&(im->y_)), 0x1000, num_pixels);
  if (ret) {
    delete im;
    throw std::runtime_error("Allocation failed");
  }
#else
  im->y_ = (uint8_t *)_aligned_malloc(num_pixels, 0x1000);
  if (!im->y_) {
    delete im;
    throw std::runtime_error("Allocation failed");
  }
#endif

  im->u_ = im->y_ + pitch_y * height;
  im->v_ = im->u_ + width * height / 4;

  im->width_ = width;
  im->height_ = height;
  im->pitch_y_ = pitch_y;
  im->pitch_u_ = width / 2;
  im->pitch_v_ = width / 2;

  return im;
}

void PlanarImage::release_image(PlanarImage *im) {
#ifdef __linux__
  free(im->y_);
#else
  _aligned_free(im->y_);
#endif
  delete im;
}

void PlanarImage::draw_pixel(int32_t x, int32_t y, uint8_t *pic,
                             size_t pic_width, size_t pic_height,
                             uint8_t u8_pixel) {
  int32_t pix_pos;

  // Don't draw out of bound pixels
  if (x < 0 || x >= pic_width || y < 0 || y >= pic_height) {
    return;
  }

  pix_pos = y * pic_width + x;
  *(pic + pix_pos) = u8_pixel;
}

// Bresenham's line algorithm
void PlanarImage::draw_line(int32_t x0, int32_t y0, int32_t dx, int32_t dy,
                            uint8_t *pic, size_t pic_width, size_t pic_height,
                            uint8_t u8_pixel) {
  using std::swap;

  int32_t x1 = x0 + dx;
  int32_t y1 = y0 + dy;
  bool b_steep = abs(dy) > abs(dx);
  if (b_steep) {
    swap(x0, y0);
    swap(x1, y1);
  }
  if (x0 > x1) {
    swap(x0, x1);
    swap(y0, y1);
  }
  int32_t delta_x = x1 - x0;
  int32_t delta_y = abs(y1 - y0);
  int32_t error = delta_x / 2;
  int32_t y_step;
  if (y0 < y1)
    y_step = 1;
  else
    y_step = -1;

  for (; x0 <= x1; x0++) {
    if (b_steep)
      draw_pixel(y0, x0, pic, pic_width, pic_height, u8_pixel);
    else
      draw_pixel(x0, y0, pic, pic_width, pic_height, u8_pixel);

    error -= delta_y;
    if (error < 0) {
      y0 += y_step;
      error += delta_x;
    }
  }
}

typedef struct { uint8_t s[4]; } uchar4_t;

void PlanarImage::compute_num_mvs(size_t pic_width, size_t pic_height,
                                  size_t &mv_image_width,
                                  size_t &mv_image_height,
                                  size_t &mb_image_width,
                                  size_t &mb_image_height) const {
  size_t pic_width_in_blk = (pic_width + 16 - 1) / 16;
  size_t pic_height_in_blk = (pic_height + 16 - 1) / 16;

  mb_image_width = pic_width_in_blk;
  mb_image_height = pic_height_in_blk;
  mv_image_width = pic_width_in_blk * 4;
  mv_image_height = pic_height_in_blk * 4;
}

void PlanarImage::overlay_vectors(const motion_vector *mvs,
                                  const inter_shape *shapes) {
  size_t mv_image_width, mv_image_height;
  size_t mb_image_width, mb_image_height;
  compute_num_mvs(width_, height_, mv_image_width, mv_image_height,
                  mb_image_width, mb_image_height);

#define OFF(P) (P + 2) >> 2

  for (size_t i = 0; i < mb_image_height; i++) {
    for (size_t j = 0; j < mb_image_width; j++) {
      size_t mb_index = j + i * mb_image_width;
      // Selectively Draw motion vectors for different sub block sizes
      size_t j0 = j * 16;
      size_t i0 = i * 16;
      size_t m0 = mb_index * 16;
      switch (shapes[mb_index].x) {
      case 0:
        draw_line(j0 + 8, i0 + 8, OFF(mvs[m0].x), OFF(mvs[m0].y), y_, width_,
                  height_, 180);
        break;
      case 1:
        draw_line(j0 + 8, i0 + 4, OFF(mvs[m0].x), OFF(mvs[m0].y), y_, width_,
                  height_, 180);
        draw_line(j0 + 8, i0 + 12, OFF(mvs[m0 + 8].x), OFF(mvs[m0 + 8].y), y_,
                  width_, height_, 180);
        break;
      case 2:
        draw_line(j0 + 4, i0 + 8, OFF(mvs[m0].x), OFF(mvs[m0].y), y_, width_,
                  height_, 180);
        draw_line(j0 + 12, i0 + 8, OFF(mvs[m0 + 8].x), OFF(mvs[m0 + 8].y), y_,
                  width_, height_, 180);
        break;
      case 3:
        uchar4_t minor_shapes;
        minor_shapes.s[0] = (shapes[mb_index].y) & 0x03;
        minor_shapes.s[1] = (shapes[mb_index].y >> 2) & 0x03;
        minor_shapes.s[2] = (shapes[mb_index].y >> 4) & 0x03;
        minor_shapes.s[3] = (shapes[mb_index].y >> 6) & 0x03;
        for (size_t m = 0; m < 4; ++m) {
          size_t mdiv = m / 2;
          size_t mmod = m % 2;
          switch (minor_shapes.s[m]) {
          case 0: // 8 x 8
            draw_line(j0 + mmod * 8 + 4, i0 + mdiv * 8 + 4,
                      OFF(mvs[m0 + m * 4].x), OFF(mvs[m0 + m * 4].y), y_,
                      width_, height_, 180);
            break;
          case 1: // 8 x 4
            for (size_t n = 0; n < 2; ++n) {
              draw_line(j0 + mmod * 8 + 4, i0 + (mdiv * 8 + n * 4 + 2),
                        OFF(mvs[m0 + m * 4 + n * 2].x),
                        OFF(mvs[m0 + m * 4 + n * 2].y), y_, width_, height_,
                        180);
            }
            break;
          case 2: // 4 x 8
            for (size_t n = 0; n < 2; ++n) {
              draw_line(j0 + (mmod * 8 + n * 4 + 2), i0 + mdiv * 8 + 4,
                        OFF(mvs[m0 + m * 4 + n * 2].x),
                        OFF(mvs[m0 + m * 4 + n * 2].y), y_, width_, height_,
                        180);
            }
            break;
          case 3: // 4 x 4
            for (size_t n = 0; n < 4; ++n) {
              draw_line(j0 + n * 4 + 2, i0 + m * 4 + 2,
                        OFF(mvs[m0 + m * 4 + n].x), OFF(mvs[m0 + m * 4 + n].y),
                        y_, width_, height_, 180);
            }
            break;
          }
        }
        break;
      }
    }
  }
}

void PlanarImage::overlay_vectors(const motion_vector *mvs,
                                  const inter_shape *inter_shapes,
                                  const intra_shape *intra_shapes,
                                  const residual *inter_residuals,
                                  const residual *intra_residuals) {
  size_t mv_image_width, mv_image_height;
  size_t mb_image_width, mb_image_height;
  compute_num_mvs(width_, height_, mv_image_width, mv_image_height,
                  mb_image_width, mb_image_height);

#define OFF(P) (P + 2) >> 2

  for (size_t i = 0; i < mb_image_height; i++) {
    for (size_t j = 0; j < mb_image_width; j++) {
      size_t mb_index = j + i * mb_image_width;
      // Selectively Draw motion vectors for different sub block sizes
      size_t j0 = j * 16;
      size_t i0 = i * 16;
      size_t m0 = mb_index * 16;

      if (inter_residuals[mb_index] < intra_residuals[mb_index]) {
        switch (inter_shapes[mb_index].x) {
        case 0: {
          draw_line(j0 + 8, i0 + 8, OFF(mvs[m0].x), OFF(mvs[m0].y), y_, width_,
                    height_, 180);
          break;
        }
        case 1: {
          draw_line(j0 + 8, i0 + 4, OFF(mvs[m0].x), OFF(mvs[m0].y), y_, width_,
                    height_, 180);
          draw_line(j0 + 8, i0 + 12, OFF(mvs[m0 + 8].x), OFF(mvs[m0 + 8].y), y_,
                    width_, height_, 180);
          break;
        }
        case 2: {
          draw_line(j0 + 4, i0 + 8, OFF(mvs[m0].x), OFF(mvs[m0].y), y_, width_,
                    height_, 180);
          draw_line(j0 + 12, i0 + 8, OFF(mvs[m0 + 8].x), OFF(mvs[m0 + 8].y), y_,
                    width_, height_, 180);
          break;
        }
        case 3: {
          uchar4_t minor_shapes;
          minor_shapes.s[0] = (inter_shapes[mb_index].y) & 0x03;
          minor_shapes.s[1] = (inter_shapes[mb_index].y >> 2) & 0x03;
          minor_shapes.s[2] = (inter_shapes[mb_index].y >> 4) & 0x03;
          minor_shapes.s[3] = (inter_shapes[mb_index].y >> 6) & 0x03;
          for (size_t m = 0; m < 4; ++m) {
            size_t mdiv = m / 2;
            size_t mmod = m % 2;
            switch (minor_shapes.s[m]) {
            case 0: {
              break;
            }
            case 1: {
              for (size_t n = 0; n < 2; ++n) {
                draw_line(j0 + mmod * 8 + 4, i0 + (mdiv * 8 + n * 4 + 2),
                          OFF(mvs[m0 + m * 4 + n * 2].x),
                          OFF(mvs[m0 + m * 4 + n * 2].y), y_, width_, height_,
                          180);
              }
              break;
            }
            case 2: {
              for (size_t n = 0; n < 2; ++n) {
                draw_line(j0 + (mmod * 8 + n * 4 + 2), i0 + mdiv * 8 + 4,
                          OFF(mvs[m0 + m * 4 + n * 2].x),
                          OFF(mvs[m0 + m * 4 + n * 2].y), y_, width_, height_,
                          180);
              }
              break;
            }
            case 3: {
              for (size_t n = 0; n < 4; ++n) {
                draw_line(j0 + n * 4 + 2, i0 + m * 4 + 2,
                          OFF(mvs[m0 + m * 4 + n].x),
                          OFF(mvs[m0 + m * 4 + n].y), y_, width_, height_, 180);
              }
              break;
            }
            }
          }
          break;
        }
        }
      } else {
        switch (intra_shapes[mb_index]) {
        case 0: {
          draw_line(j0, i0, 16, 0, y_, width_, height_, 180);
          draw_line(j0, i0, 0, 16, y_, width_, height_, 180);
          draw_line(j0 + 16, i0, 0, 16, y_, width_, height_, 180);
          draw_line(j0, i0 + 16, 16, 0, y_, width_, height_, 180);

          break;
        }
        case 1: {
          draw_line(j0, i0, 16, 0, y_, width_, height_, 180);
          draw_line(j0, i0, 0, 16, y_, width_, height_, 180);
          draw_line(j0 + 16, i0, 0, 16, y_, width_, height_, 180);
          draw_line(j0, i0 + 16, 16, 0, y_, width_, height_, 180);

          draw_line(j0 + 8, i0, 0, 16, y_, width_, height_, 180);
          draw_line(j0, i0 + 8, 16, 0, y_, width_, height_, 180);

          break;
        }
        case 2: {
          draw_line(j0, i0, 16, 0, y_, width_, height_, 180);
          draw_line(j0, i0, 0, 16, y_, width_, height_, 180);
          draw_line(j0 + 16, i0, 0, 16, y_, width_, height_, 180);
          draw_line(j0, i0 + 16, 16, 0, y_, width_, height_, 180);

          draw_line(j0 + 8, i0, 0, 16, y_, width_, height_, 180);
          draw_line(j0, i0 + 8, 16, 0, y_, width_, height_, 180);

          draw_line(j0 + 4, i0, 0, 16, y_, width_, height_, 180);
          draw_line(j0 + 12, i0, 0, 16, y_, width_, height_, 180);
          draw_line(j0, i0 + 4, 16, 0, y_, width_, height_, 180);
          draw_line(j0, i0 + 12, 16, 0, y_, width_, height_, 180);

          break;
        }
        default:
          throw std::out_of_range("PlanarImage::overlay_vectors: Invalid "
                                  "intra shape value encountered.");
        }
      }
    }
  }
}

class YuvCapture : public Capture {
public:
  YuvCapture(const std::string &fn, size_t width, size_t height,
             size_t frames = 0);
  virtual void get_sample(size_t frame_num, PlanarImage &im);
  virtual void get_sample(size_t frame_num, PlanarImage &im, bool interlaced,
                          unsigned char polarity);

protected:
  std::ifstream file_;
};

YuvCapture::YuvCapture(const std::string &fn, size_t width, size_t height,
                       size_t frames)
    : file_(fn.c_str(), std::ios::binary | std::ios::ate) {

  if (!file_.good()) {
    std::stringstream ss;
    ss << "Unable to load YUV file: " << fn;
    throw std::invalid_argument(ss.str().c_str());
  }

  const size_t file_size = static_cast<size_t>(file_.tellg());
  const size_t frame_size = width * height * 3 / 2 * sizeof(uint8_t);

  if (file_size % frame_size) {
    throw std::invalid_argument("YUV file file size error. Wrong dimensions?");
  }

  file_.seekg(0, std::ios::beg);

  num_frames_ = (frames == 0) ? (file_size / frame_size) : frames;
  width_ = width;
  height_ = height;
}

void YuvCapture::get_sample(size_t frame_num, PlanarImage &im) {
  if (im.get_width() != (size_t)width_ || im.get_height() != (size_t)height_) {
    throw std::runtime_error("Capture::get_frame: output image size mismatch.");
  }

  const size_t frame_size = width_ * height_ * 3 / 2 * sizeof(uint8_t);
  file_.clear();
  file_.seekg(frame_num * frame_size);

  size_t in_row_size = width_ * sizeof(uint8_t);
  size_t out_row_size = im.get_pitch_y() * sizeof(uint8_t);
  uint8_t *out_ptr = im.get_y();
  for (size_t i = 0; i < height_; ++i) {
    file_.read(reinterpret_cast<char *>(out_ptr), in_row_size);
    out_ptr += out_row_size;
  }
  assert(out_ptr == im.get_u());
  out_ptr = im.get_u();
  in_row_size = (width_ / 2) * sizeof(uint8_t);
  out_row_size = im.get_pitch_u() * sizeof(uint8_t);
  for (size_t i = 0; i < height_ / 2; ++i) {
    file_.read(reinterpret_cast<char *>(out_ptr), in_row_size);
    out_ptr += out_row_size;
  }
  assert(out_ptr == im.get_v());
  out_ptr = im.get_v();
  in_row_size = (width_ / 2) * sizeof(uint8_t);
  out_row_size = im.get_pitch_v() * sizeof(uint8_t);
  for (size_t i = 0; i < height_ / 2; ++i) {
    file_.read(reinterpret_cast<char *>(out_ptr), in_row_size);
    out_ptr += out_row_size;
  }
}

void YuvCapture::get_sample(size_t frame_num, PlanarImage &im, bool interlaced,
                            unsigned char polarity) {
  size_t divisor = interlaced ? 2 : 1;
  if (im.get_width() != (size_t)width_ ||
      im.get_height() != (size_t)height_ / divisor) {
    throw std::runtime_error("Capture::get_frame: output image size mismatch.");
  }

  const size_t frame_size = width_ * height_ * 3 / 2 * sizeof(uint8_t);
  file_.clear();
  file_.seekg(frame_num * frame_size);

  size_t in_row_size = width_ * sizeof(uint8_t);
  size_t out_row_size = im.get_pitch_y() * sizeof(uint8_t);
  uint8_t *out_ptr = im.get_y();
  for (size_t i = 0; i < im.get_height(); ++i) {
    if (interlaced && (polarity == 1)) {
      file_.ignore(in_row_size);
    }
    file_.read(reinterpret_cast<char *>(out_ptr), in_row_size);
    if (interlaced && (polarity == 0)) {
      file_.ignore(in_row_size);
    }
    out_ptr += out_row_size;
  }
  assert(out_ptr == im.get_u());
  out_ptr = im.get_u();
  in_row_size = (width_ / 2) * sizeof(uint8_t);
  out_row_size = im.get_pitch_u() * sizeof(uint8_t);
  for (size_t i = 0; i < im.get_height() / 2; ++i) {
    if (interlaced && (polarity == 1)) {
      file_.ignore(in_row_size);
    }
    file_.read(reinterpret_cast<char *>(out_ptr), in_row_size);
    if (interlaced && (polarity == 0)) {
      file_.ignore(in_row_size);
    }
    out_ptr += out_row_size;
  }
  assert(out_ptr == im.get_v());
  out_ptr = im.get_v();
  in_row_size = (width_ / 2) * sizeof(uint8_t);
  out_row_size = im.get_pitch_v() * sizeof(uint8_t);
  for (size_t i = 0; i < im.get_height() / 2; ++i) {
    if (interlaced && (polarity == 1)) {
      file_.ignore(in_row_size);
    }
    file_.read(reinterpret_cast<char *>(out_ptr), in_row_size);
    if (interlaced && (polarity == 0)) {
      file_.ignore(in_row_size);
    }
    out_ptr += out_row_size;
  }
}

Capture *Capture::create_file_capture(const std::string &fn, size_t width,
                                      size_t height, size_t frames) {
  Capture *cap = NULL;

  if ((strstr(fn.c_str(), ".yuv") != NULL) ||
      (strstr(fn.c_str(), ".yv12") != NULL)) {
    cap = new YuvCapture(fn, width, height, frames);
  } else {
    throw std::runtime_error("Unsupported capture file format.");
  }

  return cap;
}

void Capture::release(Capture *cap) { delete cap; }

class YuvWriter : public FrameWriter {
public:
  YuvWriter(size_t width, size_t height, size_t frame_num_hint,
            bool b_to_bmps = false);
  virtual ~YuvWriter() {}

  void append_frame(const PlanarImage &im);
  void write_to_file(const char *fn);

private:
  std::vector<uint8_t> data_;
  bool b_to_bmps_;
};

void YuvWriter::write_to_file(const char *fn) {
  if (b_to_bmps_) {
    std::string out_file(fn);
    std::size_t found = out_file.find('.');
    // crop the name
    out_file = out_file.substr(0, found);
    uchar4_t *frame = (uchar4_t *)malloc(width_ * height_ * sizeof(uchar4_t));
    if (!frame) {
      throw std::runtime_error("Failed to allocate a buffer for the bitmap.");
    }
    const size_t uv_width = width_ / 2;
    const size_t uv_height = height_ / 2;
    for (size_t y = 0; y < curr_frame_; ++y) {
      // Y (a value per pixel)
      const uint8_t *p_img_y = &data_[y * width_ * height_ * 3 / 2];
      // U (a value per 4 pixels) and V (a value per 4 pixels)
      const uint8_t *p_img_u =
          p_img_y +
          width_ *
              height_; // U plane is after Y plane (which is width_ * height_)
      const uint8_t *p_img_v =
          p_img_u + uv_width * uv_height; // V plane is after U plane (which is
                                          // uv_width * uv_height)

      memset(frame, 0, width_ * height_ * sizeof(uchar4_t));
      for (size_t i = 0; i < height_; ++i) {
        for (size_t j = 0; j < width_; ++j) {
          // Y value
          uint8_t Y = p_img_y[j + width_ * (height_ - 1 - i)];
          // the same U value 4 times, thus both i and j are divided by 2
          uint8_t U = p_img_u[j / 2 + uv_width * (uv_height - 1 - i / 2)];
          uint8_t V = p_img_v[j / 2 + uv_width * (uv_height - 1 - i / 2)];

          using namespace std;
          // R is the 3rd component in the bitmap (which is actualy stored as
          // BGRA)
          const int32_t R =
              (int32_t)(1.164f * (float(Y) - 16) + 1.596f * (float(V) - 128));
          frame[j + width_ * i].s[2] = std::min(255, std::max(R, 0));
          // G
          const int32_t G =
              (int32_t)(1.164f * (float(Y) - 16) - 0.813f * (float(V) - 128) -
                        0.391f * (float(U) - 128));
          frame[j + width_ * i].s[1] = std::min(255, std::max(G, 0));
          // B
          const int32_t B =
              (int32_t)(1.164f * (float(Y) - 16) + 2.018f * (float(U) - 128));
          frame[j + width_ * i].s[0] = std::min(255, std::max(B, 0));
        }
      }
      std::stringstream number;
      number << y;
      std::string filename = out_file + number.str() + std::string(".bmp");
      ImageBMP32Bit image(width_, height_);
      if (image.write(filename, reinterpret_cast<uint32_t *>(frame))) {
        free(frame);
        throw std::runtime_error("Failed to write output bitmap file.");
      }
    }
    free(frame);
  }

  // YUV file
  std::ofstream out_file(fn, std::ios::binary);
  if (!out_file.good()) {
    throw std::runtime_error("Failed opening output file.");
  }
  out_file.write(reinterpret_cast<char *>(&data_[0]), data_.size());
  out_file.close();
}

void YuvWriter::append_frame(const PlanarImage &im) {
  data_.resize((curr_frame_ + 1) * width_ * height_ * 3 / 2);

  uint8_t *p_src = (uint8_t *)im.get_y();
  uint8_t *p_dst = &data_[curr_frame_ * width_ * height_ * 3 / 2];
  size_t p_dst_size = width_ * height_ * 3 / 2;
  for (size_t y = 0; y < im.get_height(); ++y) {
    if (im.get_width() <= p_dst_size) {
      memcpy(p_dst, p_src, im.get_width());
    } else {
      throw std::runtime_error("Failed to append frame.");
    }
    p_src += im.get_pitch_y();
    p_dst += im.get_width();
    p_dst_size -= im.get_width();
  }

  p_src = (uint8_t *)im.get_u();
  for (size_t y = 0; y < im.get_height() / 2; ++y) {
    if (im.get_width() / 2 <= p_dst_size) {
      memcpy(p_dst, p_src, im.get_width() / 2);
    } else {
      throw std::runtime_error("Failed to append frame.");
    }
    p_src += im.get_pitch_u();
    p_dst += im.get_width() / 2;
    p_dst_size -= im.get_width() / 2;
  }

  p_src = (uint8_t *)im.get_v();
  for (size_t y = 0; y < im.get_height() / 2; ++y) {
    if (im.get_width() / 2 <= p_dst_size) {
      memcpy(p_dst, p_src, im.get_width() / 2);
    } else {
      throw std::runtime_error("Failed to append frame.");
    }
    p_src += im.get_pitch_v();
    p_dst += im.get_width() / 2;
    p_dst_size -= im.get_width() / 2;
  }

  ++curr_frame_;
}

YuvWriter::YuvWriter(size_t width, size_t height, size_t frame_num_hint,
                     bool b_to_bmps)
    : FrameWriter(width, height), b_to_bmps_(b_to_bmps) {
  if (frame_num_hint > 0) {
    data_.reserve(frame_num_hint * width_ * height_ * 3 / 2);
  }
}

FrameWriter *FrameWriter::create_frame_writer(size_t width, size_t height,
                                              size_t frame_num_hint,
                                              bool b_format_bmp_hint) {
  return new YuvWriter(width, height, frame_num_hint, b_format_bmp_hint);
}

void FrameWriter::release(FrameWriter *writer) { delete writer; }

} // namespace compute_samples
