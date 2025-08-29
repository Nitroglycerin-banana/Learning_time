#include "ThreadPool.h"

#include "WorkerThread.h"
#include <stdio.h>
#include <unistd.h>

namespace wgy{
ThreadPool::ThreadPool(int threadNum,int queSize)
    :_threads()
     ,_threadNum(threadNum)
     ,_taskque(queSize)
     ,_isExit(false)
{
    _threads.reserve(_threadNum);
}
ThreadPool::~ThreadPool()
{
    if(!_isExit)
    {
        stop();
    }
}

void ThreadPool::start()
{
    for(size_t i=0;i<_threadNum;++i)
    {
        unique_ptr<Thread> up(new WorkerThread(*this));
        _threads.push_back(std::move(up));
    }
    for(auto& pthread:_threads)
    {
        pthread->start();
    }
}

void ThreadPool::addTask(Task* ptask)
{
    if(ptask)
    {
        _taskque.push(ptask);
    }
}

void ThreadPool::stop()
{
    while(!_taskque.empty())
    {
        sleep(1);
    }
    _isExit=true;
    _taskque.wakeup();
    printf("stop:wakeup all thread.\n");
    for(auto& pthread:_threads)
    {
        pthread->join();
    }
}

void ThreadPool::doTask()
{
    while(!_isExit)
    {
        Task* ptask=_taskque.pop();
        if(ptask)
        {
            ptask->process();
        }
    }
}











}
