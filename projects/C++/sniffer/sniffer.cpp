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
#include <cerrno>
#include "sniffer.h"

const std::vector<std::string> Sniffer::list_devices() {
  pcap_if_t *alldevsp , *device;

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
  struct bpf_program fp;

  char errbuf[PCAP_ERRBUF_SIZE];
  device_name = dev;

  if((session_handle = pcap_open_live(dev.c_str(), BUFSIZ, 1, 1000, errbuf)) == NULL){
    std::cout << "pcap_open_live(): " << errbuf  << std::endl;
    std::exit(1);
  }

  if (pcap_compile(session_handle, &fp, filter.c_str(), 0, PCAP_NETMASK_UNKNOWN) < 0) {
    std::cout << "Couldn't parse filter: "
              << filter.c_str() << " "
              << pcap_geterr(session_handle)
              << std::endl;
    std::exit(2);
  }

  if (pcap_setfilter(session_handle, &fp) < 0) {
    std::cout << "Couldn't install filter: "
              << filter.c_str() << " "
              << pcap_geterr(session_handle)
              << std::endl;
    std::exit(2);
  }

  if ((fd = pcap_fileno(session_handle)) < 0) {
    std::cout << "pcap_fileno(): "
              << std::endl;
    std::exit(2);
  }
}

Sniffer::~Sniffer() {
  pcap_close(session_handle);
}

raw_frame Sniffer::sniff() {
  struct timeval timeout;
  struct pcap_pkthdr hdr;
  const u_char *deep_packet = NULL;
  fd_set rfds;
  int ret;

  while(deep_packet == NULL) {
    FD_ZERO(&rfds);
    FD_SET(fd, &rfds);

    timeout.tv_sec = 1;
    timeout.tv_usec = 1;
    if((ret = select(fd+1, &rfds, NULL, NULL, &timeout)) < 0)
      throw ("select failed");

    if(FD_ISSET(fd, &rfds)) {
      deep_packet = pcap_next(session_handle, &hdr);

      if (deep_packet != NULL)
        return raw_frame(deep_packet, hdr.len);
    }
  }
}

#endif
