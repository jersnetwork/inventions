#ifndef _SNIFFER_CPP_
#define _SNIFFER_CPP_

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <cstring>
#include <vector>
#include <iostream>
#include <string>
#include <cstdlib>
#include "sniffer.h"

const std::vector<std::string> Sniffer::list_devices() {
  struct in_addr addr;
  bpf_u_int32 netp;
  bpf_u_int32 maskp;
  pcap_if_t *alldevsp , *device;
  char *devname, *net,  **devs;
  int count = 1;
  char errbuf[PCAP_ERRBUF_SIZE];
  std::vector<std::string> hardware;

  if(pcap_findalldevs(&alldevsp, errbuf)){
    printf("Error: %s" , errbuf);
    std::exit(1);
  }

  for(device=alldevsp; device; device=device->next, count++) {
    std::string dev(device->name);
    hardware.push_back(dev);
  }

  return hardware;
}

Sniffer::Sniffer(const std::string& dev, const std::string filter) {
  bpf_u_int32 mask;
  bpf_u_int32 net;
  struct bpf_program fp;

  char errbuf[PCAP_ERRBUF_SIZE];
  device_name = dev;

  if((session_handle = pcap_open_live(dev.c_str(), BUFSIZ, 1, 1000, errbuf)) == NULL){
    printf("pcap_open_live(): %s\n",errbuf);
    std::exit(1);
  }

  if (pcap_compile(session_handle, &fp, filter.c_str(), 0, net) == -1) {
    fprintf(stderr, "Couldn't parse filter %s: %s\n", filter.c_str(), pcap_geterr(session_handle));
    std::exit(2);
  }

  if (pcap_setfilter(session_handle, &fp) == -1) {
    fprintf(stderr, "Couldn't install filter %s: %s\n", filter.c_str(), pcap_geterr(session_handle));
    std::exit(2);
  }

}

Sniffer::~Sniffer() {
  pcap_close(session_handle);
}

tcp_packet Sniffer::sniff() {
  u_char *deep_packet = NULL;
  const u_char *packet;
  struct pcap_pkthdr hdr;

  while (1) {
    packet = pcap_next(session_handle, &hdr);
    if (packet != NULL) {
      deep_packet =  new u_char[hdr.caplen];
      std::memcpy(deep_packet, packet, hdr.caplen);
      try {
        tcp_packet p(deep_packet);
        delete [] deep_packet;
        return p;
      } catch (std::string e) {
        continue;
      }
    }
  }
}

#endif
