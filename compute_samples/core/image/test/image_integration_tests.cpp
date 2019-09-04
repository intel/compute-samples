/*
 * Copyright (C) 2019 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#include "image/image.hpp"
#include "gtest/gtest.h"

TEST(ImageIntegrationTests, ReadsPNGFile) {
  compute_samples::ImagePNG32Bit image("rgb_brg_3x2.png");
  const std::vector<uint32_t> pixels = {
      0xFF0000FF, //
      0x00FF00FF, //
      0x0000FFFF, //
      0x0000FFFF, //
      0xFF0000FF, //
      0x00FF00FF  //
  };
  EXPECT_EQ(image.get_pixels(), pixels);
}

TEST(ImageIntegrationTests, WritesPNGFile) {
  const std::vector<uint32_t> pixels = {
      0xFF0000FF, //
      0x00FF00FF, //
      0x0000FFFF, //
      0x0000FFFF, //
      0xFF0000FF, //
      0x00FF00FF  //
  };
  compute_samples::ImagePNG32Bit image(3, 2);
  image.write("output.png", pixels.data());

  compute_samples::ImagePNG32Bit output("output.png");
  EXPECT_EQ(output.get_pixels(), pixels);
  std::remove("output.png");

  EXPECT_EQ(image.get_pixels(), pixels);
}

TEST(ImageIntegrationTests, ReadsGrayscaleBMPFile) {
  compute_samples::ImageBMP8Bit image("kwkw_wwkk_4x2_mono.bmp");
  const std::vector<uint8_t> pixels = {
      0x00, //
      0xFF, //
      0x00, //
      0xFF, //
      0xFF, //
      0xFF, //
      0x00, //
      0x00  //
  };
  EXPECT_EQ(image.get_pixels(), pixels);
}

TEST(ImageIntegrationTests, WritesGrayscaleBMPFile) {
  const std::vector<uint8_t> pixels = {
      0x00, //
      0xFF, //
      0x00, //
      0xFF, //
      0xFF, //
      0xFF, //
      0x00, //
      0x00  //
  };
  compute_samples::ImageBMP8Bit image(4, 2);
  image.write("output.bmp", pixels.data());

  compute_samples::ImageBMP8Bit output("output.bmp");
  EXPECT_EQ(output.get_pixels(), pixels);
  std::remove("output.bmp");

  EXPECT_EQ(image.get_pixels(), pixels);
}

TEST(ImageIntegrationTests, ReadsColorBMPFile) {
  compute_samples::ImageBMP32Bit image("rgb_brg_3x2_argb.bmp");
  const std::vector<uint32_t> pixels = {
      0xFFFF0000, //
      0xFF00FF00, //
      0xFF0000FF, //
      0xFF0000FF, //
      0xFFFF0000, //
      0xFF00FF00  //
  };
  EXPECT_EQ(image.get_pixels(), pixels);
}

TEST(ImageIntegrationTests, WritesColorBMPFile) {
  const std::vector<uint32_t> pixels = {
      0xFFFF0000, //
      0xFF00FF00, //
      0xFF0000FF, //
      0xFF0000FF, //
      0xFFFF0000, //
      0xFF00FF00  //
  };
  compute_samples::ImageBMP32Bit image(3, 2);
  image.write("output.bmp", pixels.data());

  compute_samples::ImageBMP32Bit output("output.bmp");
  EXPECT_EQ(output.get_pixels(), pixels);
  std::remove("output.bmp");

  EXPECT_EQ(image.get_pixels(), pixels);
}
