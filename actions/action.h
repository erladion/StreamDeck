#ifndef ACTION_H
#define ACTION_H

#include <QImage>
#include <QObject>

class StreamDeckInterface;

class QString;

class Action : public QObject {
  Q_OBJECT
 public:
  Action();
  Action(const Action& other);

  ~Action() { /* m_pDeck does not need to be handled here */
  }

  virtual void execute() {}

  virtual QString name();
  virtual QImage image();

  virtual void setStreamDeck(StreamDeckInterface* deck);

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
