#ifndef STREAMDECK_H
#define STREAMDECK_H

#include <QObject>
#include <QThread>

#include "hidapi/hidapi.h"

#include "streamdeckinterface.h"

namespace StreamDeckMk2 {}  // namespace StreamDeckMk2

class StreamDeck;

class StreamDeckThread : public QThread {
  Q_OBJECT
 public:
  StreamDeckThread(StreamDeck* deck);

  void run() override;
 signals:
  void buttonPressed(int);
  void buttonReleased(int);

 private:
  StreamDeck* m_deck;
  hid_device* m_device;
};

class StreamDeck : public StreamDeckInterface {
  Q_OBJECT

  friend StreamDeckThread;

 public:
  StreamDeck(hid_device* device);

  virtual void setKeyImage(uint8_t keyId, const QImage& image) override;

  virtual void setBrightness(double percent) override;
  virtual void reset() override;

  virtual int getRows() override { return Rows; }
  virtual int getColums() override { return Columns; }

  virtual QSize imageSize() override;

 private:
  StreamDeckThread* m_thread;

  hid_device* m_device;
};

#endif  // STREAMDECK_H
