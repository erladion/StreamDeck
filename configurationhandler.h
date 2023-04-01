#ifndef CONFIGURATIONHANDLER_H
#define CONFIGURATIONHANDLER_H

#include "streamdeckinterface.h"

#include <QHash>
#include <QObject>

class QJsonObject;

class Configuration : public QObject {
  Q_OBJECT
 public:
  Configuration();
  Configuration(const Configuration& other);

  void swap(Configuration& other);

  Configuration& operator=(const Configuration& other);

  QJsonObject toJson();

  static Configuration fromJson(QJsonObject obj);

  QString name;
  int pageCount;
};

class ConfigurationHandler : public QObject {
  Q_OBJECT
 public:
  ConfigurationHandler();

  void loadConfigurations();
  void saveConfiguration(Configuration config);

  Configuration currentConfiguration(StreamDeckInterface* deck) { return m_configurations.value(deck); }

 private:
  QList<Configuration> configurations;

  QHash<StreamDeckInterface*, Configuration> m_configurations;

  QString m_configPath;
};

#endif  // CONFIGURATIONHANDLER_H
