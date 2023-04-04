#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QButtonGroup>
#include <QMainWindow>

#include <QStyledItemDelegate>

#include "action.h"
#include "configurationhandler.h"
#include "streamdeckinterface.h"

class DeckHandler;

class QLabel;
class QMenu;
class QTreeWidget;
class QTreeWidgetItem;

class TreeItemDelegate : public QStyledItemDelegate {
 public:
  TreeItemDelegate(int height, QObject* parent = nullptr) : QStyledItemDelegate(parent), m_height(height) {}

  QSize sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const override {
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    if (m_height) {
      size.setHeight(m_height);
    }
    return size;
  }

 private:
  int m_height;
};

class MainWindow : public QMainWindow {
  Q_OBJECT
 public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  QWidget* createDeckButtons(StreamDeckInterface* deck);

 private slots:
  void deviceChanged(int index);
  void updateLabel(const QString& text);

  void showImageSelection();
  void adjustBrightness(int value);
  void showActionInfo();

  void deletePage();
  void previousPage();
  void nextPage();
  void addPage();

 private:
  void createActionItem(Action* action, QTreeWidgetItem*);

  void createBaseActions();

  DeckHandler* m_pDeckHandler;
  ConfigurationHandler* m_pConfigHandler;

  QLabel* m_pPageCountLabel;

  QHash<StreamDeckInterface*, QMap<int, QButtonGroup*>> m_buttonGroups;

  StreamDeckInterface* m_pCurrentDeck;

  QTreeWidget* m_pActionTree;
};
#endif  // MAINWINDOW_H
