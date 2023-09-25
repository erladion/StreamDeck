#include "mainwindow.h"

#include <QComboBox>
#include <QFile>
#include <QHBoxLayout>
#include <QLabel>
#include <QSlider>
#include <QSpacerItem>
#include <QStackedWidget>
#include <QTreeWidget>
#include <QVBoxLayout>

#include "action.h"
#include "brightnessaction.h"
#include "configuration.h"
#include "configurationhandler.h"
#include "deckbutton.h"
#include "deckhandler.h"
#include "imageselectiondialog.h"
#include "streamdeckinterface.h"

const QSize MainWindow::NavigationButtonsSize = QSize(25, 25);

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
  setWindowTitle("Streamdeck");

  m_pDeckHandler = new DeckHandler();
  m_pConfigHandler = new ConfigurationHandler();
  m_pConfigHandler->loadConfigurations();
  connect(m_pConfigHandler, &ConfigurationHandler::swapPage, this, &MainWindow::updatePage);
  connect(m_pConfigHandler, &ConfigurationHandler::updateCurrentPage, this, &MainWindow::updatePage);
  // connect(m_pConfigHandler, &ConfigurationHandler::pageAdded, this,
  // &MainWindow::updatePage);

  // Configuration config;
  // config.name = "StreamDeckTest";
  // m_pConfigHandler->saveConfiguration(config);

  QWidget* centralWidget = new QWidget(this);

  QHBoxLayout* mainLayout = new QHBoxLayout();
  QVBoxLayout* blayout = new QVBoxLayout();
  QHBoxLayout* decksLayout = new QHBoxLayout();

  QComboBox* decks = new QComboBox();
  decks->setFixedWidth(150);

  QLabel* brightnessIcon = new QLabel();
  brightnessIcon->setPixmap(QPixmap(":/navigation/brightness24x24.png"));
  QSlider* brightnessSlider = new QSlider(Qt::Horizontal);
  brightnessSlider->setRange(0, 100);
  connect(brightnessSlider, &QSlider::valueChanged, this, &MainWindow::adjustBrightness);

  decksLayout->setContentsMargins(20, 0, 20, 0);
  decksLayout->addWidget(decks);
  decksLayout->addStretch();
  decksLayout->addWidget(brightnessIcon);
  decksLayout->addWidget(brightnessSlider);
  blayout->addLayout(decksLayout);

  QStackedWidget* deckButtons = new QStackedWidget();

  m_pPageCountLabel = new QLabel("1");
  for (StreamDeckInterface* deck : m_pDeckHandler->devices()) {
    connect(deck, &StreamDeckInterface::buttonPressed, m_pConfigHandler, &ConfigurationHandler::buttonPressed);
    connect(deck, &StreamDeckInterface::buttonReleased, m_pConfigHandler, &ConfigurationHandler::buttonReleased);

    decks->addItem(deck->Product);
    deckButtons->addWidget(createDeckButtons(deck));
    m_pConfigHandler->newDevice(deck);
  }

  connect(decks, &QComboBox::currentIndexChanged, deckButtons, &QStackedWidget::setCurrentIndex);
  connect(decks, &QComboBox::currentIndexChanged, this, &MainWindow::deviceChanged);

  // m_pConfigHandler->deviceChanged(m_pDeckHandler->devices().value(decks->currentIndex()));

  deviceChanged(decks->currentIndex());

  QHBoxLayout* pageLayout = new QHBoxLayout();
  QPushButton* deletePageButton = new QPushButton(QIcon(":/navigation/delete.png"), "");
  deletePageButton->setFlat(true);
  deletePageButton->setFixedSize(NavigationButtonsSize);
  connect(deletePageButton, &QPushButton::clicked, this, &MainWindow::deletePage);
  QPushButton* previousPageButton = new QPushButton(QIcon(":/navigation/left.png"), "");
  previousPageButton->setFlat(true);
  previousPageButton->setFixedSize(NavigationButtonsSize);
  connect(previousPageButton, &QPushButton::clicked, this, &MainWindow::previousPage);
  QPushButton* nextPageButton = new QPushButton(QIcon(":/navigation/right.png"), "");
  nextPageButton->setFlat(true);
  nextPageButton->setFixedSize(NavigationButtonsSize);
  connect(nextPageButton, &QPushButton::clicked, this, &MainWindow::nextPage);
  QPushButton* addPageButton = new QPushButton(QIcon(":/navigation/add.png"), "");
  addPageButton->setFlat(true);
  addPageButton->setFixedSize(NavigationButtonsSize);
  connect(addPageButton, &QPushButton::clicked, this, &MainWindow::addPage);

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
  // QImage image = QImage(":/icons/arrow.png");
  QWidget* widget = new QWidget();
  QGridLayout* buttonLayout = new QGridLayout();
  m_pButtonGroup = new QButtonGroup();
  int count = 0;
  for (int row(0); row < deck->rows(); ++row) {
    for (int column(0); column < deck->colums(); ++column) {
      DeckButton* button = new DeckButton(row * 5 + column, deck->imageSize());
      connect(button, &DeckButton::showImageSelection, this, &MainWindow::showImageSelection);
      connect(button, &DeckButton::clicked, this, &MainWindow::showActionInfo);
      connect(button, &DeckButton::doAction, this, &MainWindow::doAction);
      connect(button, &DeckButton::actionAdded, this, &MainWindow::setAction);

      buttonLayout->addWidget(button, row, column);
      m_pButtonGroup->addButton(button, count++);
    }
  }
  buttonLayout->setContentsMargins(20, 5, 20, 20);

  buttonLayout->addItem(new QSpacerItem(0, 0), 5, 0);
  buttonLayout->addItem(new QSpacerItem(0, 0), 0, 3);
  widget->setLayout(buttonLayout);
  return widget;
}

