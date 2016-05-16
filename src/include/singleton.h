/*
 * singleton.cpp
 * Created By: Matthew Gao
 * Copyright (c) Dell.Inc
 * Created Time: 2015年09月24日 星期四 00时23分01秒
 */

#include<iostream>
#include<pthread.h>

using namespace std;

template <class T>
class Singleton{
  public:
    static T* getInstance(){
        pthread_once(&once, Singleton::init);
        return obj;
    }

  private:
    static void init(){
        obj = new T();
    }

    Singleton(){}
    ~Singleton(){}
    static T* obj;
    static pthread_once_t once;
};

template <class T>
pthread_once_t Singleton<T>::once = PTHREAD_ONCE_INIT;

template <class T>
T* Singleton<T>::obj = NULL;



/*int main(){*/
    //int* i = Singleton<int>::getInstance();
    //cout<<i<<endl;
    //int* j = Singleton<int>::getInstance();
    //cout<<j<<endl;
    //return 0;
/*}*/
