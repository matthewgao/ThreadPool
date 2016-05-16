ThreadPool
==========
[![Gitter](https://badges.gitter.im/Join Chat.svg)](https://gitter.im/matthewgao/ThreadPool?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

A Linux environment ThreadPool

##TODO LIST
* 动态库
* Logger
* shutdown gracefully
* 


UserGuide
==========
The default thread number is 10, if you want to increase number call setMaxThread().
There is two method to start the threads, one is start() it will start only one thread to process the jobs, the other is startAll() it will start the all the thread to process the jobs. How to use it, it's up to you.

Compile
==========
Using Cmake

~~~
cmake .
make
~~~

Example
==========

~~~
void *run(void *arg);

int main(){

  ThreadPool *tp=new ThreadPool();
  int *num=(int*)malloc(sizeof(int)*19);

  for(int i=0;i<19;i++){
    num[i]=i;
    tp->addJob(run,&num[i]);
  }
  tp->startAll();
  sleep(5);
  delete tp;
  free(num);
  
  return 0;
}


void *run(void *arg){
  int *num=(int*)arg;
  printf("Thread is %x,  Num %d \n",pthread_self(),*num);
}
~~~
