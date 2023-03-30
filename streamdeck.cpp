#include "streamdeck.h"

#include <QBuffer>
#include <QByteArray>
#include <QFile>
#include <QImage>
#include <QList>

#include <QDebug>

#include <algorithm>

static const int MAX_STR = 255;
static const int POLL_RATE = 30;

static const unsigned int MICROSECONDS_PER_SECOND = 1000000;

StreamDeck::StreamDeck(hid_device* device) : m_device(device) {
  m_pageCount = 1;

  // Setup all interface variables
  ProductId = 0x80;

  // Size of the deck
  Rows = 3;
  Columns = 5;

  // Report Ids
  ImageReportId = 0x2;
  SendFeatureReportId = 0x3;
  SerialNumberReportId = 0x5;
  FirmwareReportId = 0x6;

  // Image
  ImageCommandId = 0x7;
  HeaderSize = 8;
  TotalSize = 1024;
  PayloadSize = TotalSize - HeaderSize;

  ResetCommandId = 0x2;
  BrightnessCommandId = 0x8;

  ReportSize = 32;

  ButtonStateHeaderSize = 0x4;

  wchar_t wstr[MAX_STR];
  hid_get_manufacturer_string(m_device, wstr, MAX_STR);
  Manufacturer = QString::fromWCharArray(wstr);

  hid_get_product_string(m_device, wstr, MAX_STR);
  Product = QString::fromWCharArray(wstr);

  QByteArray* serialNumberbuffer = new QByteArray(ReportSize, FirmwareReportId);
  hid_get_feature_report(m_device, reinterpret_cast<unsigned char*>(serialNumberbuffer->data()), ReportSize);
  SerialNumber = QString(serialNumberbuffer->data()).mid(2);

  QByteArray* firmwareVersionBuffer = new QByteArray(ReportSize, SerialNumberReportId);
  hid_get_feature_report(m_device, reinterpret_cast<unsigned char*>(firmwareVersionBuffer->data()), ReportSize);
  FirmwareVersion = QString(firmwareVersionBuffer->data()).mid(6);

  m_thread = new StreamDeckThread(this);

  connect(m_thread, &StreamDeckThread::buttonPressed, this,
          [this](int buttonId) { qInfo("Button pressed: %d", buttonId); });
  connect(m_thread, &StreamDeckThread::buttonReleased, this,
          [this](int buttonId) { qInfo("Button released: %d", buttonId); });

  m_thread->start();
}

void StreamDeck::setKeyImage(uint8_t keyId, const QImage& image) {
  QImage tmp = image.scaled(imageSize());  // Make sure the image is 72x72

  tmp = tmp.mirrored(true);

  // Write the image to a byte buffer
  QByteArray imageByteArray;
  QBuffer buf(&imageByteArray);
  buf.open(QIODevice::WriteOnly);
  tmp.save(&buf, "JPEG");

  uint16_t remainingBytes = imageByteArray.size();
  int iteration = 0;

  while (remainingBytes > 0) {
    uint16_t actualPayloadSize = std::min(remainingBytes, PayloadSize);

    QByteArray header;
    header.append(ImageReportId);
    header.append(ImageCommandId);
    header.append(keyId);
    header.append(((actualPayloadSize == remainingBytes) ? 0x01 : 0x0));
    header.append((actualPayloadSize & 0xff));
    header.append((actualPayloadSize >> 8));
    header.append((iteration & 0xff));
    header.append((iteration >> 8));

    QByteArray payload;
    payload.append(header);
    payload.append(imageByteArray, actualPayloadSize);
    imageByteArray.erase(imageByteArray.begin(), imageByteArray.begin() + actualPayloadSize);

    if (actualPayloadSize < PayloadSize) {
      QByteArray padding;
      padding = QByteArray(PayloadSize - actualPayloadSize, 0x0);
      payload.append(padding);
    }

    hid_write(m_device, reinterpret_cast<unsigned char*>(payload.data()), TotalSize);

    remainingBytes -= actualPayloadSize;
    iteration++;
  }
}

void StreamDeck::setBrightness(double percent) {
  int realPercent = percent;
  if (percent < 1.0) {
    realPercent = percent * 100.0;
  } else if (percent > 100.0) {
    realPercent = 100.0;
  }
  realPercent = std::min(std::max(realPercent, 0), 100);

  QByteArray payload(ReportSize, 0x0);

  payload[0] = SendFeatureReportId;
  payload[1] = BrightnessCommandId;
  payload[2] = static_cast<uint8_t>(realPercent);

  hid_send_feature_report(m_device, reinterpret_cast<unsigned char*>(payload.data()), ReportSize);
}

QSize StreamDeck::imageSize() {
  return QSize(72, 72);
}

void StreamDeck::reset() {
  QByteArray payload(ReportSize, 0x0);

  payload[0] = SendFeatureReportId;
  payload[1] = ResetCommandId;

  hid_send_feature_report(m_device, reinterpret_cast<unsigned char*>(payload.data()), ReportSize);
}

StreamDeckThread::StreamDeckThread(StreamDeck* deck) : m_deck(deck), m_device(deck->m_device) {}

void StreamDeckThread::run() {
  QByteArray buffer(m_deck->ButtonStateHeaderSize + m_deck->Columns * m_deck->Rows, 0x0);

  QList<bool> pressState(m_deck->Columns * m_deck->Rows);
  QList<bool> previousPressState(m_deck->Columns * m_deck->Rows);
  while (true) {
    hid_read(m_device, reinterpret_cast<unsigned char*>(buffer.data()),
             m_deck->ButtonStateHeaderSize + m_deck->Columns * m_deck->Rows);
    for (int i(0); i < m_deck->Columns * m_deck->Rows; ++i) {
      if (buffer.data()[i + m_deck->ButtonStateHeaderSize] == 1) {
        pressState[i] = true;
        emit buttonPressed(i);
      } else {
        pressState[i] = false;
        if (previousPressState[i] == true) {
          emit buttonReleased(i);
        }
      }
    }
    previousPressState = pressState;

    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = MICROSECONDS_PER_SECOND / POLL_RATE;
    select(0, nullptr, nullptr, nullptr, &tv);
  }
}
