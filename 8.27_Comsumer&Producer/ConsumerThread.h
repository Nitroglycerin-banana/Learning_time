#ifndef _ConsumerThread_H_
#define _ConsumerThread_H_

#include "Thread.h"
#include "TaskQueue.h"
#include <unistd.h>
#include <iostream>
using std::cout;
using std::endl;
class ConsumerThread
:public byk::Thread
{
public:
    ConsumerThread(byk::TaskQueue& q)
        :_que(q)
    {}
    private:
    void run() override
    {
        int cnt=20;
        while(cnt-->0){
            int number=_que.pop();
            sleep(1);
            cout<<">> Consume a number."<<number<<endl;
        }
    }
    private:
    byk::TaskQueue& _que;
};
#endif

