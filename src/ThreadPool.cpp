#include <iostream>
#include <signal.h>
#include <boost/log/trivial.hpp>
#include "ThreadPool.h"
#include "singleton.h"

using namespace std;
namespace logging = boost::log;
 
  
ThreadPool::ThreadPool(){
    init();
}

ThreadPool:: ~ThreadPool(){
    destoryPool();
}

void ThreadPool::init()
{
    
    pthread_cond_init(&cond, NULL);

    shutdown = false;
    max_thread_num = 10;
    
    thread = (pthread_t*)malloc(sizeof(pthread_t)*max_thread_num);

    if(NULL == thread){
        BOOST_LOG_TRIVIAL(error)<<"Create threads fail";
        return;
    }

    for(int i = 0; i<max_thread_num; i++){
        pthread_create(&(thread[i]), NULL, thread_routine, (void*)this);    
    }

}

void *thread_routine(void *arg)
{
    ThreadPool *tpool = (ThreadPool*)arg;
    MutexLock* mtx;
    pthread_cond_t *cond;
    bool *shutdown;

    mtx = tpool->getMutex();
    cond = tpool->getCond();
    shutdown = tpool->shouldShutdown();

    BOOST_LOG_TRIVIAL(debug)<<"thread is "
        <<pthread_self()<<"cond is: "<<cond<<"mtx is:"
        <<mtx->getRawMutexPtr()<<"Tpool is: "<<tpool;

    Queue* jobQueue = Singleton<Queue>::getInstance();
    
    BOOST_LOG_TRIVIAL(info)<<"thread routine";

    while(false == (*shutdown)){

        {
            MutexGuard lock(*mtx);
            
            while(jobQueue->isEmpty() && (!*shutdown)){
                            
                BOOST_LOG_TRIVIAL(info)<<"loop again";
                pthread_cond_wait(cond, mtx->getRawMutexPtr());
            }     
        }   
        if(*shutdown == true){	
            pthread_exit((void*)"Thread exited");
        }

        /*
         * Because this jobQueue is thread safe, so we don't need to 
         * protect it with another mutex
         */
        boost::shared_ptr<Job> job = jobQueue->popJob();
        
        BOOST_LOG_TRIVIAL(info)<<pthread_self()<<" is running";

        if(NULL == job){
            BOOST_LOG_TRIVIAL(error)<<pthread_self()<<" Job is NULL";
            continue;
        }

        BOOST_LOG_TRIVIAL(info)<<"Got a job, thread_id: "<<pthread_self();

        job->process();
      
        BOOST_LOG_TRIVIAL(info)<<"Job done, thread_id: "<<pthread_self();
    }
    //never be here
    pthread_exit(NULL);
}


void ThreadPool::setMaxThread(int num)
{
    this->max_thread_num = num;

}

void ThreadPool::destoryPool()
{
    shutdown = true;
    pthread_cond_broadcast(&cond);
    printf("destroy pool\n");
    
    if(thread != NULL){
        for(int i = 0; i < max_thread_num; i++)
        {
            /*
             * pthread_join will wait for the thread if it haven't exited.
             */
            const char* status = NULL;
            pthread_join(thread[i], (void**)&status);
            BOOST_LOG_TRIVIAL(info)<<"thread:  "<<thread[i]<<
                " exit with "<<status<<status;
        }
        free(thread);
    }

    thread = NULL;
    BOOST_LOG_TRIVIAL(info)<<"free thread pool ";
    
    pthread_cond_destroy(&cond);

}

MutexLock* ThreadPool::getMutex(){
   return &mtx;
}
pthread_cond_t* ThreadPool::getCond(){
   return &cond;
}
bool *ThreadPool::shouldShutdown(){
   return &shutdown;
}

void ThreadPool::start(){
   pthread_cond_signal(&cond);
}

void ThreadPool::startAll(){
    pthread_cond_broadcast(&cond);
}
