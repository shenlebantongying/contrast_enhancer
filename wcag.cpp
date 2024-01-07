#include "wcag.hpp"

namespace wcag {

qreal rev_lum_helper(qreal x) {
  return x < 0.04045 ? (x / 12.92) : (std::pow(((x + 0.055) / 1.055), 2.4));
};

qreal relative_luminance(const QColor &c) {
  auto R = rev_lum_helper(c.redF());
  auto G = rev_lum_helper(c.greenF());
  auto B = rev_lum_helper(c.blueF());
  return 0.2126 * R + 0.7152 * G + 0.0722 * B;
}

qreal contrast_ratio(qreal l1, qreal l2) {
  return l1 < l2 ? (l2 + 0.05) / (l1 + 0.05) : (l1 + 0.05) / (l2 + 0.05);
}

qreal get_contrast_ratio(QColor c1, QColor c2) {
  return contrast_ratio(relative_luminance(c1), relative_luminance(c2));
}

constexpr int red_helper(int i) {
  return std::max(0, i - std::max(1, int(i * 0.1)));
}

QColor reduceLuminance(QColor c) {
  c.setRed(red_helper(c.red()));
  c.setGreen(red_helper(c.green()));
  c.setBlue(red_helper(c.blue()));
  return c;
}

constexpr int inc_helper(int i) {
  return std::min(255, i + std::max(1, int((255 - i) * 0.1)));
}

QColor increaseLuminance(QColor c) {
  c.setRed(inc_helper(c.red()));
  c.setGreen(inc_helper(c.green()));
  c.setBlue(inc_helper(c.blue()));
  return c;
}

QColor get_foreground_with_enough_contrast(QColor fg, QColor bg, qreal ratio) {
  auto fgL = relative_luminance(fg);
  auto bgL = relative_luminance(bg);
  auto cr = contrast_ratio(fgL, bgL);
  while (cr < ratio) {
    if (fgL < bgL) {
      fg = reduceLuminance(fg);
      if (fg.red() == 0 && fg.green() == 0 && fg.blue() == 0) {
        break;
      }
    } else {
      fg = increaseLuminance(fg);
      if (fg.red() == 255 && fg.green() == 255 && fg.blue() == 255) {
        break;
      }
    }
    fgL = relative_luminance(fg);
    bgL = relative_luminance(bg);
    cr = contrast_ratio(fgL, bgL);
  }
  return fg;
}
} // namespace wcag