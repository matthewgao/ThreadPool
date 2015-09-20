/*
 * job.cpp
 * Created By: Matthew Gao
 * Copyright (c) Dell.Inc
 * Created Time: Sun Sep 20 15:12:51 2015
 */

#include<iostream>
#include<stdlib.h>
#include"job.h"

using namespace std;

Job::Job():args(NULL),
            name("default"){
    cout<<"construct job "<<name<<endl;
}

Job::~Job(){
    cout<<"destruct job "<<name<<endl;
}

void Job::process(){
    //abort();
    cout<<"job process"<<endl;
}

void Job::setArgs(void* args){
    //abort();
}

void* Job::getArgs(){
    return NULL;
}
