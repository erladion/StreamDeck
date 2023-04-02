#ifndef ACTION_H
#define ACTION_H

#include <QImage>
#include <QObject>

class QString;
class QImage;

class Action : public QObject {
  Q_OBJECT
 public:
  Action();
  Action(const Action& other);

  ~Action() {}

  QString name() { return m_name; }
  QImage image() { return m_image; }

 protected:
  QString m_name;
  QImage m_image;
};

Q_DECLARE_METATYPE(Action)

#endif  // ACTION_H