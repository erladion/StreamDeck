#include "mainwindow.h"

#include "streamdeck.h"

#include <QGridLayout>
#include <QPushButton>
#include <QTreeWidget>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowTitle("Streamdeck");

  StreamDeck* deck = new StreamDeck();

  QWidget* centralWidget = new QWidget(this);

  QHBoxLayout* mainLayout = new QHBoxLayout(this);

  QVBoxLayout* blayout = new QVBoxLayout(this);

  QGridLayout* buttonLayout = new QGridLayout(this);
  for (int row(0); row < StreamDeckMk2::Rows; ++row) {
    for (int column(0); column < StreamDeckMk2::Columns; ++column) {
      QPushButton* button = new QPushButton();
      button->setMinimumSize(QSize(72, 72));
      button->setMaximumSize(QSize(72, 72));

      buttonLayout->addWidget(button, row, column);
    }
  }
  buttonLayout->setContentsMargins(20, 20, 20, 20);

  QTreeWidget* tw = new QTreeWidget(this);
  tw->setHeaderHidden(true);
  tw->setMinimumWidth(300);

  blayout->addLayout(buttonLayout);
  blayout->addStretch();

  mainLayout->addLayout(blayout);
  mainLayout->addWidget(tw);

  centralWidget->setLayout(mainLayout);
  setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {}
