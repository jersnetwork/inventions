


threading::lock::lock() : locked(false) {}

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

void threading::thread::start(void* input) {
  int ret = pthread_create(&t1, NULL, run, (void*) input);  // >0 on err, throw
}

void threading::thread::join() {
  if(!detached)
    pthread_join(tid, NULL);
}

void threading::thread::detach() {
  if(!detached)
    pthread_detach(thread1);
  detached = !detached;
}
