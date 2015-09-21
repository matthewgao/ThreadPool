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

/*
 * 关于binary compatible,如果我们更改了头文件, 而没有编译所有跟头文件相关的obj
 * 那么会产生运行时的core dump, 原因就是binary noncompatible, 所以要指明依赖性
 */
boost::shared_ptr<Job> Queue::popJob(){
    if(list.empty()){
        cout<<"popJob: Empty list"<<endl;
        return boost::shared_ptr<Job>();
    }
    lock();
    //back() front()返回的是引用,这里做了一个转换,相当于用引用复制给了一个
    //新的变量tmp,引用计数+1
    boost::shared_ptr<Job> tmp = list.back();
    cout<<"Popjob "<<tmp.use_count()<<endl;
    list.pop_back(); 
    cout<<"After Popjob "<<tmp.use_count()<<endl;
    unlock();
    return tmp;
}
