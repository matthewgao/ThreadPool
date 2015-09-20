/*
 * queue.cpp
 * Created By: Matthew Gao
 * Copyright (c) Dell.Inc
 * Created Time: Sun Sep 20 15:58:20 2015
 */

#include<iostream>
#include "queue.h"

using namespace std;

Queue* Queue::self = NULL;

Queue* Queue::getInstance(){
    if(NULL == self){
        self = new Queue();
    }
    return self;
}

bool Queue::addJob(boost::shared_ptr<Job>& job){
    lock();
    list.push_back(job);    
    unlock();
    return true;
}

bool Queue::isEmpty(){
    lock();
    bool isEmpty = list.empty();
    unlock();
    return isEmpty;
}

boost::shared_ptr<Job>& Queue::popJob(){
    /*if(list.empty()){*/
        //cout<<"popJob: Empty list"<<endl;
        //return boost::shared_ptr<Job>&;
    /*}*/
    lock();
    boost::shared_ptr<Job>& tmp = list.back();
    cout<<"Popjob"<<tmp.use_count()<<endl;
    list.pop_back(); 
    unlock();
    return tmp;
}
