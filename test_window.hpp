#pragma once

#include <QColor>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QSlider>
#include <QVBoxLayout>
#include <QWidget>

#include "wcag.hpp"

struct testGroup {
  QColor foreground;
  QColor background;
};

class Drawing_board : public QWidget {
  Q_OBJECT

public:
  const qreal width = 500;
  const qreal chunk_height = 100;
  QList<testGroup> &contrastTests;
  qreal ratio;

  explicit Drawing_board(QWidget *p, QList<testGroup> &contrastTests)
      : QWidget(p), contrastTests(contrastTests) {
    this->setFixedWidth(width);
    ratio = 0;
  };

  const QString text = QStringLiteral(
      "Draws the given text within the provided rectangle. The rectangle "
      "along with alignment flags defines the anchors for the text.");

  void paintEvent(QPaintEvent *event) override {
    QPainter painter(this);

    for (int i = 0; i < contrastTests.size(); ++i) {
      painter.fillRect(QRectF{0, i * chunk_height, width, chunk_height},
                       contrastTests[i].background);
      painter.setPen(contrastTests[i].foreground);
      painter.drawText(QRectF{0, i * chunk_height, width / 2, chunk_height},
                       text);

      painter.setPen(wcag::get_foreground_with_enough_contrast(
          contrastTests[i].foreground, contrastTests[i].background, ratio));
      painter.drawText(
          QRectF{width / 2, i * chunk_height, width / 2, chunk_height}, text);
    }
  }
};

class Slider : public QWidget {
  Q_OBJECT

public:
  Slider(QWidget *parent, QString &&label, int max) : QWidget(parent) {
    setLayout(new QHBoxLayout());

    slider = new QSlider(Qt::Horizontal, this);
    slider->setTickInterval(1);
    slider->setTickPosition(QSlider::TicksBothSides);
    slider->setRange(0, max);

    number = new QLabel("0", this);

    layout()->addWidget(new QLabel(label, this));
    layout()->addWidget(number);
    layout()->addWidget(slider);

    connect(slider, &QSlider::sliderMoved,
            [this](int i) { this->number->setText(QString::number(i)); });
  };
  QLabel *number;
  QSlider *slider;
};

class test_window : public QWidget {
  Q_OBJECT
public:
  explicit test_window(QWidget *p) : QWidget(p) {

    contrastTests = new QList<testGroup>();
    this->setLayout(new QVBoxLayout(this));
    drawing_board = new Drawing_board(this, *contrastTests);

    layout()->addWidget(drawing_board);

    contrast_ratio_control =
        new Slider(this, QStringLiteral("contrast ratio"), 22);
    layout()->addWidget(contrast_ratio_control);
    connect(contrast_ratio_control->slider, &QSlider::sliderMoved,
            [this](int i) {
              drawing_board->ratio = i;
              drawing_board->update();
            });
  }

  void addContrastTest(QColor foreground, QColor background) const {
    contrastTests->push_back({foreground, background});
    drawing_board->setFixedHeight(drawing_board->chunk_height *
                                  contrastTests->count());
  }

  QList<testGroup> *contrastTests;

  Drawing_board *drawing_board;
  Slider *contrast_ratio_control;
};