#ifndef _CENTER_CPP_
#define _CENTER_CPP_
#include <unistd.h>

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

#include "centerwidget.h"
#include "threading.h"
#include "packet.h"
#include "qtcpstack.h"
#include "sniffer.h"

CenterWidget::CenterWidget(QWidget *parent) : QWidget(parent), sniffer(NULL), machine(NULL) {
  num_cores = sysconf(_SC_NPROCESSORS_ONLN);
  threads.reserve(num_cores);

  machine = new QTcpStack();
  QPalette* palette = new QPalette();
  palette->setColor(QPalette::Base, Qt::black);
  edit = new QTextEdit(this);
  edit->setTextColor( QColor( "green" ) );
  edit->setPalette(*palette);

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

CenterWidget::~CenterWidget() {
  std::vector<threading::thread*>::iterator itr;
  for(itr = threads.begin(); itr!= threads.end(); itr++)
    delete *itr;
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
    tabs->addTab(new QTextEdit(), itr->c_str());
}

void CenterWidget::sniff() {
  emit toggle_sniffer_act(false);
  std::vector<threading::thread*>::iterator itr;

  // construct sniffers
  if (sniffer == NULL)
    sniffer = new Sniffer(selected_dev, selected_filter);

  // construct threads
  if (threads.empty()) {
    for(int i = 0; i < num_cores; i++)
      threads.push_back(new StackBuilder<QTcpStack, Sniffer>(*machine, *sniffer, sys_lock, net_lock, poll_cond));
    threads.push_back(new TabCreator<QTcpStack>(*machine, sys_lock, poll_cond, poll_lock));

    for(itr = threads.begin(); itr!= threads.end(); itr++)
      (*itr)->start();

    for(itr = threads.begin(); itr!= threads.end(); itr++)
      (*itr)->detach();
  }
}
#endif
