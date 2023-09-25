#ifndef NEXTPAGEACTION_H
#define NEXTPAGEACTION_H

#include "action.h"

class ConfigurationHandler;

class NextPageAction : public Action {
  Q_OBJECT
 public:
  NextPageAction(ConfigurationHandler* handler);
  NextPageAction(const NextPageAction& other);

  ~NextPageAction() {}

  NextPageAction& operator=(NextPageAction other);

  void execute() override;

 private:
  void swap(NextPageAction& other);

 private:
  ConfigurationHandler* m_pHandler;
};

#endif  // NEXTPAGEACTION_H
