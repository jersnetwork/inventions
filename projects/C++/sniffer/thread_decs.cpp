#ifndef _TDECS_CPP_
#define _TDECS_CPP_

#include <set>
#include <string>
#include "sniffer.h"
#include "qtcpstack.h"
#include "threading.h"
#include "thread_decs.h"

template <class T1, class T2>
  StackBuilder<T1, T2>::StackBuilder(T1& sr1, T2& sr2, threading::lock& l1, threading::lock& l2):
  stack(sr1), sniffer(sr2), sys_lock(l1), net_lock(l2) {}

template <class T1, class T2>
    void StackBuilder<T1, T2>::run() {
  while(1) {
    net_lock.aquire();
    tcp_packet p = sniffer.sniff();
    net_lock.release();

    sys_lock.aquire();
    stack.insert(p);
    sys_lock.release();
  }
}

template <class T1>
StackWatcher<T1>::StackWatcher(T1& sr,  threading::lock& l1):
stack(sr), sys_lock(l1) {}

template <class T1>
void StackWatcher<T1>::run() {
  int num_connections = 0;
  int current_connections = 0;
  std::vector<std::string> connections = stack.list_streams();

  while (1) {
    sys_lock.aquire();
    current_connections = stack.num_connections();
    sys_lock.release();

    if (num_connections < current_connections) {
      sys_lock.aquire();
      std::vector<std::string> update = stack.list_streams();
      sys_lock.release();

      std::set<std::string> s1(connections.begin(), connections.end());
      std::set<std::string> s2(update.begin(), update.end());
      std::vector<std::string> diff;
      std::set_intersection(connections.begin(), connections.end(),
                            update.begin(), update.end(), std::back_inserter(diff));
      connections = update;
      stack.trigger_update(diff);
    }
    num_connections = current_connections;
  }
}
#endif
