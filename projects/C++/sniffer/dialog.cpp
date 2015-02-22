#include <QtWidgets>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QButtonGroup>
#include <QLineEdit>
#include <iostream>
#include <string>
#include "dialog.h"

void DeviceDialog::assign_device(QAbstractButton* button) {
  assigned_device = button->text();
}

void DeviceDialog::on_click() {
  if ( exec() == QDialog::Accepted ) {
    emit interface_selected(assigned_device.toUtf8().constData());
    emit filter_selected(filter->text().toUtf8().constData());
  }
}

DeviceDialog::DeviceDialog(QWidget *parent) : QDialog(parent) {
  std::vector<std::string>::iterator it;
  filter = new QLineEdit(this);
  QLabel *label = new QLabel(this);
  label->setText("Filter:");
  QHBoxLayout *filterLayout = new QHBoxLayout;
  filterLayout->addWidget(label);
  filterLayout->addWidget(filter);

  QButtonGroup* button_group = new QButtonGroup(this);
  QGroupBox* groupBox = new QGroupBox();
  QVBoxLayout *vbox = new QVBoxLayout;

  for(it = device_list.begin(); it != device_list.end(); ++it) {
    QRadioButton* r = new QRadioButton(it->c_str());
    button_group->addButton(r);
    vbox->addWidget(r);
  }
  groupBox->setLayout(vbox);

  QVBoxLayout *leftLayout = new QVBoxLayout;
  leftLayout->addWidget(groupBox);
  QPushButton *okButton = new QPushButton(tr("&Ok"));
  QPushButton *cancelButton = new QPushButton(tr("Cancel"));
  cancelButton->setDefault(true);

  QVBoxLayout *rightLayout = new QVBoxLayout;
  rightLayout->addWidget(okButton);
  rightLayout->addWidget(cancelButton);

  QHBoxLayout *netLayout = new QHBoxLayout;
  netLayout->addLayout(leftLayout);
  netLayout->addLayout(rightLayout);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(filterLayout);
  mainLayout->addLayout(netLayout);
  setLayout(mainLayout);
  setWindowTitle(tr("Set Interface"));
  setFixedHeight(sizeHint().height());

  connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
  connect(button_group, SIGNAL(buttonClicked(QAbstractButton*)),
          this, SLOT(assign_device(QAbstractButton*)));

}
