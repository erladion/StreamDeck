#include "deckbutton.h"

#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
#include <QSize>

void DeckButton::showContextMenu(const QPoint& pos) {
  QMenu* menu = new QMenu;
  QAction* imageAction = menu->addAction("Set image");
  connect(imageAction, &QAction::triggered, this, &DeckButton::setImage);
  menu->exec(pos);
}

DeckButton::DeckButton(QSize buttonSize, QWidget* parent) : QPushButton(parent) {
  setMinimumSize(buttonSize);
  setMaximumSize(buttonSize);
}

void DeckButton::contextMenuEvent(QContextMenuEvent* event) {
  showContextMenu(event->globalPos());
  QPushButton::contextMenuEvent(event);
}
