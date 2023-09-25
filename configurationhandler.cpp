#include "configurationhandler.h"

#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>

#include "action.h"
#include "configuration.h"
#include "streamdeckinterface.h"

ConfigurationHandler::ConfigurationHandler() : m_currentPage(1) {
  m_configPath = QString("%0/.config/%1/configurations/").arg(qEnvironmentVariable("HOME"), QApplication::applicationName());

  m_currentConfiguration = nullptr;
}

void ConfigurationHandler::sendPageUpdate() { emit updateCurrentPage(m_currentConfiguration->pageActions(m_currentPage - 1), m_currentPage); }

void ConfigurationHandler::loadConfigurations() {
  QDirIterator dirIt(m_configPath, QDir::Files | QDir::NoDotAndDotDot);

  while (dirIt.hasNext()) {
    dirIt.next();

    QFile file(dirIt.filePath());

    if (file.open(QIODevice::ReadOnly)) {
      QByteArray bytes = file.readAll();
      file.close();

      QJsonParseError jsonError;
      QJsonDocument document = QJsonDocument::fromJson(bytes, &jsonError);
      if (jsonError.error != QJsonParseError::NoError) {
        qCritical() << jsonError.errorString();
        return;
      }

      if (document.isObject()) {
        QJsonObject obj = document.object();
        Configuration config = Configuration::fromJson(obj);
        configurations.append(config);
      }
    } else {
      qCritical() << file.errorString();
    }
  }
}

void ConfigurationHandler::saveConfiguration(Configuration config) {
  QDir dir;
  if (!dir.exists(m_configPath)) {
    dir.mkpath(m_configPath);
  }

  QJsonDocument doc;
  doc.setObject(config.toJson());
  QByteArray bytes = doc.toJson();
  QFile file(m_configPath + QString("%0.config").arg(config.name));
  if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
    QTextStream tStream(&file);
    tStream << bytes;
    file.close();
  } else {
    qCritical("Failed to open file: %s - %s", qPrintable(m_configPath + QString("%0.config").arg(config.name)), qPrintable(file.errorString()));
  }
}

Configuration* ConfigurationHandler::currentConfiguration() const { return m_currentConfiguration; }

void ConfigurationHandler::newDevice(StreamDeckInterface* device) {
  m_configurations.insert(device, new Configuration(device->rows(), device->colums()));
}

void ConfigurationHandler::deviceChanged(StreamDeckInterface* deck) {
  if (deck == nullptr) {
    return;
  }
  m_pCurrentDeck = deck;
  m_currentConfiguration = m_configurations.value(m_pCurrentDeck);
  connect(m_currentConfiguration, &Configuration::pageAdded, this, &ConfigurationHandler::sendPageUpdate);
}

int ConfigurationHandler::pageCount() const { return m_currentConfiguration->pageCount; }

void ConfigurationHandler::buttonPressed(int buttonIndex) {
  qWarning() << __FUNCTION__ << "Button index:" << buttonIndex;
  m_currentConfiguration->pageActions(m_currentPage - 1).value(buttonIndex)->execute();
}

void ConfigurationHandler::buttonReleased(int buttonIndex) {
  qWarning() << __FUNCTION__ << "Button index:" << buttonIndex;
  // m_currentConfiguration->pageActions(m_currentPage -
  // 1).value(buttonIndex)->execute();
}

void ConfigurationHandler::addPage() {
  qWarning() << metaObject()->className() << __FUNCTION__;
  m_currentConfiguration->addPage(this);
}

void ConfigurationHandler::deletePage() {
  qWarning() << metaObject()->className() << __FUNCTION__;
  m_currentConfiguration->deletePage(m_currentPage);
}

void ConfigurationHandler::nextPage() {
  qWarning() << metaObject()->className() << __FUNCTION__;
  m_currentPage++;
  if (m_currentPage > m_currentConfiguration->pageCount) {
    m_currentPage = m_currentConfiguration->pageCount;
  }

  QList<Action*> actions = m_currentConfiguration->pageActions(m_currentPage - 1);
  emit swapPage(actions, m_currentPage);
  setActions(actions);
}

void ConfigurationHandler::previousPage() {
  qWarning() << metaObject()->className() << __FUNCTION__;
  m_currentPage--;
  if (m_currentPage < 1) {
    m_currentPage = 1;
  }

  emit swapPage(m_currentConfiguration->pageActions(m_currentPage - 1), m_currentPage);
  setActions(m_currentConfiguration->pageActions(m_currentPage - 1));
}

void ConfigurationHandler::adjustBrightness(int value) const {
  m_currentConfiguration->brightness = value;
  m_pCurrentDeck->setBrightness(value);
}

void ConfigurationHandler::setAction(int buttonIndex, Action* action, StreamDeckInterface* deck) {
  if (action == nullptr) {
    return;
  }

  Configuration* currentConfiguration = m_configurations[deck];
  if (currentConfiguration != nullptr) {
    currentConfiguration->addAction(buttonIndex, m_currentPage, action);

    m_pCurrentDeck->setKeyImage(buttonIndex, action->image());
  }
}

void ConfigurationHandler::setActions(QList<Action*> actions) {
  for (int index(0); index < actions.size(); ++index) {
    m_pCurrentDeck->setKeyImage(index, actions.value(index)->image());
  }
}
