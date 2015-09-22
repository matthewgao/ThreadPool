/*
 * queue.h
 * Created By: Matthew Gao
 * Copyright (c) Dell.Inc
 * Created Time: Sun Sep 20 15:29:37 2015
 */

#ifndef _QUEUE_H
#define _QUEUE_H

#include <pthread.h>
#include <boost/shared_ptr.hpp>
#include <vector>
#include "job.h"

/*
 * shared_ptr now is a part of C++11, so you can use it 
 * directly with std::shared_ptr
 */
typedef std::vector<boost::shared_ptr<Job> > JobList;

class Queue{
  public:
    static Queue* getInstance();
    bool addJob(boost::shared_ptr<Job>& job);
    bool isEmpty();
    boost::shared_ptr<Job> popJob();

  private:
    Queue(){
        pthread_mutex_init(&mtx, NULL);
    }

    ~Queue(){
        pthread_mutex_destroy(&mtx);
    }

    void lock(){pthread_mutex_lock(&mtx);}
    void unlock(){pthread_mutex_unlock(&mtx);}
    
    JobList list;
    pthread_mutex_t mtx;
    static Queue* self;
};

#endif
