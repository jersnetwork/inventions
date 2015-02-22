#ifndef _CENTER_CPP_
#define _CENTER_CPP_
#include <QToolBar>
#include <QTabWidget>
#include <QIcon>
#include <QAction>
#include <QStatusBar>
#include <QTextEdit>
#include <QString>
#include <QDir>
#include <string>
#include <algorithm>
#include <set>
#include <fstream>
#include <iostream>

#include "centerwidget.h"
#include "threading.h"
#include "packet.h"
#include "qtcpstack.h"
#include "sniffer.h"

CenterWidget::CenterWidget(QWidget *parent) : QWidget(parent) {
  sniffer = NULL;
  machine = NULL;
  main_loop = NULL;
  second_loop = NULL;
  machine = new QTcpStack();

  edit = new QTextEdit(this);
  tabs = new QTabWidget(edit);
  tabs->setFixedHeight(25);
  tabs->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  QVBoxLayout *vbox = new QVBoxLayout;
  vbox->addWidget(tabs);
  vbox->addWidget(edit);
  setLayout(vbox);

  connect(machine, SIGNAL(new_connections(std::vector<std::string>)),
            this, SLOT(new_tabs(std::vector<std::string>)));

  connect(tabs, SIGNAL(tabBarClicked(int)),
          this, SLOT(render(int)));
}

void CenterWidget::save_data() {
  std::ofstream myfile;
  int index = tabs->currentIndex();
  QString filename;
  QString socket = tabs->tabText(index);
  QFileDialog file_dialog(this);
  std::string data;

  data = machine->get_payload(std::string(socket.toUtf8().constData()));
  QString pwd = QDir::currentPath();
  filename = QFileDialog::getSaveFileName(this, QString("Save File"), pwd, QString(""), NULL, 0);

  myfile.open (filename.toUtf8().constData());
  myfile << data;
  myfile.close();
}

void CenterWidget::accept_interface(std::string net) {
  selected_dev = std::string(net.c_str());
}

void CenterWidget::accept_filter(std::string filter) {
  selected_filter = std::string(filter.c_str());
}

void CenterWidget::render(int index) {
  std::string socket(tabs->tabText(index).toUtf8().constData());
  std::string data(machine->get_payload(socket));

  edit->setPlainText(QString(data.c_str()));
}

void CenterWidget::new_tabs(std::vector<std::string> newtabs) {
  std::vector<std::string>::iterator itr;
  for(itr = newtabs.begin(); itr != newtabs.end(); itr++)
    //std::cout << itr->c_str() << std::endl;

    tabs->addTab(new QTextEdit(), itr->c_str());
  //QString *x = new QString(itr->c_str());
}

void CenterWidget::sniff() {
  emit toggle_sniffer_act(false);

  // construct sniffers
  if (sniffer == NULL)
    sniffer = new Sniffer(selected_dev, selected_filter);

  // construct thread
  if (main_loop == NULL) {
    main_loop = new StackBuilder<QTcpStack, Sniffer>(*machine, *sniffer, sys_lock, net_lock);
    main_loop->start();
    main_loop->detach();
  }

  if (second_loop == NULL) {
    second_loop = new StackWatcher<QTcpStack>(*machine, sys_lock);
    second_loop->start() ;
    second_loop->detach();
  }

  /*
  QTextCursor prev_cursor = edit->textCursor();
  myTextEdit->moveCursor (QTextCursor::End);
  myTextEdit->insertPlainText (myString);
  myTextEdit->setTextCursor (&prev_cursor);
  */
}
#endif
