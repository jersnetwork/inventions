#ifndef _STACKSIM_H_
#define _STACKSIM_H_
#include <map>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>
#include <algorithm>
#include <utility>
#include <pcap.h>
#include <QObject>

#include "packet.h"

class TcpStack {

 public:
  TcpStack();
  std::string get_payload(std::string connection);
  std::string get_payload(int index);
  int num_connections();
  void dump_streams();
  void insert(tcp_packet packet);

 protected:
  std::map<std::string, std::vector<tcp_packet> > streams;
};

#endif //_STACKSIM_H_
