#ifndef DIALOG_H
#define DIALOG_H
#include <QtWidgets>
#include <QList>
#include <QString>
#include <string>
#include <vector>
#include "sniffer.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;

class DeviceDialog: public QDialog {
    Q_OBJECT

 public:
    DeviceDialog(QWidget *parent = 0);

 signals:
    void interface_selected(std::string);
    void filter_selected(std::string);
 private slots:
   void on_click();
   void assign_device(QAbstractButton* button);


 private:
   std::vector<std::string> device_list = Sniffer::list_devices();
   QList<QRadioButton> *devicelist;
   QString assigned_device;
   QLineEdit *filter;
};

#endif
