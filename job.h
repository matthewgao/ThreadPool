/*
 * job.h
 * Created By: Matthew Gao
 * Copyright (c) Dell.Inc
 * Created Time: Sun Sep 20 14:50:33 2015
 */

#ifndef _JOB_H
#define _JOB_H
#include <iostream>
//template<class T>
class Job{
  public:
    Job();
    virtual ~Job();
    virtual void process();
    virtual void setArgs(void* args);
    void setName(std::string s){
        name = s;
    }

  protected:
    virtual void* getArgs();
  
  private:
    void* args;
    std::string name;
};

#endif
