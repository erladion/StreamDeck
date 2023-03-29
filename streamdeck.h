#ifndef STREAMDECK_H
#define STREAMDECK_H

#include <QObject>
#include <QThread>

#include "hidapi/hidapi.h"

namespace StreamDeckMk2 {
const int Rows = 3;
const int Columns = 5;
}  // namespace StreamDeckMk2

class StreamDeckThread : public QThread {
  Q_OBJECT
 public:
  StreamDeckThread(hid_device* device);

  void run() override;
 signals:
  void buttonPressed(int);
  void buttonReleased(int);

 private:
  hid_device* m_device;
};

class StreamDeck : public QObject {
  Q_OBJECT

  const uint16_t VendorId = 0xfd9;
  const uint16_t ProductId = 0x80;

  const uint8_t ImageReportId = 0x2;
  const uint8_t HeaderSize = 8;
  const uint16_t TotalSize = 1024;
  const uint16_t PayloadSize = TotalSize - HeaderSize;

  const uint16_t SerialNumberReportId = 0x5;
  const uint16_t FirmwareReportId = 0x6;
  const uint16_t ReportSize = 32;

 public:
  StreamDeck();

  void setKeyImage(uint8_t keyId, const QImage& image);

 private:
  StreamDeckThread* m_thread;

  QString m_serialNumber;
  QString m_firmwareVersion;

  hid_device* m_device;
};

#endif  // STREAMDECK_H
