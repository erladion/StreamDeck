#ifndef IMAGESELECTIONDIALOG_H
#define IMAGESELECTIONDIALOG_H

#include <QDialog>
#include <QList>

class QGridLayout;

class ImageSelectionDialog : public QDialog {
  Q_OBJECT
 public:
  ImageSelectionDialog(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

  QString getImagePath() { return imagePath; }

 public slots:
  void setReturnImage();

 private:
  QGridLayout* mainLayout;

  QList<QString> imagePaths;

  QString imagePath;
};

#endif  // IMAGESELECTIONDIALOG_H
