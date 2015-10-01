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
    
    pthread_cond_init(&m_cond, NULL);

    m_shut_down = false;
    m_max_thread_num = 10;
    
    m_thread = (pthread_t*)malloc(sizeof(pthread_t)*m_max_thread_num);

    if(NULL == m_thread){
        BOOST_LOG_TRIVIAL(error)<<"Create threads fail";
        return;
    }

    for(int i = 0; i<m_max_thread_num; i++){
        pthread_create(&(m_thread[i]), NULL, thread_routine, (void*)this);    
    }

}

bool ThreadPool::grow(int max_num){
    //m_max_thread_num 
    if(max_num <= m_max_thread_num){
        BOOST_LOG_TRIVIAL(info)<<"ALREADY have "<<m_max_thread_num<<" threads, no needs to grow";
        return true;
    }
    pthread_t * new_thread = (pthread_t*)malloc(sizeof(pthread_t)*max_num);
    BOOST_LOG_TRIVIAL(info)<<"Growing threads to "<<max_num;
    for(int i = m_max_thread_num; i < max_num; i++){
        if(pthread_create(&(new_thread[i]), NULL, thread_routine, (void*)this)){
            BOOST_LOG_TRIVIAL(error)<<"init thread fail: "<<i;
            free(new_thread);
            return false;
        }    
    }
    memcpy(new_thread, m_thread, sizeof(pthread_t)*m_max_thread_num);
    free(m_thread);
    m_thread = new_thread;
    m_max_thread_num = max_num;

    return true;
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
    pthread_exit((void*)"Unepected exit");
}


void ThreadPool::setMaxThread(int num)
{
    this->m_max_thread_num = num;

}

void ThreadPool::destoryPool()
{
    m_shut_down = true;
    pthread_cond_broadcast(&m_cond);
    BOOST_LOG_TRIVIAL(info)<<"destroying pool";
    
    if(m_thread != NULL){
        for(int i = 0; i < m_max_thread_num; i++)
        {
            /*
             * pthread_join will wait for the thread if it haven't exited.
             */
            const char* status = NULL;
            pthread_join(m_thread[i], (void**)&status);
            BOOST_LOG_TRIVIAL(info)<<"thread:  "<<m_thread[i]<<
                " exit with "<<status<<status;
        }
        free(m_thread);
    }

    m_thread = NULL;
    BOOST_LOG_TRIVIAL(info)<<"free thread pool ";
    
    pthread_cond_destroy(&m_cond);

}

MutexLock* ThreadPool::getMutex(){
   return &m_mtx;
}
pthread_cond_t* ThreadPool::getCond(){
   return &m_cond;
}
bool *ThreadPool::shouldShutdown(){
   return &m_shut_down;
}

void ThreadPool::start(){
   pthread_cond_signal(&m_cond);
}

void ThreadPool::startAll(){
    pthread_cond_broadcast(&m_cond);
}
