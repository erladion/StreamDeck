#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QList>
#include <QObject>

class Action;

class QJsonObject;
class ConfigurationHandler;

class Configuration : public QObject {
  Q_OBJECT
 public:
  Configuration();
  Configuration(const int deckRows, const int deckColumns);
  Configuration(const Configuration& other);

  Configuration& operator=(Configuration other);

  QJsonObject toJson() const;
  static Configuration fromJson(QJsonObject obj);

  QList<Action*> pageActions(int index) const;

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

  int m_deckRows;
  int m_deckColumns;
  int m_deckSize;
  QList<QList<Action*>> m_pages;
};
#endif  // CONFIGURATION_H
