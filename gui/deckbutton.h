#ifndef DECKBUTTON_H
#define DECKBUTTON_H

#include "action.h"

#include <QPushButton>

class QMenu;

class DeckButton : public QPushButton {
  Q_OBJECT
 public:
  DeckButton(int position, QSize buttonSize, QWidget* parent = nullptr);

  void setAction(Action* action);

 signals:
  void showImageSelection();
  void actionAdded(int, Action*);

 public slots:
  void showContextMenu(const QPoint& pos);
  void setImage(QImage image);
  void setImage(QIcon iconw);

 protected:
  void contextMenuEvent(QContextMenuEvent* event) override;

  void dragEnterEvent(QDragEnterEvent* event) override;
  void dragMoveEvent(QDragMoveEvent* event) override;
  void dropEvent(QDropEvent* event) override;

 private:
  QMenu* contextMenu;

  int m_position;
  QSize m_size;
};
#endif  // DECKBUTTON_H
