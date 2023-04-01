#include "configurationhandler.h"

#include <QApplication>
#include <QDir>
#include <QDirIterator>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTextStream>

Configuration::Configuration() : pageCount(1) {}

Configuration::Configuration(const Configuration& other) {
  name = other.name;
  pageCount = other.pageCount;
}

void Configuration::swap(Configuration& other) {
  std::swap(name, other.name);
  std::swap(pageCount, other.pageCount);
}

Configuration& Configuration::operator=(const Configuration& other) {
  Configuration tmp(other);
  swap(tmp);
  return *this;
}

QJsonObject Configuration::toJson() {
  QJsonObject obj;
  obj.insert("name", name);
  obj.insert("pageCount", pageCount);

  return obj;
}

Configuration Configuration::fromJson(QJsonObject obj) {
  Configuration config;
  QVariantHash h = obj.toVariantHash();

  config.name = h.value("name").toString();
  config.pageCount = h.value("pageCount").toInt();

  return config;
}

ConfigurationHandler::ConfigurationHandler() {
  m_configPath =
      QString("%0/.config/%1/configurations/").arg(qEnvironmentVariable("HOME"), QApplication::applicationName());
}

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
    qCritical("Failed to open file: %s - %s", qPrintable(m_configPath + QString("%0.config").arg(config.name)),
              qPrintable(file.errorString()));
  }
}
