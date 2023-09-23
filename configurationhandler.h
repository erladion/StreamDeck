#ifndef CONFIGURATIONHANDLER_H
#define CONFIGURATIONHANDLER_H

#include <QHash>
#include <QObject>

#include "action.h"
#include "configuration.h"
#include "streamdeckinterface.h"

class ConfigurationHandler : public QObject {
  Q_OBJECT
 public:
  ConfigurationHandler();

  void loadConfigurations();
  void saveConfiguration(Configuration config);

  void newDevice(StreamDeckInterface* device);
  void deviceChanged(StreamDeckInterface* deck);
  Configuration* currentConfiguration() { return m_currentConfiguration; }

  int pageCount() { return m_currentConfiguration->pageCount; }

  void addPage();
  void deletePage();
  void nextPage();
  void previousPage();

  void adjustBrightness(int value);

  void setAction(int buttonIndex, Action* action, StreamDeckInterface* deck);

  void setActions(QList<Action*>);

 signals:
  void swapPage(QList<Action*>, int);
  void pageAdded(int, int);
  void updateCurrentPage(QList<Action*>, int);

 public slots:
  void buttonPressed(int buttonIndex);
  void buttonReleased(int buttonIndex);

 private slots:
  void sendPageUpdate();

 private:
  StreamDeckInterface* m_pCurrentDeck;
  Configuration* m_currentConfiguration;
  int m_currentPage;

  QList<Configuration> configurations;

  QHash<StreamDeckInterface*, Configuration*> m_configurations;

  QString m_configPath;
};

#endif  // CONFIGURATIONHANDLER_H
