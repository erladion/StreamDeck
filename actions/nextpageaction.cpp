#include "nextpageaction.h"

NextPageAction::NextPageAction(ConfigurationHandler* handler) : Action(), m_pHandler(handler) {
  m_name = QString();
  m_pDeck = nullptr;

  QImage image(":/actions/next-page.png");
  image.invertPixels();

  m_image = image;
}

NextPageAction::NextPageAction(const NextPageAction& other) {
  m_name = other.m_name;
  m_image = other.m_image;
  m_pDeck = other.m_pDeck;
}

NextPageAction& NextPageAction::operator=(NextPageAction other) {
  swap(other);
  return *this;
}

void NextPageAction::execute() {
  m_pHandler->nextPage();
}

void NextPageAction::swap(NextPageAction& other) {
  std::swap(m_name, other.m_name);
  std::swap(m_image, other.m_image);
  std::swap(m_pDeck, other.m_pDeck);
}
