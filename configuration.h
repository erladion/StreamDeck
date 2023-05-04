#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include "action.h"

#include <QList>
#include <QObject>

class QJsonObject;
class ConfigurationHandler;

class Configuration : public QObject {
  Q_OBJECT
 public:
  Configuration();
  Configuration(const Configuration& other);

  Configuration& operator=(Configuration other);

  QJsonObject toJson();
  static Configuration fromJson(QJsonObject obj);

  QList<Action*> pageActions(int index);

  void addPage(ConfigurationHandler* handler);
  void deletePage(int index);

  void addAction(int buttonIndex, int pageIndex, Action* action);

  QString name;
  int pageCount;
  int brightness;

 signals:
  void pageAdded();

 private:
  void swap(Configuration& other);

  QList<QList<Action*>> m_actions;
};
#endif  // CONFIGURATION_H
