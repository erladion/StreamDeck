#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QButtonGroup>
#include <QMainWindow>
#include <QStyledItemDelegate>

class StreamDeckInterface;
class ConfigurationHandler;
class Action;
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

 signals:
  void buttonPressed(int buttonIndex);
  void buttonReleased(int buttonIndex);

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

  void doAction();
  void setAction(int, Action*);

 private slots:
  void updatePageLabel(int currentPage, int pageCount);

  void updatePage(QList<Action*> actions, int currentPage);

 private:
  static const QSize NavigationButtonsSize;

  void createActionItem(Action* action, QTreeWidgetItem*);

  void createBaseActions();

  DeckHandler* m_pDeckHandler;
  ConfigurationHandler* m_pConfigHandler;

  QLabel* m_pPageCountLabel;

  QButtonGroup* m_pButtonGroup;

  StreamDeckInterface* m_pCurrentDeck;

  QTreeWidget* m_pActionTree;
};
#endif  // MAINWINDOW_H
