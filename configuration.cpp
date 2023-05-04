#include "configuration.h"

#include "nextpageaction.h"
#include "previouspageaction.h"

#include "configurationhandler.h"

#include <QJsonDocument>
#include <QJsonObject>

Configuration::Configuration() : pageCount(1) {
  QList<Action*> actions;
  for (int i(0); i < 15; ++i) {
    actions.append(new Action());
  }
  m_actions.append(actions);
  qWarning() << __FUNCTION__;
}

Configuration::Configuration(const Configuration& other) {
  name = other.name;
  pageCount = other.pageCount;
  brightness = other.brightness;
}

Configuration& Configuration::operator=(Configuration other) {
  swap(other);
  return *this;
}

void Configuration::swap(Configuration& other) {
  std::swap(name, other.name);
  std::swap(pageCount, other.pageCount);
  std::swap(brightness, other.brightness);
}

QJsonObject Configuration::toJson() {
  QJsonObject obj;
  obj.insert("name", name);
  obj.insert("pageCount", pageCount);
  obj.insert("brightness", brightness);

  return obj;
}

Configuration Configuration::fromJson(QJsonObject obj) {
  Configuration config;
  QVariantHash h = obj.toVariantHash();

  config.name = h.value("name").toString();
  config.pageCount = h.value("pageCount").toInt();
  config.brightness = h.value("brightness").toInt();

  return config;
}

QList<Action*> Configuration::pageActions(int index) {
  QList<Action*> actions = m_actions[index];
  return actions;
}

void Configuration::addPage(ConfigurationHandler* handler) {
  qWarning() << metaObject()->className() << __FUNCTION__;

  QList<Action*>& previousPage = m_actions.last();
  Action* a = previousPage[14];
  if (a != nullptr) {
    delete a;
  }
  previousPage.replace(14, new NextPageAction(handler));

  QList<Action*> actions;
  for (int i(0); i < 15; ++i) {
    actions.append(new Action());
  }
  actions[10] = new PreviousPageAction(handler);

  m_actions.append(actions);
  pageCount++;

  qWarning() << m_actions.size();
  qWarning() << actions.size();
  qWarning() << m_actions[pageCount - 1];

  emit pageAdded();
}

void Configuration::deletePage(int index) {
  qWarning() << metaObject()->className() << __FUNCTION__;
  m_actions.removeAt(index);
  pageCount--;
}

void Configuration::addAction(int buttonIndex, int pageIndex, Action* action) {
  qWarning() << metaObject()->className() << __FUNCTION__;
  m_actions[pageIndex - 1][buttonIndex] = action;
}
