#ifndef THREADPOOL
#define THREADPOOL

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <boost/shared_ptr.hpp>
#include <stdlib.h>
#include "queue.h"


static int debuglevel = 0; //0 means no debug info

void *thread_routine(void*);

class ThreadPool{

  private:
    bool shutdown;
    int max_thread_num;
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    pthread_t *thread;
    void init();
  public:
  //here is the other way to define the function,to pthread_create
  // friend void *thread_routine(void *);
  // static void *thread_routine(void *);
  // void *thread_routine();
    ThreadPool();
    ~ThreadPool();
    void start();
    void startAll();
    void setMaxThread(int);
    void destoryPool();
    pthread_mutex_t* getMutex();
    pthread_cond_t* getCond();
    bool *shouldShutdown();
};
#endif
