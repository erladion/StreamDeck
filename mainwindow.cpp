#include "mainwindow.h"

#include <QComboBox>
#include <QDir>
#include <QLabel>
#include <QMenu>
#include <QSlider>
#include <QStackedWidget>
#include <QTreeWidget>

#include "deckhandler.h"

ImageSelectionDialog::ImageSelectionDialog(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f) {
  imagePath = "";
  mainLayout = new QGridLayout();

  int count = 0;
  for (const QString& image : QDir(":/icons/").entryList()) {
    QString path = ":/icons/" + image;
    QPushButton* button = new QPushButton(QIcon(path), "");
    connect(button, &QPushButton::clicked, this, &ImageSelectionDialog::setReturnImage);
    button->setFlat(true);

    imagePaths.append(path);
    mainLayout->addWidget(button, count / 10, count++);
  }

  setLayout(mainLayout);
}

void ImageSelectionDialog::setReturnImage() {
  int index = mainLayout->indexOf(qobject_cast<QPushButton*>(sender()));

  imagePath = imagePaths.value(index);
  accept();
}

void DeckButton::showContextMenu(const QPoint& pos) {
  QMenu* menu = new QMenu;
  QAction* imageAction = menu->addAction("Set image");
  connect(imageAction, &QAction::triggered, this, &DeckButton::setImage);
  menu->exec(pos);
}

DeckButton::DeckButton(QWidget* parent) : QPushButton(parent) {}

void DeckButton::contextMenuEvent(QContextMenuEvent* event) {
  showContextMenu(event->globalPos());
  QPushButton::contextMenuEvent(event);
}

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowTitle("Streamdeck");

  m_pDeckHandler = new DeckHandler();
  m_pConfigHandler = new ConfigurationHandler();

  Configuration config;
  config.name = "StreamDeckTest";
  m_pConfigHandler->saveConfiguration(config);

  m_pConfigHandler->loadConfigurations();

  QWidget* centralWidget = new QWidget(this);

  QHBoxLayout* mainLayout = new QHBoxLayout();
  QVBoxLayout* blayout = new QVBoxLayout();
  QHBoxLayout* decksLayout = new QHBoxLayout();
  QComboBox* decks = new QComboBox();
  decks->setMinimumWidth(150);
  decks->setMaximumWidth(150);
  decksLayout->setContentsMargins(20, 0, 20, 0);
  decksLayout->addWidget(decks);
  decksLayout->addStretch();

  QLabel* brightnessIcon = new QLabel();
  QPixmap brightnessPixmap(":/navigation/brightness24x24.png");
  brightnessIcon->setPixmap(brightnessPixmap);
  QSlider* brightnessSlider = new QSlider(Qt::Horizontal);
  brightnessSlider->setMinimum(0);
  brightnessSlider->setMaximum(100);
  connect(brightnessSlider, &QSlider::valueChanged, this, &MainWindow::adjustBrightness);
  decksLayout->addWidget(brightnessIcon);
  decksLayout->addWidget(brightnessSlider);

  blayout->addLayout(decksLayout);

  QStackedWidget* deckButtons = new QStackedWidget();

  connect(decks, &QComboBox::currentIndexChanged, deckButtons, &QStackedWidget::setCurrentIndex);
  connect(decks, &QComboBox::currentIndexChanged, this, &MainWindow::deviceChanged);

  m_pPageCountLabel = new QLabel("1");
  for (StreamDeckInterface* deck : m_pDeckHandler->devices()) {
    decks->addItem(deck->Product);
    deckButtons->addWidget(createDeckButtons(deck));
  }

  QHBoxLayout* pageLayout = new QHBoxLayout();
  QPushButton* deletePageButton = new QPushButton(QIcon(":/navigation/delete.png"), "");
  deletePageButton->setFlat(true);
  deletePageButton->setMaximumSize(QSize(25, 25));
  deletePageButton->setMinimumSize(QSize(25, 25));

  QPushButton* previousPageButton = new QPushButton(QIcon(":/navigation/left.png"), "");
  previousPageButton->setFlat(true);
  previousPageButton->setMaximumSize(QSize(25, 25));
  previousPageButton->setMinimumSize(QSize(25, 25));
  QPushButton* nextPageButton = new QPushButton(QIcon(":/navigation/right.png"), "");
  nextPageButton->setFlat(true);
  nextPageButton->setMaximumSize(QSize(25, 25));
  nextPageButton->setMinimumSize(QSize(25, 25));
  QPushButton* addPageButton = new QPushButton(QIcon(":/navigation/add.png"), "");
  addPageButton->setFlat(true);
  addPageButton->setMaximumSize(QSize(25, 25));
  addPageButton->setMinimumSize(QSize(25, 25));

  pageLayout->setAlignment(Qt::AlignHCenter);
  pageLayout->addWidget(deletePageButton);
  pageLayout->addWidget(previousPageButton);
  pageLayout->addWidget(m_pPageCountLabel);
  pageLayout->addWidget(nextPageButton);
  pageLayout->addWidget(addPageButton);

  QGridLayout* actionInfoLayout = new QGridLayout();

  QTreeWidget* tw = new QTreeWidget(this);
  tw->setHeaderHidden(true);
  tw->setMinimumWidth(300);

  blayout->addWidget(deckButtons);
  blayout->addLayout(pageLayout);
  blayout->addStretch();
  blayout->addLayout(actionInfoLayout);

  mainLayout->addLayout(blayout);
  mainLayout->addWidget(tw);

  centralWidget->setLayout(mainLayout);
  setCentralWidget(centralWidget);
}

