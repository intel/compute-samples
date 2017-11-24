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

#ifndef COMPUTE_SAMPLES_IMAGE_HPP
#define COMPUTE_SAMPLES_IMAGE_HPP

#include <cstdint>
#include <string>
#include <vector>

namespace compute_samples {
template <typename T> class Image {
public:
  virtual ~Image() = default;
  virtual bool read(const std::string &image_path) = 0;
  virtual bool write(const std::string &image_path) = 0;
  virtual bool write(const std::string &image_path, const T *data) = 0;
  virtual size_t width() const = 0;
  virtual size_t height() const = 0;
  virtual size_t number_of_channels() const = 0;
  virtual size_t bits_per_channel() const = 0;
  virtual size_t bits_per_pixel() const = 0;
  virtual size_t size() const = 0;
  virtual size_t size_in_bytes() const = 0;
  virtual std::vector<T> get_pixels() const = 0;
  virtual void copy_raw_data(const T *data) = 0;
  virtual T *raw_data() = 0;
  virtual const T *raw_data() const = 0;
};

template <typename T> class ImagePNG : public Image<T> {
public:
  ImagePNG();
  ImagePNG(const std::string &image_path);
  ImagePNG(const unsigned width, const unsigned height);
  bool read(const std::string &image_path) override;
  bool write(const std::string &image_path) override;
  bool write(const std::string &image_path, const T *data) override;
  size_t width() const override;
  size_t height() const override;
  size_t number_of_channels() const override;
  size_t bits_per_channel() const;
  size_t bits_per_pixel() const;
  size_t size() const override;
  size_t size_in_bytes() const override;
  std::vector<T> get_pixels() const override;
  void copy_raw_data(const T *data) override;
  T *raw_data() override;
  const T *raw_data() const override;

  bool operator==(const ImagePNG &rhs) const;

private:
  std::vector<T> pixels_;
  size_t width_;
  size_t height_;
};

typedef ImagePNG<uint32_t> ImagePNG32Bit;

template <typename T> class ImageBMP : public Image<T> {
public:
  ImageBMP();
  ImageBMP(const std::string &image_path);
  ImageBMP(const unsigned width, const unsigned height);
  bool read(const std::string &image_path) override;
  bool write(const std::string &image_path) override;
  bool write(const std::string &image_path, const T *data) override;
  size_t width() const override;
  size_t height() const override;
  size_t number_of_channels() const override;
  size_t bits_per_channel() const;
  size_t bits_per_pixel() const;
  size_t size() const override;
  size_t size_in_bytes() const override;
  std::vector<T> get_pixels() const override;
  void copy_raw_data(const T *data) override;
  T *raw_data() override;
  const T *raw_data() const override;

  bool operator==(const ImageBMP &rhs) const;

private:
  std::vector<T> pixels_;
  size_t width_;
  size_t height_;
};

typedef ImageBMP<uint8_t> ImageBMP8Bit;
typedef ImageBMP<uint32_t> ImageBMP32Bit;

} // namespace compute_samples

#endif
