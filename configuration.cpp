#include "configuration.h"

#include <QJsonDocument>
#include <QJsonObject>

#include "action.h"
#include "configurationhandler.h"
#include "nextpageaction.h"
#include "previouspageaction.h"

Configuration::Configuration() : pageCount(0), m_deckSize(0), m_deckRows(0), m_deckColumns(0) {}

Configuration::Configuration(const int deckRows, const int deckColumns)
    : pageCount(1), m_deckSize(deckRows * deckColumns), m_deckRows(deckRows), m_deckColumns(deckColumns) {
  qWarning() << __FUNCTION__;
  QList<Action*> actions;
  for (int i(0); i < m_deckSize; ++i) {
    actions.append(new Action());
  }
  m_pages.append(actions);
}

Configuration::Configuration(const Configuration& other) {
  name = other.name;
  pageCount = other.pageCount;
  brightness = other.brightness;

  m_deckSize = other.m_deckSize;
  m_deckRows = other.m_deckRows;
  m_deckColumns = other.m_deckColumns;
}

Configuration& Configuration::operator=(Configuration other) {
  swap(other);
  return *this;
}

void Configuration::swap(Configuration& other) {
  std::swap(name, other.name);
  std::swap(pageCount, other.pageCount);
  std::swap(brightness, other.brightness);

  std::swap(m_deckSize, other.m_deckSize);
  std::swap(m_deckRows, other.m_deckRows);
  std::swap(m_deckColumns, other.m_deckColumns);
}

QJsonObject Configuration::toJson() const {
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

QList<Action*> Configuration::pageActions(int index) const {
  QList<Action*> actions = m_pages[index];
  return actions;
}

void Configuration::addPage(ConfigurationHandler* handler) {
  qWarning() << metaObject()->className() << __FUNCTION__;

  QList<Action*>& previousPage = m_pages.last();
  Action* a = previousPage[m_deckSize - 1];
  if (a != nullptr) {
    delete a;
  }
  previousPage.replace(m_deckSize - 1, new NextPageAction(handler));

  QList<Action*> actions;
  for (int i(0); i < m_deckSize; ++i) {
    actions.append(i == m_deckSize - m_deckColumns ? new PreviousPageAction(handler) : new Action());
  }

  m_pages.append(actions);
  pageCount++;

  qWarning() << m_pages.size();
  qWarning() << actions.size();
  qWarning() << m_pages[pageCount - 1];

  emit pageAdded();
}

void Configuration::deletePage(int index) {
  qWarning() << metaObject()->className() << __FUNCTION__ << index << m_pages.count();

  if (pageCount == 1) {
    return;
  }

  m_pages.removeAt(index - 1);
  pageCount--;
}

void Configuration::addAction(int buttonIndex, int pageIndex, Action* action) {
  qWarning() << metaObject()->className() << __FUNCTION__;
  m_pages[pageIndex - 1][buttonIndex] = action;
}
