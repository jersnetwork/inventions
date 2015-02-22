#ifndef _QSTACKSIM_H_
#define _QSTACKSIM_H_
#include <QObject>
#include <vector>
#include "tcpstack.h"
//#include "packet.h"

class QTcpStack: public QObject, public TcpStack {
  Q_OBJECT

 public:
  QTcpStack() {};
  std::vector<std::string> list_streams();
  void trigger_update(std::vector<std::string>);

  signals:
  void new_connections(std::vector<std::string>);
};
#endif //_STACKSIM_H_
