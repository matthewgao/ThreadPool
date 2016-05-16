#ifndef THREADPOOL
#define THREADPOOL

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <boost/shared_ptr.hpp>
#include <stdlib.h>
#include "queue.h"
#include "../include/MutexLock.h"

static int debuglevel = 0; //0 means no debug info

void *thread_routine(void*);

class MutexLock;

class ThreadPool{

  private:
    bool m_shut_down;
    int m_max_thread_num;
    MutexLock m_mtx;
    pthread_cond_t m_cond;
    pthread_t *m_thread;
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
    MutexLock* getMutex();
    pthread_cond_t* getCond();
    bool *shouldShutdown();
    bool grow(int);
};
#endif
