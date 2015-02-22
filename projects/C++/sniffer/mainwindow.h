#ifndef MAIN_H
#define MAIN_H

#include <QMainWindow>
#include <QApplication>
#include "dialog.h"
#include "centerwidget.h"

class MainWindow: public QMainWindow {
  Q_OBJECT

 private:
  QAction *sniff_act;
  DeviceDialog *dialog;
  CenterWidget *centralWidget;

  private slots:
    void accept_interface(std::string);

    // stop button should destroy the sniffer

  public:
    MainWindow(QWidget *parent = 0);
};
#endif
