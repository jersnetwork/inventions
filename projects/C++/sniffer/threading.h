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
    virtual ~thread();

    void start();
    friend void* threading::task(void* x);
  };
  void* task(void* t);
}
#endif
