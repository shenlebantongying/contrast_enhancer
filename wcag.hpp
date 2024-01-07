#ifndef WCAG_H
#define WCAG_H

#include <QColor>
#include <QtTypes>

namespace wcag {

qreal relative_luminance(const QColor &c);
qreal get_contrast_ratio(QColor& c1, QColor& c2);

QColor get_foreground_with_enough_contrast(QColor fg, QColor bg, qreal ratio);

} // namespace wcag

#endif