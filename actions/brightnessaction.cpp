#include "brightnessaction.h"

BrightnessAction::BrightnessAction() {
  m_name = QString();
  m_pDeck = nullptr;
}

BrightnessAction::BrightnessAction(const QString& name, int value, const QImage& image) : m_value(value) {
  m_name = name;
  m_image = image;
  m_pDeck = nullptr;
}

BrightnessAction::BrightnessAction(const BrightnessAction& other) {
  m_name = other.m_name;
  m_image = other.m_image;
  m_value = other.m_value;
  m_pDeck = other.m_pDeck;
}

BrightnessAction& BrightnessAction::operator=(BrightnessAction other) {
  swap(other);
  return *this;
}

void BrightnessAction::swap(BrightnessAction& other) {
  std::swap(m_name, other.m_name);
  std::swap(m_image, other.m_image);
  std::swap(m_value, other.m_value);
  std::swap(m_pDeck, other.m_pDeck);
}

void BrightnessAction::execute() {
  if (m_pDeck) {
    m_pDeck->setBrightness(m_value);
  }
}
