#include "previouspageaction.h"

PreviousPageAction::PreviousPageAction(ConfigurationHandler* handler) : Action(), m_pHandler(handler) {
  m_name = QString();
  m_pDeck = nullptr;

  QImage image(":/actions/previous-page.png");
  image.invertPixels();

  m_image = image;
}

PreviousPageAction::PreviousPageAction(const PreviousPageAction& other) {
  m_name = other.m_name;
  m_image = other.m_image;
  m_pDeck = other.m_pDeck;
}

PreviousPageAction& PreviousPageAction::operator=(PreviousPageAction other) {
  swap(other);
  return *this;
}

void PreviousPageAction::execute() {
  m_pHandler->previousPage();
}

void PreviousPageAction::swap(PreviousPageAction& other) {
  std::swap(m_name, other.m_name);
  std::swap(m_image, other.m_image);
  std::swap(m_pDeck, other.m_pDeck);
}
