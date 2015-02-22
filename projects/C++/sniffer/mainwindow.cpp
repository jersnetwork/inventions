#ifndef _MAIN_CPP_
#define _MAIN_CPP_
#include <QToolBar>
#include <QTabWidget>
#include <QIcon>
#include <QAction>
#include <QStatusBar>
#include <QTextEdit>
#include <QString>
#include <string>
#include <algorithm>
#include <set>
#include <iostream>
#include "mainwindow.h"
#include "threading.h"
#include "packet.h"
#include "qtcpstack.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
  dialog = new DeviceDialog;

  QPixmap settings("settings.png");
  QPixmap save("save.png");
  QPixmap sniff("sniff.png");
  QPixmap connections("connections.png");

  QToolBar *toolbar = addToolBar("main mainwindow");
  QAction *save_act = toolbar->addAction(QIcon(save), "Save");
  QAction *settings_act = toolbar->addAction(QIcon(settings), "Settings");


  sniff_act = toolbar->addAction(QIcon(sniff), "Sniff");
  sniff_act->setEnabled(false);
  toolbar->addSeparator();
  toolbar->addAction(QIcon(connections), "Connections");
  centralWidget = new CenterWidget(this);

  setCentralWidget(centralWidget);
  statusBar()->showMessage("No network interface selected.");

  connect(sniff_act, SIGNAL(triggered()), centralWidget, SLOT(sniff()));
  connect(centralWidget, SIGNAL(toggle_sniffer_act(bool)),
          sniff_act, SLOT(setEnabled(bool)));

  connect(save_act, SIGNAL(triggered()), centralWidget, SLOT(save_data()));

  connect(settings_act, SIGNAL(triggered()), dialog, SLOT(on_click()));
  connect(dialog, SIGNAL(interface_selected(std::string)),
          this, SLOT(accept_interface(std::string)));

  connect(dialog, SIGNAL(interface_selected(std::string)),
          centralWidget, SLOT(accept_interface(std::string)));

  connect(dialog, SIGNAL(filter_selected(std::string)),
          centralWidget, SLOT(accept_filter(std::string)));
}

void MainWindow::accept_interface(std::string net) {
  statusBar()->showMessage(net.c_str());
  sniff_act->setEnabled(true);
}

#endif
