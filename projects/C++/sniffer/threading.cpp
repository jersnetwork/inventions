#ifndef _THREADING_CPP_
#define _THREADING_CPP_
#include "threading.h"


void* threading::task(void* t) {
  threading::thread* my_thread;
  my_thread = static_cast<threading::thread*>(t);
  my_thread->run();
  return NULL;
}

threading::lock::lock() : locked(false) {
  pthread_mutex_init (&mutex, NULL);
}

threading::lock::~lock() {
  if (locked)
    release();
}

void threading::lock::aquire()  {
  pthread_mutex_lock(&mutex);
  locked = !locked;     // check return values
}

void threading::lock::release() {
  pthread_mutex_unlock(&mutex);
  locked = !locked;    // check return values
}

threading::thread::thread() : detached(false) {}

threading::thread::~thread() { join(); }

void threading::thread::start() {
  int ret = pthread_create(&tid, NULL, &threading::task, this);
  if (ret)
    throw ret;
}

void threading::thread::join() {
  if(!detached)
    pthread_join(tid, NULL);
}

void threading::thread::detach() {
  if(!detached)
    pthread_detach(tid);
  detached = !detached;
}
#endif
