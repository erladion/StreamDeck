#ifndef PREVIOUSPAGEACTION_H
#define PREVIOUSPAGEACTION_H

#include "action.h"

class ConfigurationHandler;

class PreviousPageAction : public Action {
  Q_OBJECT
 public:
  PreviousPageAction(ConfigurationHandler* handler);
  PreviousPageAction(const PreviousPageAction& other);

  ~PreviousPageAction() {}

  PreviousPageAction& operator=(PreviousPageAction other);

  void execute() override;

 private:
  void swap(PreviousPageAction& other);

 private:
  ConfigurationHandler* m_pHandler;
};

#endif  // PREVIOUSPAGEACTION_H
