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
#include <iostream>

class tcp_packet {
private:
  std::string payload;
  std::string src_ip;
  std::string dst_ip;
  int src_port;
  int dst_port;
  long seq_num;
  int syn, fin;

  tcp_packet();
public:
  tcp_packet(const u_char *packet);
  const std::string& get_src_ip() const;
  const std::string& get_dst_ip() const;
  const std::string& get_data() const;
  int get_src_port() const;
  int get_dst_port() const;
  int get_seq() const;
};

tcp_packet::tcp_packet(const u_char *packet) {
  struct ether_header* eth;
  struct iphdr* ip;
  struct tcphdr* tcp;
  struct in_addr raw_src_ip;
  struct in_addr raw_dst_ip;
  int ip_hdr_len;
  int ip_total_len;
  int tcp_hdr_len;

  eth = (struct ether_header *) packet;

  if(ntohs(eth->ether_type) != ETHERTYPE_IP)
    throw std::string("Not an IP Packet");

  ip = (struct iphdr*)(packet + sizeof(struct ether_header));
  raw_src_ip = *(reinterpret_cast<struct in_addr*>(&ip->saddr));
  raw_dst_ip = *(reinterpret_cast<struct in_addr*>(&ip->daddr));
  src_ip = std::string(inet_ntoa(raw_src_ip));
  dst_ip = std::string(inet_ntoa(raw_dst_ip));
  ip_hdr_len = static_cast<int>(ip->ihl);
  ip_total_len = htons(static_cast<int>(ip->tot_len));

  if(static_cast<int>(ip->protocol) != 6)
    throw std::string("Not an TCP Packet");

  tcp = (struct tcphdr*)(packet + sizeof(struct ether_header) + (ip_hdr_len*4));
  seq_num = htonl(static_cast<long>(tcp->seq));
  src_port = htons(static_cast<int>(tcp->source));
  dst_port = htons(static_cast<int>(tcp->dest));
  syn = static_cast<int>(tcp->syn);
  fin = static_cast<int>(tcp->fin);
  tcp_hdr_len = static_cast<int>(tcp->doff);

  /* if (src_ip == std::string("192.168.1.2") ) { */
  /*   std::cout << "IP LEN: " << ip_total_len << std::endl; */
  /*   std::cout << "TCP SEQ Num: " << htonl(seq_num) << std::endl; */
  /*   std::cout << "TCP headers len: " << ip_hdr_len << std::endl; */
  /*   std::cout << "IP headers len: " << tcp_hdr_len << std::endl; */
  /*   std::cout << "SYN: " << syn << std::endl; */
  /*   std::cout << "FIN: " << fin << std::endl; */
  /*   std::cout << "Both headers LEN: " << ((ip_hdr_len * 4)+(tcp_hdr_len * 4)) << std::endl; */
  /* } */

  payload = std::string(reinterpret_cast<const char*>(packet+sizeof(struct ether_header) +
                                                      (ip_hdr_len * 4) + (tcp_hdr_len * 4)),
                        ip_total_len - ((ip_hdr_len * 4) + (tcp_hdr_len * 4)));
}

const std::string& tcp_packet::get_src_ip() const {
  return src_ip;
}

const std::string& tcp_packet::get_dst_ip() const {
  return dst_ip;
}

const std::string& tcp_packet::get_data() const {
  return payload;
}

int tcp_packet::get_src_port() const {
  return src_port;
}

int tcp_packet::get_dst_port() const {
  return dst_port;
}

int tcp_packet::get_seq() const {
  return seq_num;
}
#endif //_PACKET_H_
