#ifndef _ProducerThread_H_
#define _ProducerThread_H_

#include "TaskQueue.h"
#include "Thread.h"
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;

class ProducerThread:
    public byk::Thread
{
public:
    ProducerThread(byk::TaskQueue& q)
        :_que(q)
    {}
private:
    void run() override
    {
        srand(time(nullptr));
              int cnt=20;
              while(cnt-->0){
              int number=rand()%100;
              _que.push(number);
              cout<<"make a number."<<number<<endl;
              sleep(1);
              }
    }
    private:
    byk::TaskQueue& _que;
};
#endif

