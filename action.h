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

  ~Action() { /* m_pDeck does not need to be handled here */
  }

  virtual void execute() {}

  virtual QString name() { return m_name; }
  virtual QImage image() {
    if (m_image.isNull()) {
      return QImage();
    }
    return m_image;
  }

  virtual void setStreamDeck(StreamDeckInterface* deck) {
    if (deck == nullptr) {
      return;
    }
    m_pDeck = deck;
  }

  virtual Action& operator=(Action other);

  static const int ActionRole;

 protected:
  void swap(Action& other);

 protected:
  QString m_name;
  QImage m_image;

  StreamDeckInterface* m_pDeck;
};

Q_DECLARE_METATYPE(Action)

#endif  // ACTION_H
