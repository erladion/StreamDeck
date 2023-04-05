#ifndef BRIGHTNESSACTION_H
#define BRIGHTNESSACTION_H

#include "action.h"

class BrightnessAction : public Action {
  Q_OBJECT
 public:
  BrightnessAction();
  BrightnessAction(const QString& name, int value, const QImage& image);
  BrightnessAction(const BrightnessAction& other);

  ~BrightnessAction() {}

  BrightnessAction& operator=(const BrightnessAction& other);

  void execute() override;

 private:
  void swap(BrightnessAction& other);

  int m_value;
};

#endif  // BRIGHTNESSACTION_H
