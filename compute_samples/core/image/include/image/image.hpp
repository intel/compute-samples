/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
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
  virtual int width() const = 0;
  virtual int height() const = 0;
  virtual int number_of_channels() const = 0;
  virtual int bits_per_channel() const = 0;
  virtual int bits_per_pixel() const = 0;
  virtual int size() const = 0;
  virtual int size_in_bytes() const = 0;
  virtual T get_pixel(const int x, const int y) const = 0;
  virtual void set_pixel(const int x, const int y, const T data) = 0;
  virtual std::vector<T> get_pixels() const = 0;
  virtual void copy_raw_data(const T *data) = 0;
  virtual T *raw_data() = 0;
  virtual const T *raw_data() const = 0;
};

template <typename T> class ImagePNG : public Image<T> {
public:
  ImagePNG();
  ImagePNG(const std::string &image_path);
  ImagePNG(const int width, const int height);
  ImagePNG(const int width, const int height, const std::vector<T> &data);
  bool read(const std::string &image_path) override;
  bool write(const std::string &image_path) override;
  bool write(const std::string &image_path, const T *data) override;
  int width() const override;
  int height() const override;
  int number_of_channels() const override;
  int bits_per_channel() const override;
  int bits_per_pixel() const override;
  int size() const override;
  int size_in_bytes() const override;
  T get_pixel(const int x, const int y) const override;
  void set_pixel(const int x, const int y, const T data) override;
  std::vector<T> get_pixels() const override;
  void copy_raw_data(const T *data) override;
  T *raw_data() override;
  const T *raw_data() const override;

  bool operator==(const ImagePNG &rhs) const;

private:
  std::vector<T> pixels_;
  int width_;
  int height_;
};

typedef ImagePNG<uint32_t> ImagePNG32Bit;

template <typename T> class ImageBMP : public Image<T> {
public:
  ImageBMP();
  ImageBMP(const std::string &image_path);
  ImageBMP(const int width, const int height);
  ImageBMP(const int width, const int height, const std::vector<T> &data);
  bool read(const std::string &image_path) override;
  bool write(const std::string &image_path) override;
  bool write(const std::string &image_path, const T *data) override;
  int width() const override;
  int height() const override;
  int number_of_channels() const override;
  int bits_per_channel() const override;
  int bits_per_pixel() const override;
  int size() const override;
  int size_in_bytes() const override;
  T get_pixel(const int x, const int y) const override;
  void set_pixel(const int x, const int y, const T data) override;
  std::vector<T> get_pixels() const override;
  void copy_raw_data(const T *data) override;
  T *raw_data() override;
  const T *raw_data() const override;

  bool operator==(const ImageBMP &rhs) const;

private:
  std::vector<T> pixels_;
  int width_;
  int height_;
};

typedef ImageBMP<uint8_t> ImageBMP8Bit;
typedef ImageBMP<uint32_t> ImageBMP32Bit;

template <typename T> int size_in_bytes(const Image<T> &i) {
  return i.size_in_bytes();
}

} // namespace compute_samples

#endif
