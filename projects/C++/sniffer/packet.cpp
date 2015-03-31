#ifndef _PACKET_CPP_
#define _PACKET_CPP_
#include <cstring>
#include <cstdlib>
#include <iostream>
#include "packet.h"

raw_frame::raw_frame(const u_char* f, int size) {
  this->size = size;
  try {
    frame = new char[size];

  } catch(const std::bad_alloc& e) {
    std::cout << "ERROR: Can't allocate " << size << " bytes." << std::endl;
    std::exit(1);
  }
  std::memcpy(frame, f, size);
}

raw_frame::raw_frame(const raw_frame& rhs) {
  size = rhs.size;

  try {
    frame = new char[size];
  } catch(const std::bad_alloc& e) {
    std::cout << "ERROR: Can't allocate " << size << " bytes." << std::endl;
    std::exit(1);
  }
  std::memcpy(frame, rhs.frame, size);
}

raw_frame& raw_frame::operator=(const raw_frame& rhs) {
  size = rhs.size;

  try {
    frame = new char[size];
  } catch(const std::bad_alloc& e) {
    std::cout << "ERROR: Can't allocate " << size << " bytes." << std::endl;
    std::exit(1);
  }
  std::memcpy(frame, rhs.frame, rhs.size);
  return *this;
}

raw_frame::~raw_frame() {
  delete [] frame;
}

const char* raw_frame::c_str() const{
  return frame;
}

int raw_frame::get_size() const{
  return size;
}

tcp_packet::tcp_packet(const raw_frame& frame) {
  size = frame.get_size();
  try {
    packet = new char[size];
  } catch(const std::bad_alloc& e) {
    std::cout << "ERROR: Can't allocate " << size << " bytes." << std::endl;
    std::exit(1);
  }
  std::memcpy(packet, frame.c_str()  , size);

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

tcp_packet::tcp_packet(const tcp_packet& frame) {
  size = frame.size;
  try {
    packet = new char[size];
  } catch(const std::bad_alloc& e) {
    std::cout << "ERROR: Can't allocate " << size << " bytes." << std::endl;
    std::exit(1);
  }
  std::memcpy(packet, frame.packet  , size);

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

tcp_packet& tcp_packet::operator=(const tcp_packet& frame) {
  size = frame.size;
  try {
    packet = new char[size];
  } catch(const std::bad_alloc& e) {
    std::cout << "ERROR: Can't allocate " << size << " bytes." << std::endl;
    std::exit(1);
  }
  std::memcpy(packet, frame.packet  , size);

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
  return *this;
}

tcp_packet::~tcp_packet() {
  delete packet;
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

bool tcp_packet::get_syn() const {
  return syn == 1 ? true : false  ;
}

bool tcp_packet::get_fin() const {
  return fin == 1 ? true : false  ;
}

long tcp_packet::get_seq() const {
  return seq_num;
}


#endif //_PACKET_H_
