#ifndef DECKBUTTON_H
#define DECKBUTTON_H

#include "action.h"

#include <QPushButton>

class QMenu;

class DeckButton : public QPushButton {
  Q_OBJECT
 public:
  DeckButton(QSize buttonSize, QWidget* parent = nullptr);

 signals:
  void setImage();

 public slots:
  void showContextMenu(const QPoint& pos);

 protected:
  void contextMenuEvent(QContextMenuEvent* event) override;

 private:
  QMenu* contextMenu;

  Action action;
};
#endif  // DECKBUTTON_H
