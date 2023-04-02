#ifndef BRIGHTNESSACTION_H
#define BRIGHTNESSACTION_H

#include "action.h"

#include "streamdeckinterface.h"

class BrightnessAction : public Action {
 public:
  BrightnessAction();
  BrightnessAction(const QString& name, int value, const QImage& image);
  BrightnessAction(const BrightnessAction& other);

  ~BrightnessAction() {}

  BrightnessAction& operator=(const BrightnessAction& other);

  void setStreamDeck(StreamDeckInterface* deck) {
    if (deck == nullptr) {
      return;
    }
    m_pDeck = deck;
  }

 private:
  void swap(BrightnessAction& other);

  int m_value;

  StreamDeckInterface* m_pDeck;
};

#endif  // BRIGHTNESSACTION_H
