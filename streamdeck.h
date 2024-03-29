#ifndef STREAMDECK_H
#define STREAMDECK_H

#include <QObject>
#include <QThread>

#ifndef _WIN32
#include "hidapi/hidapi.h"
#endif
#ifdef _WIN32
#include "hidapi/hidapi/hidapi.h"
#endif

#include "streamdeckinterface.h"

namespace StreamDeckMk2 {}  // namespace StreamDeckMk2

class StreamDeck;

class StreamDeckThread : public QThread {
  Q_OBJECT

  static const unsigned int MicrosecondsPerSecond;
  static const int PollRate;

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

  static const int MaxStringSize;

 public:
  StreamDeck(hid_device* device, const QString& serialNumber = QString());

  virtual void setKeyImage(uint8_t keyId, const QImage& image) override;

  virtual void setBrightness(double percent) override;
  virtual void reset() const override;

  virtual int rows() const override { return Rows; }
  virtual int colums() const override { return Columns; }

  virtual QSize imageSize() const override;

 private:
  StreamDeckThread* m_thread;

  hid_device* m_device;
};

#endif  // STREAMDECK_H
