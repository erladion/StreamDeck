#include "deckbutton.h"

#include <QAction>
#include <QContextMenuEvent>
#include <QMenu>
#include <QSize>

#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QMimeData>

DeckButton::DeckButton(int position, QSize buttonSize, QWidget* parent)
    : QPushButton(parent), m_position(position), m_size(buttonSize) {
  setFixedSize(m_size);
  setIconSize(m_size / 2);
  setAcceptDrops(true);
}

void DeckButton::showContextMenu(const QPoint& pos) {
  QMenu* menu = new QMenu;
  QAction* imageAction = menu->addAction("Set image");
  connect(imageAction, &QAction::triggered, this, &DeckButton::showImageSelection);
  menu->exec(pos);
}

void DeckButton::setImage(QImage image) {
  QPixmap p;
  p.convertFromImage(image);
  setIcon(QIcon(p));
}

void DeckButton::setImage(QIcon icon) {
  setImage(icon.pixmap(m_size).toImage());
}

void DeckButton::contextMenuEvent(QContextMenuEvent* event) {
  showContextMenu(event->globalPos());
  QPushButton::contextMenuEvent(event);
}

void DeckButton::dragEnterEvent(QDragEnterEvent* event) {
  if (!event->mimeData()->hasUrls()) {
    QWidget::dragEnterEvent(event);
  }
  event->acceptProposedAction();
}

void DeckButton::dragMoveEvent(QDragMoveEvent* event) {}

void DeckButton::dropEvent(QDropEvent* event) {
  QByteArray d = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
  QDataStream stream(&d, QIODevice::ReadOnly);
  int row, col;
  QMap<int, QVariant> roleDataMap;
  stream >> row >> col >> roleDataMap;

  Action* action = reinterpret_cast<Action*>(roleDataMap.value(Action::ActionRole).value<quintptr>());
  setImage(roleDataMap.value(Qt::DecorationRole).value<QIcon>());

  emit actionAdded(m_position, action);
}
