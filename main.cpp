#include <QApplication>

#include "mainwindow.h"
#include "phantomstyle.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);

  PhantomStyle ps;

  a.setStyle(&ps);

  a.setApplicationName("StreamDeck");

  MainWindow w;
  w.show();
  return a.exec();
}
