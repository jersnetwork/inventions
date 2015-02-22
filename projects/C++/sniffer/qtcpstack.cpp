#ifndef _QSTACKSIM_CPP_
#define _QSTACKSIM_CPP_
#include <QMetaType>
#include "qtcpstack.h"

void QTcpStack::trigger_update(std::vector<std::string> update) {
  qRegisterMetaType<std::vector<std::string> >("std::vector<std::string>");
  emit new_connections(update);
}

std::vector<std::string> QTcpStack::list_streams() {
  std::vector<std::string> list;
  std::map<std::string, std::vector<tcp_packet> >::iterator itr;

  for(itr = streams.begin(); itr != streams.end(); itr++) {
    list.push_back(itr->first);
  }
  return list;
}
#endif //_STACKSIM_H_
