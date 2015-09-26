#include "ThreadPool.h"
#include <unistd.h>  //for sleeping
#include "queue.h"
#include <boost/make_shared.hpp>
#include "singleton.h"
#include <iostream>
#include <sstream>

/*void *run(void *arg){*/
    //int *num = (int*)arg;
    //printf("Thread is %x,  Num %d \n", pthread_self(), *num);
/*}*/
void shutdown(ThreadPool* tp){
    tp->destoryPool();
}

int main(){
    Queue* q = Singleton<Queue>::getInstance();
    ThreadPool *tp = new ThreadPool();
    
    int i;
    for(i = 100; i<110; i++){
        std::stringstream ss;
        ss<<i<<endl;
        
        boost::shared_ptr<Job> j1 = boost::make_shared<Job>();
        j1->setName(ss.str());
        cout<<ss.str()<<endl;
        q->addJob(j1);
    }

    boost::shared_ptr<Job> j1 = boost::make_shared<Job>();
    j1->setName("AA");
    
    boost::shared_ptr<Job> j2 = boost::make_shared<Job>();
    j2->setName("BB");

    q->addJob(j1);
    q->addJob(j2);

    printf("MAIN::adding job\n");
    tp->startAll();
    //tp->start();

    //sleep(1);
    j1 = boost::shared_ptr<Job>();
    j2 = boost::shared_ptr<Job>();

    sleep(1000);
    //tp->destoryPool();
    //pthread_exit(NULL);
    delete tp;
  
    return 0;
}
