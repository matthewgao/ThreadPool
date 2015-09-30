/*
 * queue.cpp
 * Created By: Matthew Gao
 * Copyright (c) Dell.Inc
 * Created Time: Sun Sep 20 15:58:20 2015
 */

#include <iostream>
#include <boost/log/trivial.hpp>
#include "queue.h"


using namespace std;
namespace logging = boost::log;

Queue::Queue(){}

Queue::~Queue(){
    MutexGuard lock(mtx);
    list.clear();
}

bool Queue::addJob(boost::shared_ptr<Job>& job){
    MutexGuard lock(mtx);
    list.push_back(job);    
    return true;
}

bool Queue::isEmpty(){
    MutexGuard lock(mtx);
    bool isEmpty = list.empty();
    return isEmpty;
}

/*
 * 关于binary compatible,如果我们更改了头文件, 而没有编译所有跟头文件相关的obj
 * 那么会产生运行时的core dump, 原因就是binary noncompatible, 所以要指明依赖性
 */
boost::shared_ptr<Job> Queue::popJob(){
    if(list.empty()){
        BOOST_LOG_TRIVIAL(info)<<"PopJob: empty list";
        return boost::shared_ptr<Job>();
    }
    MutexGuard lock(mtx);
    //back() front()返回的是引用,这里做了一个转换,相当于用引用复制给了一个
    //新的变量tmp,引用计数+1
    boost::shared_ptr<Job> tmp = list.front();
    BOOST_LOG_TRIVIAL(info)<<"Job Popped";
    list.pop_front(); 
    BOOST_LOG_TRIVIAL(info)<<"After pop a job the ref counting is: "<<tmp.use_count();
    return tmp;
}
