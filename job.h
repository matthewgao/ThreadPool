/*
 * job.h
 * Created By: Matthew Gao
 * Copyright (c) Dell.Inc
 * Created Time: Sun Sep 20 14:50:33 2015
 */

#ifndef _JOB_H
#define _JOB_H

//template<class T>
class Job{
  public:
    Job();
    virtual ~Job();
    virtual void process();
    virtual void setArgs(void* args);
    void setName(const char* c){
        name = c;
    }

  protected:
    virtual void* getArgs();
  
  private:
    void* args;
    const char* name;
};

#endif
