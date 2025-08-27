#include "TaskQueue.h"

namespace byk{
TaskQueue::TaskQueue(size_t sz)
    :_que()
     ,_queSize(sz)
     ,_mutex()
     ,_notEmpty(_mutex)
     ,_notFull(_mutex)
{}
bool TaskQueue::empty() const
{
    return _que.size()==0;
}
bool TaskQueue::full() const
{
    return _que.size()==_queSize;
}

void TaskQueue::push(int elem)
{
    _mutex.lock();
    while(full())
    {
        _notFull.wait();
    }
    _que.push(elem);
    _notEmpty.notify();
    _mutex.unlock();
}

int TaskQueue::pop()
{
    _mutex.lock();
    while(empty())
    {
        _notEmpty.wait();
    }
    int tmp=_que.front();
    _que.pop();
    _notFull.notify();
    _mutex.unlock();
    return tmp;
}
}

