#include "ThreadPool.h"
#include <iostream>

using namespace std;
 
  
ThreadPool::ThreadPool(){
    init();
}

ThreadPool:: ~ThreadPool(){
    destoryPool();
}

void ThreadPool::init()
{
    
    pthread_mutex_init(&mtx, NULL);
    pthread_cond_init(&cond, NULL);

    shutdown = false;
    max_thread_num = 10;
    
    thread = (pthread_t*)malloc(sizeof(pthread_t)*max_thread_num);

    if(NULL == thread){
        printf("Create threads fail\n");
        return;
    }

    for(int i = 0; i<max_thread_num; i++){
        pthread_create(&(thread[i]), NULL, thread_routine, (void*)this);    
    }

}

//this is a member function, pthread_create can not use a member 
//function Not EVEN a static one.
void *thread_routine(void *arg)
{
    ThreadPool *tpool = (ThreadPool*)arg;
    pthread_mutex_t *mtx;
    pthread_cond_t *cond;
    bool *shutdown;

    mtx = tpool->getMutex();
    cond = tpool->getCond();
    shutdown = tpool->shouldShutdown();

    if(debuglevel >= 2){
        printf("Thread is %x, The cond is %x\n", pthread_self(), cond);
        printf("Thread is %x, The mutex is %x\n", pthread_self(), mtx);
        printf("Thread is %x, Tpool is %x\n", pthread_self(), tpool);
    }

    Queue* jobQueue = Queue::getInstance();
    
    printf("thread_routine\n");
    while(false == (*shutdown)){

        pthread_mutex_lock(mtx);
        
        while(jobQueue->isEmpty() && (!*shutdown)){
            if(debuglevel >= 0){
                printf("%x loop again \n",pthread_self());
            }
        
            pthread_cond_wait(cond, mtx);
        }     
        
        boost::shared_ptr<Job> job = jobQueue->popJob();
        if(*shutdown == true){	
            pthread_mutex_unlock(mtx);
            pthread_exit(NULL);
        }
        if(debuglevel >= 1){
            printf("%x is Runing\n", pthread_self());
        }      
        
        pthread_mutex_unlock(mtx);
        
        if(NULL == job){
            cout<<"Job is NULL"<<endl;
            continue;
        }
        if(debuglevel >= 0){
            printf("Got a Job, Thread is %x,\n", pthread_self());
        }

        job->process();
      
        if(debuglevel >= 1){
            printf("%x is exiting\n", pthread_self());
        }
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
            pthread_join(thread[i], NULL);
        }
        free(thread);
    }

    thread = NULL;
    if(debuglevel >= 1){
        printf("free thread pool\n");
    }
    
    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond);

}

pthread_mutex_t* ThreadPool::getMutex(){
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
