#ifndef DECKHANDLER_H
#define DECKHANDLER_H

#include <QObject>
#include <QSet>

class StreamDeckInterface;

class DeckHandler : public QObject {
  Q_OBJECT
 public:
  explicit DeckHandler(QObject* parent = nullptr);

  QList<StreamDeckInterface*> devices() const { return m_devices; }

 private:
  QList<StreamDeckInterface*> m_devices;
};

#endif  // DECKHANDLER_H
