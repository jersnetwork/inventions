#ifndef _TDECS_H_
#define _TDECS_H_

#include <set>
#include <string>
#include "sniffer.h"
#include "qtcpstack.h"
#include "threading.h"

template <class T1, class T2>
class StackBuilder: public threading::thread {
 private:
  T1& stack;
  T2& sniffer;

  threading::lock& sys_lock;
  threading::lock& net_lock;

 public:
  StackBuilder(T1& sr1, T2& sr2, threading::lock& l1, threading::lock& l2);
  void run();
};

template <class T1>
class StackWatcher: public threading::thread {
 private:
  T1& stack;
  threading::lock& sys_lock;

 public:
  StackWatcher(T1& sr1, threading::lock& l1);
  void run();
};
#endif
