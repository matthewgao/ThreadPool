/*
 * test.cpp
 * Created By: Matthew Gao
 * Copyright (c) Dell.Inc
 * Created Time: Sun Sep 20 19:45:14 2015
 */

#include<iostream>
using namespace std;
#include "job.h"
#include "queue.h"
#include <boost/make_shared.hpp>

void func(boost::shared_ptr<Job> j){
    cout<<"func"<<endl;
    cout<< j.use_count() << endl;
    j = boost::make_shared<Job>();
    j->setName("C");
    cout<< j.use_count() << endl;
    
}

boost::shared_ptr<Job> func1(boost::shared_ptr<Job> &j){
    cout<<"func1"<<endl;
    cout<< j.use_count() << endl;
    j = boost::make_shared<Job>();
    j->setName("B");
    cout<< j.use_count() << endl;
    return j;
}


int main(){

    boost::shared_ptr<Job> x;
    if(x == NULL)
        cout<< "its null"<<endl;

    Queue* q = Queue::getInstance();
    cout<< q <<endl;
    q = Queue::getInstance();
    cout<< q <<endl;

    boost::shared_ptr<Job> j = boost::make_shared<Job>();
    j->setName("A");
    func(j);
    cout<< j << endl;
    cout<< j.use_count() << endl;
    boost::shared_ptr<Job> j1 = func1(j);
    cout<< j.use_count() << endl;
    cout<< j1.use_count() << endl;

    cout<<"-------"<<endl;
    q->addJob(j);
    cout<< j.use_count() << endl;
    cout<< j1.use_count() << endl;

    cout<<"-------"<<endl;
    q->addJob(j);
    cout<< j.use_count() << endl;
    cout<< j1.use_count() << endl;

    cout<<"----------"<<endl;
    q->popJob();
    cout<< j.use_count() << endl;
    
    cout<<"----------"<<endl;
    q->popJob();
    cout<< j.use_count() << endl;

    boost::shared_ptr<Job> tmp = q->popJob();
    if(tmp == NULL){
        cout<<"can not get anything"<<endl;
    }

    return 0;
}
