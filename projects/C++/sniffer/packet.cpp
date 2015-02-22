#ifndef _PACKET_CPP_
#define _PACKET_CPP_
#include "packet.h"

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
