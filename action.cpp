#include "action.h"

#include <QImage>
#include <QString>

Action::Action() {}

Action::Action(const Action& other) {
  m_name = other.m_name;
  m_image = other.m_image;
}
