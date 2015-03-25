#ifndef _THREADING_H_
#define _THREADING_H_
#include <pthread.h>

namespace threading {

  class lock {
    friend class condition;
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

  class condition {
  private:
    pthread_cond_t cond;

  public:
    explicit condition();
    ~condition();
    void wait(threading::lock& m);
    void signal();
  };

  class thread {
    pthread_t tid;
    bool detached;
    condition idle_cond;
    lock idle_mutex;

    condition start_cond;
    lock start_mutex;


    bool started;


  protected:
    virtual void run() = 0;

  public:
    thread();
    virtual ~thread();

    void join();
    void detach();
    void kill();

    void start();
    friend void* threading::task(void* x);
  };

  void* task(void* t);
}
#endif
