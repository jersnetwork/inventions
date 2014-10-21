#ifndef _THREADING_H_
#define _THREADING_H_
#include <pthread.h>

namespace threading {
  class lock {
  private:
    pthread_mutex_t mutex;
    bool locked;

    lock(const lock& rhs);
    lock& operator=(const lock& rhs);

  public:
    explicit lock();
    ~lock();
    void aquire();
    void release();
  };

  class thread {
    pthread_t tid;
    bool detached;

  protected:
    virtual void run() = 0;

  public:
    void join();
    void detach();
    thread();
    ~thread();

    void start();
    friend void* threading::task(void* x);
  };

  void* task(void* t) {
    threading::thread* my_thread;
    my_thread = static_cast<threading::thread*>(t);
    my_thread->run();
    return NULL;
  }

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
