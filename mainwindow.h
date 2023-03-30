#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QButtonGroup>
#include <QDialog>
#include <QGridLayout>
#include <QMainWindow>
#include <QMouseEvent>
#include <QPushButton>

#include "streamdeckinterface.h"

class DeckHandler;

class QLabel;
class QMenu;

class ImageSelectionDialog : public QDialog {
  Q_OBJECT
 public:
  ImageSelectionDialog();

  QString getImagePath() { return imagePath; }

 public slots:
  void setReturnImage();

 private:
  QGridLayout* mainLayout;

  QList<QString> imagePaths;

  QString imagePath;
};

class DeckButton : public QPushButton {
  Q_OBJECT
 public:
  DeckButton(QWidget* parent = nullptr);

 signals:
  void setImage();

 public slots:
  void showContextMenu(const QPoint& pos);

 protected:
  void contextMenuEvent(QContextMenuEvent* event) override;

 private:
  QMenu* contextMenu;
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

  void setImage();

 private:
  DeckHandler* m_pDeckHandler;

  QLabel* m_pPageCountLabel;

  QHash<StreamDeckInterface*, QMap<int, QButtonGroup*>> m_buttonGroups;
};
#endif  // MAINWINDOW_H
