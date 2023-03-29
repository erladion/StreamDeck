#include "streamdeck.h"

#include <QBuffer>
#include <QByteArray>
#include <QFile>
#include <QImage>
#include <QList>

#include <algorithm>

static const int MAX_STR = 255;
static const int POLL_RATE = 30;

static const unsigned int MICROSECONDS_PER_SECOND = 1000000;

StreamDeck::StreamDeck() {
  wchar_t wstr[MAX_STR];

  m_device = hid_open(VendorId, ProductId, nullptr);
  if (!m_device) {
    printf("Unable to open device\n");
    hid_exit();
    return;
  }

  hid_get_manufacturer_string(m_device, wstr, MAX_STR);
  QString manufacturer = QString::fromWCharArray(wstr);

  // Read the Product String
  hid_get_product_string(m_device, wstr, MAX_STR);
  QString product = QString::fromWCharArray(wstr);

  QByteArray* serialNumberbuffer = new QByteArray(ReportSize, FirmwareReportId);
  hid_get_feature_report(m_device, reinterpret_cast<unsigned char*>(serialNumberbuffer->data()), ReportSize);
  m_serialNumber = QString(serialNumberbuffer->data()).mid(2);

  QByteArray* firmwareVersionBuffer = new QByteArray(ReportSize, SerialNumberReportId);
  hid_get_feature_report(m_device, reinterpret_cast<unsigned char*>(firmwareVersionBuffer->data()), ReportSize);
  m_firmwareVersion = QString(firmwareVersionBuffer->data()).mid(6);

  m_thread = new StreamDeckThread(m_device);

  connect(m_thread, &StreamDeckThread::buttonPressed, this,
          [this](int buttonId) { qInfo("Button pressed: %d", buttonId); });
  connect(m_thread, &StreamDeckThread::buttonReleased, this,
          [this](int buttonId) { qInfo("Button released: %d", buttonId); });

  m_thread->start();
}

void StreamDeck::setKeyImage(uint8_t keyId, const QImage& image) {
  QImage tmp = image.scaled(72, 72);  // Make sure the image is 72x72

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
    header.append(0x07);
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

StreamDeckThread::StreamDeckThread(hid_device* device) : m_device(device) {}

void StreamDeckThread::run() {
  QByteArray* buffer = new QByteArray(4 + StreamDeckMk2::Columns * StreamDeckMk2::Rows, '0');

  QList<bool> pressState(StreamDeckMk2::Columns * StreamDeckMk2::Rows);
  QList<bool> previousPressState(StreamDeckMk2::Columns * StreamDeckMk2::Rows);
  while (true) {
    hid_read(m_device, reinterpret_cast<unsigned char*>(buffer->data()),
             4 + StreamDeckMk2::Columns * StreamDeckMk2::Rows);
    for (int i(0); i < StreamDeckMk2::Columns * StreamDeckMk2::Rows; ++i) {
      if (buffer->data()[i + 4] == 1) {
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

  delete buffer;
}