void MainWindow::deviceChanged(int index) {
  m_pCurrentDeck = m_pDeckHandler->devices().value(index);
  m_pConfigHandler->deviceChanged(m_pCurrentDeck);

  Configuration* config = m_pConfigHandler->currentConfiguration();
  if (config != nullptr) {
    updateLabel(QString("1/%0").arg(config->pageCount));
  }
}

void MainWindow::updateLabel(const QString& text) { m_pPageCountLabel->setText(text); }

void MainWindow::showImageSelection() {
  DeckButton* button = qobject_cast<DeckButton*>(sender());

  ImageSelectionDialog* dialog = new ImageSelectionDialog(this, Qt::Dialog);
  QString image;
  if (dialog->exec() == QDialog::Accepted) {
    image = dialog->getImagePath();
  }

  if (!image.isNull()) {
    button->setImage(QImage(image));
  }
}

void MainWindow::adjustBrightness(int value) { m_pConfigHandler->adjustBrightness(value); }

void MainWindow::showActionInfo() {
  // TODO: Implement
}

void MainWindow::deletePage() { m_pConfigHandler->deletePage(); }

void MainWindow::previousPage() { m_pConfigHandler->previousPage(); }

void MainWindow::nextPage() { m_pConfigHandler->nextPage(); }

void MainWindow::addPage() { m_pConfigHandler->addPage(); }

void MainWindow::doAction() {
  if (sender() == nullptr) {
    return;
  }

  if (DeckButton* button = qobject_cast<DeckButton*>(sender())) {
    m_pConfigHandler->buttonPressed(button->position());
  }
}

void MainWindow::setAction(int index, Action* action) { m_pConfigHandler->setAction(index, action, m_pCurrentDeck); }

void MainWindow::updatePageLabel(int currentPage, int pageCount) {
  qWarning() << __FUNCTION__ << currentPage << pageCount;
  m_pPageCountLabel->setText(QString("%0/%1").arg(currentPage).arg(pageCount));
}

void MainWindow::updatePage(QList<Action*> actions, int currentPage) {
  if (actions.isEmpty()) {
    return;
  }

  updatePageLabel(currentPage, m_pConfigHandler->pageCount());

  for (int i(0); i < m_pCurrentDeck->colums() * m_pCurrentDeck->rows(); ++i) {
    DeckButton* button = qobject_cast<DeckButton*>(m_pButtonGroup->button(i));

    if (actions[i] == nullptr) {
      return;
    }

    button->setImage(actions[i]->image());
  }
}

void MainWindow::createActionItem(Action* action, QTreeWidgetItem* parent) {
  QTreeWidgetItem* item = new QTreeWidgetItem(parent, QStringList() << action->name());
  item->setData(0, Action::ActionRole, QVariant::fromValue(reinterpret_cast<quintptr>(action)));
  item->setIcon(0, QPixmap::fromImage(action->image()));
}

void MainWindow::createBaseActions() {
  QTreeWidgetItem* brightnessItem = new QTreeWidgetItem(QStringList() << "Brightness");
  QImage image(":/actions/brightness-increase.png");
  image.invertPixels();
  Action* increaseBrightnessAction = new BrightnessAction("Increase", 10, image);
  increaseBrightnessAction->setStreamDeck(m_pCurrentDeck);
  image = QImage(":/actions/brightness-decrease.png");
  image.invertPixels();
  Action* decreaseBrightnessAction = new BrightnessAction("Decrease", -10, image);
  createActionItem(increaseBrightnessAction, brightnessItem);
  createActionItem(decreaseBrightnessAction, brightnessItem);

  QTreeWidgetItem* root = m_pActionTree->invisibleRootItem();
  root->addChild(brightnessItem);
}
