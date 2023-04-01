#ifndef STREAMDECKINTERFACE_H
#define STREAMDECKINTERFACE_H

#include <QObject>

namespace ProductIds {
static const uint16_t Mk2 = 0x80;
}

class StreamDeckInterface : public QObject {
  Q_OBJECT
 public:
  static const uint16_t VendorId = 0xfd9;

  QString Manufacturer;
  QString Product;
  QString SerialNumber;
  QString FirmwareVersion;

  virtual void setKeyImage(uint8_t keyId, const QImage& image) = 0;

  virtual void setBrightness(double percent) = 0;
  virtual void reset() = 0;

  virtual int getRows() = 0;
  virtual int getColums() = 0;

  virtual QSize imageSize() = 0;

 protected:
  uint16_t ProductId;

  int Rows;
  int Columns;

  uint8_t ImageReportId;
  uint8_t ImageCommandId;
  uint8_t HeaderSize;
  uint16_t TotalSize;
  uint16_t PayloadSize;

  uint8_t SerialNumberReportId;
  uint8_t FirmwareReportId;
  uint8_t SendFeatureReportId;
  uint8_t BrightnessCommandId;
  uint8_t ResetCommandId;

  uint8_t ReportSize;

  uint8_t ButtonStateHeaderSize;
};

#endif  // STREAMDECKINTERFACE_H
