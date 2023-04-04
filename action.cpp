#include "action.h"

#include <QImage>
#include <QString>

const int Action::ActionRole = Qt::UserRole + 1;

Action::Action() {}

Action::Action(const Action& other) {
  m_name = other.m_name;
  m_image = other.m_image;
}
