#include "mainwindow.h"

#include <QComboBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "brightnessaction.h"
#include "deckbutton.h"
#include "deckhandler.h"
#include "imageselectiondialog.h"

static const int ActionRole = Qt::UserRole + 1;

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowTitle("Streamdeck");

  m_pDeckHandler = new DeckHandler();
  m_pConfigHandler = new ConfigurationHandler();
  m_pConfigHandler->loadConfigurations();

  // Configuration config;
  // config.name = "StreamDeckTest";
  // m_pConfigHandler->saveConfiguration(config);

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
  brightnessIcon->setPixmap(QPixmap(":/navigation/brightness24x24.png"));
  QSlider* brightnessSlider = new QSlider(Qt::Horizontal);
  brightnessSlider->setRange(0, 100);
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

  m_pActionTree = new QTreeWidget(this);
  m_pActionTree->setHeaderHidden(true);
  m_pActionTree->setMinimumWidth(300);
  m_pActionTree->setItemDelegate(new TreeItemDelegate(30));
  m_pActionTree->setRootIsDecorated(false);
  m_pActionTree->setDragEnabled(true);
  m_pActionTree->setDragDropMode(QAbstractItemView::DragOnly);

  blayout->addWidget(deckButtons);
  blayout->addLayout(pageLayout);
  blayout->addStretch();
  blayout->addLayout(actionInfoLayout);

  mainLayout->addLayout(blayout);
  mainLayout->addWidget(m_pActionTree);

  centralWidget->setLayout(mainLayout);
  setCentralWidget(centralWidget);

  createBaseActions();
  m_pActionTree->expandAll();
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
      DeckButton* button = new DeckButton(deck->imageSize());
      connect(button, &DeckButton::setImage, this, &MainWindow::setImage);
      connect(button, &DeckButton::clicked, this, &MainWindow::showActionInfo);

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

void MainWindow::showActionInfo() {}

void MainWindow::createActionItem(Action* action, QTreeWidgetItem* parent) {
  QTreeWidgetItem* item = new QTreeWidgetItem(parent, QStringList() << action->name());
  item->setData(0, ActionRole, QVariant::fromValue(action));
  item->setIcon(0, QPixmap::fromImage(action->image()));
}

void MainWindow::createBaseActions() {
  QTreeWidgetItem* brightnessItem = new QTreeWidgetItem(QStringList() << "Brightness");
  Action* increaseBrightnessAction = new BrightnessAction("Increase", 10, QImage(":/actions/brightness-increase.png"));
  Action* decreaseBrightnessAction = new BrightnessAction("Decrease", 10, QImage(":/actions/brightness-decrease.png"));
  createActionItem(increaseBrightnessAction, brightnessItem);
  createActionItem(decreaseBrightnessAction, brightnessItem);

  QTreeWidgetItem* root = m_pActionTree->invisibleRootItem();
  root->addChild(brightnessItem);
}
