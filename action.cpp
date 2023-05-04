#include "action.h"

#include <QImage>
#include <QString>

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

Action& Action::operator=(Action other) {
  swap(other);
  return *this;
}

void Action::swap(Action& other) {
  std::swap(m_name, other.m_name);
  std::swap(m_image, other.m_image);
}
