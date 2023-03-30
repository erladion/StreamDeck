#include "deckhandler.h"

#include "streamdeck.h"

#include "hidapi/hidapi.h"

#include <QDebug>

DeckHandler::DeckHandler(QObject* parent) : QObject{parent} {
  /*
  struct hid_device_info* devs = hid_enumerate(StreamDeckInterface::VendorId, 0x0);

  struct hid_device_info* currentDevice = devs;

  while (currentDevice) {
    unsigned short productId = currentDevice->product_id;
    wchar_t* serialNumber = currentDevice->serial_number;


  }*/

  hid_device* device = hid_open(StreamDeckInterface::VendorId, 0x80, nullptr);
  if (!device) {
    printf("Unable to open device\n");
    hid_exit();
    return;
  }

  StreamDeck* deck = new StreamDeck(device);

  m_devices.append(deck);

  qWarning() << m_devices.size();

  // hid_free_enumeration(devs);
}
