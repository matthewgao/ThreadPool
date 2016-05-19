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
#include <list>
#include "job.h"
#include "MutexLock.h"
/*
 * shared_ptr now is a part of C++11, so you can use it 
 * directly with std::shared_ptr
 */
typedef std::list<boost::shared_ptr<Job> > JobList;

class Queue{
  public:
    bool addJob(boost::shared_ptr<Job>& job);
    bool isEmpty();
    boost::shared_ptr<Job> popJob();

    Queue();
    ~Queue();

  private:
    JobList list;
    MutexLock mtx;
};

#endif
