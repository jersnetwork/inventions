#ifndef _TDECS_CPP_
#define _TDECS_CPP_

#include <set>
#include <string>
#include "sniffer.h"
#include "qtcpstack.h"
#include "thread_decs.h"

template <class T1, class T2>
StackBuilder<T1, T2>::StackBuilder(T1& sr1, T2& sr2, threading::lock& l1,
                                   threading::lock& l2, threading::condition& pc)
  : stack(sr1), sniffer(sr2), sys_lock(l1), net_lock(l2), poll_cond(pc) {}

template <class T1, class T2>
void StackBuilder<T1, T2>::run() {
  while(true) {
    net_lock.aquire();
    raw_frame ether = sniffer.sniff();
    net_lock.release();

    try {
      tcp_packet tcp(ether);
      sys_lock.aquire();
      if (tcp.get_data().length() > 0)
        stack.insert(tcp);
      sys_lock.release();

      poll_cond.signal();

    } catch (std::string e) {
      continue;
    }
  }
}

template <class T1>
TabCreator<T1>::TabCreator(T1& sr,  threading::lock& l1, threading::condition& pc, threading::lock& pl)
  : stack(sr), sys_lock(l1), poll_cond(pc), poll_lock(pl) {}

template <class T1>
void TabCreator<T1>::run() {
  int num_connections = 0;
  int current_connections = 0;
  std::vector<std::string> connections = stack.list_streams();
  std::vector<std::string> update;
  std::vector<std::string> new_tabs;
  std::set<std::string> diff;

  while (true) {
    poll_lock.aquire();
    poll_cond.wait(poll_lock);
    poll_lock.release();

    sys_lock.aquire();
    current_connections = stack.num_connections();
    sys_lock.release();

    if (num_connections < current_connections) {
      sys_lock.aquire();
      update = stack.list_streams();
      sys_lock.release();

      std::set<std::string> s1(connections.begin(), connections.end());
      std::set<std::string> s2(update.begin(), update.end());

      std::set_difference(update.begin(),
                          update.end(),
                          connections.begin(),
                          connections.end(),
                          std::inserter(diff,
                                        diff.end()));

      std::copy(diff.begin(), diff.end(), std::back_inserter(new_tabs));
      stack.trigger_update(new_tabs);
      num_connections = current_connections;
      connections = update;
      new_tabs.clear();
      diff.clear();
    }
  }
}

#endif
