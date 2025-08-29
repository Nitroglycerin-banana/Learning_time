#ifndef _WorkerThread_H_
#define _WorkerThread_H_

#include "Thread.h"
#include "ThreadPool.h"

namespace wgy{
class WorkerThread
    :public Thread
{
public:
WorkerThread(ThreadPool& tl)
    :_threadpool(tl)
{}

private:
void run() override
{
    _threadpool.doTask();
}
private:
ThreadPool& _threadpool;
};

}
#endif