MainWindow::~MainWindow() {}

QWidget* MainWindow::createDeckButtons(StreamDeckInterface* deck) {
  QImage image = QImage(":/icons/arrow.png");
  QWidget* widget = new QWidget();
  QGridLayout* buttonLayout = new QGridLayout();
  QButtonGroup* buttonGroup = new QButtonGroup();
  int count = 0;
  for (int row(0); row < deck->getRows(); ++row) {
    for (int column(0); column < deck->getColums(); ++column) {
      DeckButton* button = new DeckButton();
      connect(button, &DeckButton::setImage, this, &MainWindow::setImage);
      button->setMinimumSize(deck->imageSize());
      button->setMaximumSize(deck->imageSize());

      buttonLayout->addWidget(button, row, column);

      buttonGroup->addButton(button, count++);
    }
  }
  QMap<int, QButtonGroup*> map;
  map.insert(1, buttonGroup);
  m_buttonGroups.insert(deck, map);
  buttonLayout->setContentsMargins(20, 5, 20, 20);
  widget->setLayout(buttonLayout);
  return widget;
}

void MainWindow::deviceChanged(int index) {
  QList<StreamDeckInterface*> devices = m_pDeckHandler->devices();

  m_pCurrentDeck = devices.value(index);

  Configuration config = m_pConfigHandler->currentConfiguration(m_pCurrentDeck);
  updateLabel(QString("1/%0").arg(config.pageCount));
}

void MainWindow::updateLabel(const QString& text) {
  m_pPageCountLabel->setText(text);
}

void MainWindow::setImage() {
  DeckButton* button = qobject_cast<DeckButton*>(sender());

  int id = m_buttonGroups.value(m_pCurrentDeck).value(1)->id(button);
  ImageSelectionDialog* dialog = new ImageSelectionDialog(this, Qt::Dialog);
  dialog->setModal(true);
  QString image;
  if (dialog->exec() == QDialog::Accepted) {
    image = dialog->getImagePath();
  }

  if (!image.isNull()) {
    m_pCurrentDeck->setKeyImage(id, QImage(image));
    QPixmap p;
    p.convertFromImage(QImage(image));
    button->setIcon(QIcon(p));
  }
}

void MainWindow::adjustBrightness(int value) {
  m_pCurrentDeck->setBrightness(value);
}
