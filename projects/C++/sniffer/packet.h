#ifndef _PACKET_H_
#define _PACKET_H_
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <string>

class tcp_packet {
private:
  std::string payload;
  std::string src_ip;
  std::string dst_ip;
  int src_port;
  int dst_port;
  long seq_num;
  int syn, fin;

public:
  tcp_packet(const u_char *packet);
  const std::string& get_src_ip() const;
  const std::string& get_dst_ip() const;
  const std::string& get_data() const;
  int get_src_port() const;
  int get_dst_port() const;
  int get_seq() const;
};

#endif //_PACKET_H_
