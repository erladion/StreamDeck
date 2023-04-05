#ifndef ACTION_H
#define ACTION_H

#include <QImage>
#include <QObject>

#include "streamdeckinterface.h"

class QString;

class Action : public QObject {
  Q_OBJECT
 public:
  Action();
  Action(const Action& other);

  ~Action() { /* m_pDeck does not need to be handled here */ }

  virtual void execute() = 0;

  QString name() { return m_name; }
  QImage image() { return m_image; }

  void setStreamDeck(StreamDeckInterface* deck) {
    if (deck == nullptr) {
      return;
    }
    m_pDeck = deck;
  }

  static const int ActionRole;

 protected:
  QString m_name;
  QImage m_image;

  StreamDeckInterface* m_pDeck;
};

Q_DECLARE_METATYPE(Action)

#endif  // ACTION_H
