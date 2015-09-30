#include <unistd.h>  //for sleeping
#include <iostream>
#include <sstream>
#include <boost/log/trivial.hpp>
#include <boost/make_shared.hpp>
#include "ThreadPool.h"
#include "singleton.h"
#include "queue.h"
//#include <boost/log/core.hpp>
//#include <boost/log/expressions.hpp>

namespace logging = boost::log;

int main(){
    
    BOOST_LOG_TRIVIAL(info)<<"---------main start-----------";

    Queue* q = Singleton<Queue>::getInstance();
    boost::shared_ptr<ThreadPool> tp = boost::make_shared<ThreadPool>();

    tp->grow(15);
    tp->grow(20);
    
    int i;
    for(i = 100; i<1100; i++){
        std::stringstream ss;
        ss<<i<<endl;
        
        boost::shared_ptr<Job> j1 = boost::make_shared<Job>();
        j1->setName(ss.str());
        BOOST_LOG_TRIVIAL(debug)<<"the job name: "<<ss.str();
        q->addJob(j1);
    }

    boost::shared_ptr<Job> j1 = boost::make_shared<Job>();
    j1->setName("AA");
    
    boost::shared_ptr<Job> j2 = boost::make_shared<Job>();
    j2->setName("BB");

    q->addJob(j1);
    q->addJob(j2);

    BOOST_LOG_TRIVIAL(info)<<"main: job added, waiting to start";
    tp->startAll();
    //tp->start();

    //sleep(1);
    j1 = boost::shared_ptr<Job>();
    j2 = boost::shared_ptr<Job>();

    sleep(1000);
    tp->destoryPool();
    //pthread_exit(NULL);
  
    return 0;
}
