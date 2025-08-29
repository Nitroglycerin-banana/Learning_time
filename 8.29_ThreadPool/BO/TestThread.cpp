#include "ThreadPool.h"
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <functional>
#include "Task.h"


#include <memory>
using std::unique_ptr;
using std::bind;

class MyTask
:public wgy::Task
{
public:
    void process()
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
    unique_ptr<MyTask> cb(new MyTask);

    int cnt=20;
    while(cnt-->0)
    {
        threadpool.addTask(bind(&MyTask::process,cb.get()));
    }
    threadpool.stop();

}


