#ifndef CONFIGURATIONHANDLER_H
#define CONFIGURATIONHANDLER_H

#include "action.h"
#include "streamdeckinterface.h"

#include <QHash>
#include <QObject>

class QJsonObject;

class Configuration : public QObject {
  Q_OBJECT
 public:
  Configuration();
  Configuration(const Configuration& other);

  Configuration& operator=(const Configuration& other);

  QJsonObject toJson();
  static Configuration fromJson(QJsonObject obj);

  void addPage();
  void deletePage(int index);

  void addAction(int buttonIndex, int pageIndex, Action* action);

  QString name;
  int pageCount;
  int brightness;

 private:
  void swap(Configuration& other);

  QList<QList<Action*>> m_actions;
};

class ConfigurationHandler : public QObject {
  Q_OBJECT
 public:
  ConfigurationHandler();

  void loadConfigurations();
  void saveConfiguration(Configuration config);

  void deviceChanged(StreamDeckInterface* deck);
  Configuration currentConfiguration() { return m_currentConfiguration; }

  void addPage();
  void deletePage();
  void nextPage();
  void previousPage();

  void adjustBrightness(int value);

  void setAction(int buttonIndex, Action* action, StreamDeckInterface* deck);

 public slots:
  void buttonPressed(int buttonIndex);
  void buttonReleased(int buttonIndex);

 private:
  StreamDeckInterface* m_pCurrentDeck;
  Configuration m_currentConfiguration;
  int m_currentPage;

  QList<Configuration> configurations;

  QHash<StreamDeckInterface*, Configuration> m_configurations;

  QString m_configPath;
};

#endif  // CONFIGURATIONHANDLER_H
