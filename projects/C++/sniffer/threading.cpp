#ifndef _THREADING_CPP_
#define _THREADING_CPP_
#include "threading.h"

threading::lock::lock() : locked(false) {
  pthread_mutex_init (&mutex, NULL);
}

threading::lock::~lock() {
  if (locked)
    release();
}

void threading::lock::aquire() {
  pthread_mutex_lock(&mutex);
  locked = !locked;
}

void threading::lock::release() {
  pthread_mutex_unlock(&mutex);
  locked = !locked;
}

void threading::condition::wait(threading::lock& lock) {
  pthread_cond_wait(&cond, &lock.mutex);
}

void threading::condition::signal() {
  pthread_cond_signal(&cond);
}

threading::condition::condition() {
  pthread_cond_init(&cond, NULL);
}

threading::condition::~condition() {
  pthread_cond_destroy(&cond);
}

threading::thread::~thread() {}

threading::thread::thread()
  : detached(false), started(false), start_mutex(), idle_cond() {
  idle_mutex.aquire();
  int ret = pthread_create(&tid, NULL, &threading::task, this);
  if (ret)
    throw ret;
}

void* threading::task(void* t) {
  threading::thread* my_thread;
  my_thread = static_cast<threading::thread*>(t);
  my_thread->idle_cond.wait(my_thread->idle_mutex);
  my_thread->idle_mutex.release();
  my_thread->run();

  return NULL;
}

void threading::thread::start() {
  idle_mutex.aquire();
  idle_cond.signal();
  idle_mutex.release();
}

void threading::thread::join() {
  if(!detached)
    pthread_join(tid, NULL);
}

void threading::thread::kill() {
  pthread_cancel(tid);
}

void threading::thread::detach() {
  if(!detached)
    pthread_detach(tid);
  detached = !detached;
}
#endif
