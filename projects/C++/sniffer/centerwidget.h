#ifndef _CENTER_H_
#define _CENTER_H_

#include "dialog.h"
#include "sniffer.h"
#include "qtcpstack.h"
#include "thread_decs.cpp"

class CenterWidget: public QWidget {
  Q_OBJECT

 private:
  //DeviceDialog *dialog;
  QTextEdit *edit;
  QTabWidget *tabs;

  std::string selected_dev;
  std::string selected_filter;
  Sniffer* sniffer;
  QTcpStack* machine;

  threading::thread* main_loop;
  threading::thread* second_loop;
  threading::lock sys_lock;
  threading::lock net_lock;

  threading::condition poll_cond;
  threading::lock poll_lock;

 signals:
  void toggle_sniffer_act(bool);

  private slots:
    void new_tabs(std::vector<std::string>);
    void accept_interface(std::string);
    void accept_filter(std::string);
    void sniff();
    void render(int index);
    void save_data();
  public:
    CenterWidget(QWidget *parent = 0);
};
#endif
