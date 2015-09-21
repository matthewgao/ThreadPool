#include "ThreadPool.h"
#include <unistd.h>  //for sleeping
#include "queue.h"
#include <boost/make_shared.hpp>

void *run(void *arg){
    int *num = (int*)arg;
    printf("Thread is %x,  Num %d \n", pthread_self(), *num);
}

int main(){
    Queue* q = Queue::getInstance();
    ThreadPool *tp = new ThreadPool();
    /*int *num = (int*)malloc(sizeof(int)*19);*/
    
    //for(int i = 0; i<19; i++){
        //num[i] = i;
        //tp->addJob(run, &num[i]);
        ////tp->start();
    /*}*/
    boost::shared_ptr<Job> j1 = boost::make_shared<Job>();
    j1->setName("AA");
    
    boost::shared_ptr<Job> j2 = boost::make_shared<Job>();
    j2->setName("BB");

    q->addJob(j1);
    q->addJob(j2);

    printf("MAIN::adding job\n");
    tp->startAll();
    //tp->start();

    sleep(1);
    j1 = boost::shared_ptr<Job>();
    j2 = boost::shared_ptr<Job>();

    sleep(1000);
    //tp->destoryPool();
    delete tp;
  
    return 0;
}
