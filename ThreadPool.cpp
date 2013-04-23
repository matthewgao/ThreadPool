#include "ThreadPool.h"
 
//extern "C" void thread_routine(void*);
  
ThreadPool::ThreadPool(){
  init();
}

ThreadPool:: ~ThreadPool(){
  destoryPool();
}

void ThreadPool::init(){
  pthread_mutex_init(&mtx,NULL);
  pthread_cond_init(&cond,NULL);

  shutdown=false;
  max_thread_num=10;
  max_quene_num=10;
  jq_head=NULL;
  jq_rear=NULL;
  
  thread=(pthread_t*)malloc(sizeof(pthread_t)*max_thread_num);
  for(int i=0;i<max_thread_num;i++){
    pthread_create(&(thread[i]),NULL,
		   thread_routine,
		   (void*)this);    
  }

}

//void *thread_routine(void *delegate){
//  return reinterpret_cast<ThreadPool*>(delegate)->thread_routine();
//}

//this is a member function, pthread_create can not use a member function Not EVEN a static one.
 void *thread_routine(void *arg){
  ThreadPool *tpool=(ThreadPool*)arg;
  pthread_mutex_t *mtx;
  pthread_cond_t *cond;
  bool *shutdown;
  JobQuene *jq_head;

  mtx=tpool->getMutex();
  cond=tpool->getCond();
  shutdown=tpool->shouldShutdown();
  jq_head=tpool->getJobQuene();

  if(debuglevel>=2){
    printf("Thread is %x, The cond is %x\n",pthread_self(),cond);
    printf("Thread is %x,The mutex is %x\n",pthread_self(),mtx);
    printf("Thread is %x,Tpool is %x\n",pthread_self(),tpool);
  }
  JobQuene *pos=jq_head;
  printf("thread_routine\n");
  while(false==(*shutdown)){
    pthread_mutex_lock(mtx);
    while((tpool->getJobQuene()==NULL)&&(!(*shutdown))){
      if(debuglevel>=2)
	printf("%x loop again \n",pthread_self());
      pthread_cond_wait(cond,mtx);
      if(debuglevel>=1)
	printf("!!!Thread is %x, jobhead is %x\n",
	       pthread_self(),tpool->getJobQuene());
    }
    //should be aware that the jobquene pointer should be update when the data has been updated, because the two pointer is different variable, it should be synconized.
    pos=tpool->getJobQuene();
    if(*shutdown==true)
    {	
      pthread_mutex_unlock(mtx);
      pthread_exit(NULL);
    }
    if(debuglevel>=1)
      printf("%x is Runing\n", pthread_self());
    jq_head=pos->next;
    tpool->setJobQuene(jq_head);
    pthread_mutex_unlock(mtx);
    (*(pos->process))(pos->arg);
    free(pos);
    pos=jq_head;
    if(debuglevel>=1)
      printf("%x is exiting\n", pthread_self());
  }
  //never be here
  pthread_exit(NULL);
}

void ThreadPool::addJob(void *(*process)(void *arg),void *arg){
 
  JobQuene *newJob=(JobQuene*)malloc(sizeof(JobQuene));
  newJob->process=process;
  newJob->arg=arg;
  newJob->next=NULL;

  pthread_mutex_lock(&mtx);
  if(NULL==jq_head){
    jq_head=newJob;
    jq_rear=jq_head;
  }
  jq_rear->next=newJob;
  jq_rear=newJob;
  pthread_mutex_unlock(&mtx);
  if(debuglevel>=1)
    printf("Add Job finished\n");
  
}
void ThreadPool::setMaxThread(int num){
  this->max_thread_num=num;

}
void ThreadPool::destoryPool(){
  shutdown=true;
  pthread_cond_broadcast(&cond);
  printf("destroy pool\n");
  if(thread!=NULL){
  for(int i=0;i<max_thread_num;i++)
    {
      pthread_join(thread[i],NULL);
    }
  free(thread);
  }
  thread=NULL;
  if(debuglevel>=1)
    printf("free thread pool\n");
  while(jq_head!=NULL){
    JobQuene *tmp=jq_head;
    jq_head=jq_head->next;
    free(tmp);
    if(debuglevel>=1)
      printf("freeing\n");
  }
  if(debuglevel>=1)
    printf("free job quene\n");
  pthread_mutex_destroy(&mtx);
  pthread_cond_destroy(&cond);

}
void ThreadPool::setMaxQuene(int num){
  this->max_quene_num=num;
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
JobQuene *ThreadPool::getJobQuene(){
  return jq_head;
}

void ThreadPool::start(){
  pthread_cond_signal(&cond);
}

void ThreadPool::startAll(){
  pthread_cond_broadcast(&cond);
}

void ThreadPool::setJobQuene(JobQuene *jq)
{
  jq_head=jq;

}
