#include "brightnessaction.h"

BrightnessAction::BrightnessAction() : m_pDeck(nullptr) {
  m_name = QString();
}

BrightnessAction::BrightnessAction(const QString& name, int value, const QImage& image)
    : m_value(value), m_pDeck(nullptr) {
  m_name = name;
  m_image = image;
}

BrightnessAction::BrightnessAction(const BrightnessAction& other) {
  m_name = other.m_name;
  m_image = other.m_image;
  m_value = other.m_value;
  m_pDeck = other.m_pDeck;
}

BrightnessAction& BrightnessAction::operator=(const BrightnessAction& other) {
  BrightnessAction tmp(other);

  swap(tmp);
  return *this;
}

void BrightnessAction::swap(BrightnessAction& other) {
  std::swap(m_name, other.m_name);
  std::swap(m_image, other.m_image);
  std::swap(m_value, other.m_value);
  std::swap(m_pDeck, other.m_pDeck);
}
