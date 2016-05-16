/*
 * job.cpp
 * Created By: Matthew Gao
 * Copyright (c) Dell.Inc
 * Created Time: Sun Sep 20 15:12:51 2015
 */

#include<iostream>
#include<stdlib.h>
#include <boost/log/trivial.hpp>
#include"job.h"

using namespace std;
namespace logging = boost::log;

Job::Job():args(NULL),
            name("default"){
    BOOST_LOG_TRIVIAL(info)<<"Construct a job";
}

Job::~Job(){
    BOOST_LOG_TRIVIAL(info)<<"Destruct a job: "<<name;
}

void Job::process(){
    //abort();
    BOOST_LOG_TRIVIAL(info)<<"Processing a job "<<name;
}

void Job::setArgs(void* args){
    //abort();
}

void* Job::getArgs(){
    return NULL;
}
