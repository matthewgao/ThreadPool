#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>

typedef struct Quene{
  void *arg;
  void *(*process)(void *arg);
  struct Quene *next;
}JobQuene;

static int debuglevel=0; //0 means no debug info
//extern "C" 
void *thread_routine(void*);

class ThreadPool{

 private:
  bool shutdown;
  int max_thread_num;
  int max_quene_num;
  pthread_mutex_t mtx;
  pthread_cond_t cond;
  JobQuene *jq_head;
  JobQuene *jq_rear;
  pthread_t *thread;
  void init();
 public:
  //here is the other way to define the function,to pthread_create
  // friend void *thread_routine(void *);
  // static void *thread_routine(void *);
  // void *thread_routine();
  ThreadPool();
  ~ThreadPool();
  void addJob(void* (*)(void*), void*);
  void start();
  void startAll();
  void setMaxThread(int);
  void setMaxQuene(int);
  void destoryPool();
  pthread_mutex_t* getMutex();
  pthread_cond_t* getCond();
  bool *shouldShutdown();
  JobQuene *getJobQuene();
  void setJobQuene(JobQuene *);

};
