#ifndef _ThreadPool_H_
#define _ThreadPool_H_

#include "Task.h"
#include "Thread.h"
#include "TaskQueue.h"

#include <memory>
#include <vector>

using std::vector;
using std::unique_ptr;

namespace wgy{
class ThreadPool{
    friend class WorkerThread;
public:
    ThreadPool(int,int);
    ~ThreadPool();
    void start();
    void stop();
    void addTask(Task*);
private:
    void doTask();
private:
    vector<unique_ptr<Thread>> _threads;
    size_t _threadNum;
    TaskQueue _taskque;
    bool _isExit;

};
}


#endif

