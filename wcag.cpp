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

qreal get_contrast_ratio(QColor &c1, QColor &c2) {
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

///
/// \return result color and final ratio reached
std::tuple<QColor, qreal> try_reduce_luminance_to_ratio(QColor fg, QColor &bg,
                                                        qreal ratio) {
  auto cr = get_contrast_ratio(fg, bg);
  while (cr < ratio) {
    fg = reduceLuminance(fg);
    if (fg.red() == 0 && fg.green() == 0 && fg.blue() == 0) {
      break;
    }
    cr = get_contrast_ratio(fg, bg);
  }
  return {fg, cr};
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

///
/// \return result color and final ratio reached
std::tuple<QColor, qreal> try_increase_luminance_to_ratio(QColor fg, QColor &bg,
                                                          qreal ratio) {
  auto cr = get_contrast_ratio(fg, bg);
  while (cr < ratio) {
    fg = increaseLuminance(fg);
    if (fg.red() == 255 && fg.green() == 255 && fg.blue() == 255) {
      break;
    }
    cr = get_contrast_ratio(fg, bg);
  }
  return {fg, cr};
}

QColor get_foreground_with_enough_contrast(QColor fg, QColor bg, qreal ratio) {
  auto fgL = relative_luminance(fg);
  auto bgL = relative_luminance(bg);
  auto cr = get_contrast_ratio(fg, bg);
  if (cr < ratio) {
    if (fgL < bgL) {
      auto [result_fg, result_cr] =
          try_reduce_luminance_to_ratio(fg, bg, ratio);

      // reduce luminance does not solve the problem, try another way around
      if (result_cr < ratio) {
        auto [result_fg_another, result_cr_another] =
            try_increase_luminance_to_ratio(fg, bg, ratio);
        return (result_cr_another > result_cr) ? result_fg_another : result_fg;
      } else {
        return result_fg;
      }
    } else {
      auto [result_fg, result_cr] =
          try_increase_luminance_to_ratio(fg, bg, ratio);

      if (result_cr < ratio) {
        auto [result_fg_another, result_cr_another] =
            try_reduce_luminance_to_ratio(fg, bg, ratio);
        return (result_cr_another > result_cr) ? result_fg_another : result_fg;
      } else {
        return result_fg;
      }
    }
  } else {
    return fg;
  }
  throw std::runtime_error("NO");
}
} // namespace wcag