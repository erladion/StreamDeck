#ifndef DECKBUTTON_H
#define DECKBUTTON_H

#include "action.h"
#include "streamdeckinterface.h"

#include <QPushButton>

class QMenu;

class DeckButton : public QPushButton {
  Q_OBJECT
 public:
  DeckButton(StreamDeckInterface* deck, int position, QSize buttonSize, QWidget* parent = nullptr);

  void setDeck(StreamDeckInterface* deck) { m_pDeck = deck; }

 signals:
  void showImageSelection();

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

  StreamDeckInterface* m_pDeck;
  int m_position;
  QSize m_size;

  Action* m_pAction;
};
#endif  // DECKBUTTON_H
