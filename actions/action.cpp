#include "action.h"

#include <QImage>
#include <QString>

#include "streamdeckinterface.h"

const int Action::ActionRole = Qt::UserRole + 1;

Action::Action() {
  QImage image(72, 72, QImage::Format_RGB32);
  image.fill(Qt::black);
  m_image = image;
}

Action::Action(const Action& other) {
  m_name = other.m_name;
  m_image = other.m_image;
}

QString Action::name() { return m_name; }

QImage Action::image() {
  if (m_image.isNull()) {
    QImage image = QImage();
    // image.fill(QColor(0, 0, 0, 0));
    return image;
  }
  return m_image;
}

void Action::setStreamDeck(StreamDeckInterface *deck) {
  if (deck == nullptr) {
    return;
  }
  m_pDeck = deck;
}

Action& Action::operator=(Action other) {
  swap(other);
  return *this;
}

void Action::swap(Action& other) {
  std::swap(m_name, other.m_name);
  std::swap(m_image, other.m_image);
}
