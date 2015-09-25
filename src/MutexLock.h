/*
 * MutexLock.h
 * Created By: Matthew Gao
 * Copyright (c) Dell.Inc
 * Created Time: Tue Sep 22 23:23:54 2015
 */

#ifndef _MUTEXLOCK_H
#define _MUTEXLOCK_H

#include <pthread.h>
#include <type_traits>

class MutexLock{
  public:
    MutexLock(){pthread_mutex_init(&mtx, NULL);}
    ~MutexLock(){pthread_mutex_destroy(&mtx);}
    void lock(){pthread_mutex_lock(&mtx);}
    void unlock(){pthread_mutex_unlock(&mtx);}
    pthread_mutex_t* getRawMutexPtr(){return &mtx;}
  private:
    pthread_mutex_t mtx;
};

class MutexGuard{
  public:
    explicit MutexGuard(MutexLock& mtx):mutex(mtx){
        mutex.lock();
    }
    ~MutexGuard(){
        mutex.unlock();
    }
  private:
    MutexLock& mutex;
};

#define MutexGuard(x) static_assert(false, "Misuse MutexGuard")

#endif
