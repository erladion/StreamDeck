#include "imageselectiondialog.h"

#include <QDir>
#include <QGridLayout>
#include <QPushButton>

ImageSelectionDialog::ImageSelectionDialog(QWidget* parent, Qt::WindowFlags f) : QDialog(parent, f) {
  imagePath = "";
  mainLayout = new QGridLayout();

  int count = 0;
  for (const QString& image : QDir(":/icons/").entryList()) {
    QString path = ":/icons/" + image;
    QPushButton* button = new QPushButton(QIcon(path), "");
    connect(button, &QPushButton::clicked, this, &ImageSelectionDialog::setReturnImage);
    button->setFlat(true);

    imagePaths.append(path);
    mainLayout->addWidget(button, count / 10, count);
    count++;
  }

  setLayout(mainLayout);
}

void ImageSelectionDialog::setReturnImage() {
  int index = mainLayout->indexOf(qobject_cast<QPushButton*>(sender()));

  imagePath = imagePaths.value(index);
  accept();
}
