#include "test_window.hpp"
#include <QApplication>

int main(int argc, char **argv) {
  QApplication app(argc, argv);

  test_window t(nullptr);

  t.addContrastTest({235, 245, 255}, {255, 255, 255});
  t.addContrastTest({3, 3, 30}, {0, 0, 0});
  t.addContrastTest({230, 200, 180}, {253, 246, 227});

  t.show();

  return QApplication::exec();
}
