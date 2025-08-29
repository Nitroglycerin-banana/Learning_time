#include "TaskQueue.h"
#include "MutexLock.h"

namespace wgy{

TaskQueue::TaskQueue(size_t sz)
    :_que()
     ,_queSize(sz)
     ,_mutex()
     ,_notEmpty(_mutex)
     ,_notFull(_mutex)
     ,_flag(true)
{}

bool TaskQueue:: empty() const
{
    return _que.size()==0;
}

bool TaskQueue:: full() const
{
    return _que.size()==_queSize;
}

void TaskQueue::push(ElemType elem)
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

ElemType TaskQueue::pop()
{
    _mutex.lock();
    while(empty()&&_flag)
    {
        _notEmpty.wait();
    }
    if(_flag){
    ElemType tmp=_que.front();
    _que.pop();
    _notFull.notify();
    _mutex.unlock();
    return tmp;
    }else{
        _mutex.unlock();
        return nullptr;
    }    
}



void TaskQueue::wakeup()
{
    _flag=false;
    _notEmpty.notifyAll();
}
}

