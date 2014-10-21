#include <pcap.h>
#include <errno.h>
#include <iostream>
#include <typeinfo>
#include <cstdlib>
#include <vector>
#include "packet.h"
#include "stack_sim.h"
#include "threading.h"

template <class T>
class execution: public threading::thread {
private:
  T& shared_resource;
  threading::lock& mylock;
public:
  execution(T& sr, threading::lock& l);
  void run();
};

template <class T>
execution<T>::execution(T& sr, threading::lock& l): shared_resource(sr), mylock(l) {}

template <class T>
void execution<T>::run() {
  while (1) {
    mylock.aquire();

    shared_resource.list_streams();
    for (int i = 1; i <= shared_resource.num_connections(); i++) {
      std::cout << "peeking at " << shared_resource.get_payload(i) << std::endl;

    }

    std::cout << "------------" <<  shared_resource.num_connections()
              << "------------" << std::endl;

    mylock.release();


  }
}

int main(int argc, char **argv) {
  char *dev;
  struct pcap_pkthdr hdr;     /* pcap.h */
  char errbuf[PCAP_ERRBUF_SIZE];
  pcap_t* descr;
  const u_char *packet;

  if((dev = pcap_lookupdev(errbuf)) == NULL){
      printf("%s\n",errbuf);
      std::exit(1);
  }

  if((descr = pcap_open_live(dev, BUFSIZ, 1, 1000, errbuf)) == NULL){
      printf("pcap_open_live(): %s\n",errbuf);
      std::exit(1);
  }

  tcp_stack machine;
  threading::lock mylock;
  execution<tcp_stack> mythread(machine, mylock);

  mythread.start();


  while (true) {
    if((packet = pcap_next(descr,&hdr)) != NULL) {
      try {
        tcp_packet p(packet);

        if(p.get_src_ip() == std::string("192.168.1.2") ) {
          mylock.aquire();
          std::cout << "inserting " << p.get_data() << std::endl;
          machine.insert(p);
          mylock.release();
        }

      } catch(std::string e) {
        continue;
      }
      // machine.list_streams();

      // for (int i = 1; i <= machine.num_connections(); i++) {
      //   std::cout << "peeking at: " << machine.get_payload(i) << std::endl;

      // }

    }
  }



  return 0;
}
