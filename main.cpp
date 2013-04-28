#include "ThreadPool.h"
#include <unistd.h>  //for sleeping


void *run(void *arg){
  int *num=(int*)arg;
  printf("Thread is %x,  Num %d \n",pthread_self(),*num);
}

int main(){
  ThreadPool *tp=new ThreadPool();
  int *num=(int*)malloc(sizeof(int)*19);
  for(int i=0;i<19;i++){
    num[i]=i;
    tp->addJob(run,&num[i]);
    //tp->start();
  }
  printf("MAIN::adding job\n");
  tp->startAll();
  //tp->start();


  sleep(5);
  //tp->destoryPool();
  delete tp;
  
  return 0;
}
