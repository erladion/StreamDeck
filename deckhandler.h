#ifndef DECKHANDLER_H
#define DECKHANDLER_H

#include <QObject>

#include <QSet>

#include "streamdeckinterface.h"

class DeckHandler : public QObject {
  Q_OBJECT
 public:
  explicit DeckHandler(QObject* parent = nullptr);

  QList<StreamDeckInterface*> devices() { return m_devices; }

 signals:

 private:
  QList<StreamDeckInterface*> m_devices;
};

#endif  // DECKHANDLER_H
