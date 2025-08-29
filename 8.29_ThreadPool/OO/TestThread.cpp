#include "ThreadPool.h"
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <memory>
using std::unique_ptr;

class MyTask
:public wgy::Task
{
public:
    void process() override
    {
        int number=rand()%100;
        printf("make a number %d\n",number);
        sleep(1);
    }
};

int main()
{
    srand(time(nullptr));
    wgy::ThreadPool threadpool(4,10);
    threadpool.start();
    unique_ptr<wgy::Task> ptask(new MyTask);
    int cnt=20;
    while(cnt-->0)
    {
        threadpool.addTask(ptask.get());
    }
    threadpool.stop();

}


