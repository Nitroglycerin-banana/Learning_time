#ifndef _TaskQueue_H_
#define _TaskQueue_H_

#include "Task.h"
#include "Condition.h"
#include "MutexLock.h"

#include <queue>
using std::queue;
using std::size_t;

namespace wgy{
using ElemType=Task::CallBack;
class TaskQueue
{
public:
    TaskQueue(size_t sz);
    bool empty() const;
    bool full() const;
    void push(ElemType&&);
    ElemType pop();
    void wakeup();
private:
    queue<ElemType> _que;
    size_t _queSize;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
    bool _flag;
};
}

#endif

