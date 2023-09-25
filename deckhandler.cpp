#include "deckhandler.h"

#include "streamdeck.h"
#include "streamdeckinterface.h"

#ifndef _WIN32
#include "hidapi/hidapi.h"
#endif
#ifdef _WIN32
#include "hidapi/hidapi/hidapi.h"
#endif

#include <QDebug>

DeckHandler::DeckHandler(QObject* parent) : QObject{parent} {
  // Get all Elgato devicse
  hid_device_info* devices = hid_enumerate(StreamDeckInterface::VendorId, 0x0);

  for (hid_device_info* currentDevice = devices; currentDevice != nullptr; currentDevice = currentDevice->next) {
    unsigned short productId = currentDevice->product_id;
    wchar_t* serialNumber = currentDevice->serial_number;

    hid_device* device = hid_open(StreamDeckInterface::VendorId, productId, serialNumber);
    if (!device) {
      printf("Unable to open device\n");
      hid_exit();
      return;
    }

    StreamDeckInterface* deck = nullptr;
    // Create the appropriate deck if it can be handled
    if (productId == ProductIds::Mk2) {
      deck = new StreamDeck(device);
    }

    if (deck) {
      m_devices.append(deck);
    }
  }

  hid_free_enumeration(devices);
}
