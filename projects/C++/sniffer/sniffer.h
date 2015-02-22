#ifndef _SNIFFER_H_
#define _SNIFFER_H_
#include <vector>
#include <string>
#include <pcap.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include "packet.h"

class Sniffer {
 private:
  pcap_t * session_handle;
  std::string device_name;
  Sniffer() {};

 public:
  static const  std::vector<std::string> list_devices();
  Sniffer(const std::string& dev, const std::string filter);
  tcp_packet sniff();
  ~Sniffer();
};
#endif
