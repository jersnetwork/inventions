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

class raw_frame {
 private:
  char* frame;
  int size;
  raw_frame(){}

 public:
  const char* c_str() const;
  int get_size() const;
  raw_frame(const u_char* contents, int size);
  raw_frame(const raw_frame& rhs);
  raw_frame& operator=(const raw_frame& rhs);
  ~raw_frame();
};

class tcp_packet {
private:
  std::string payload;
  std::string src_ip;
  std::string dst_ip;
  int size;
  int src_port;
  int dst_port;
  long seq_num;
  int syn, fin;
  char* packet;

public:
  tcp_packet(const raw_frame& rhs);
  tcp_packet(const tcp_packet& rhs);
  tcp_packet& operator=(const tcp_packet& frame);
  ~tcp_packet();

  const std::string& get_src_ip() const;
  const std::string& get_dst_ip() const;
  const std::string& get_data() const;
  int get_src_port() const;
  int get_dst_port() const;
  long get_seq() const;
  bool get_syn() const;
  bool get_fin() const;
};

#endif //_PACKET_H_
