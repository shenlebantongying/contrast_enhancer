#include "wcag.hpp"
#include <gtest/gtest.h>

TEST(wcag1, BasicAssertions) {
  EXPECT_EQ(wcag::relative_luminance({0, 0, 0}), 0.0);
  EXPECT_EQ(wcag::relative_luminance({255, 255, 255}), 1.0);

  EXPECT_EQ(wcag::get_contrast_ratio({0, 0, 0}, {255, 255, 255}), 21);
  EXPECT_NEAR(wcag::get_contrast_ratio(QColor(0xffa348), QColor(0x986a44)),
              2.36, 0.01);
}