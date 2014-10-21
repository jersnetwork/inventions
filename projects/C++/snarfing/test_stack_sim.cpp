#include <pcap.h>
#include <errno.h>
#include <iostream>
#include <typeinfo>
#include <cstdlib>
#include <vector>
#include "packet.h"
#include "stack_sim.h"

int main(int argc, char **argv) {
  tcp_stack machine

  char *dev;
  struct pcap_pkthdr hdr;     /* pcap.h */
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t* descr;
  const u_char *packet;
  std::vector<tcp_packet> q;

  if((dev = pcap_lookupdev(errbuf)) == NULL){
      printf("%s\n",errbuf);
      std::exit(1);
  }

  if((descr = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf)) == NULL){
      printf("pcap_open_live(): %s\n",errbuf);
      std::exit(1);
  }

  while (true) {
    if((packet = pcap_next(descr,&hdr)) != NULL) {
      try {
        //std::cout << "trying" << std::endl;
        tcp_packet p(packet);

        //q.push_back(p);
      } catch(std::string e) {
        continue;
      }

      // std::cout << q.back().get_src_ip() << ":"
      //           << q.back().get_src_port()
      //           << " --> "
      //           << q.back().get_dst_ip() << ":"
      //           << q.back().get_dst_port()
      //           << std::endl;

      // std::cout << q.back().get_data() << std::endl;

      if(q.back().get_src_ip() == std::string("192.168.1.2") ) {
        std::cout << q.back().get_src_ip() << ":"
                  << q.back().get_src_port()
                  << " --> "
                  << q.back().get_dst_ip() << ":"
                  << q.back().get_dst_port()
                  << std::endl;

        std::cout << q.back().get_data();;
      }



      // if(q.back().get_src_port() == 29 || q.back().get_dst_port() == 29  ) {
      //   std::cout << q.back().get_data() << std::endl;
      // }

    }
  }



  return 0;
}
