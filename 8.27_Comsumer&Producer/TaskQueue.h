#ifndef _TaskQueue_H_
#define _TaskQueue_H_


#include "MutexLock.h"
#include "Condition.h"
#include <queue>
using std::queue;
using std::size_t;

namespace byk{
class TaskQueue{
public:
    TaskQueue(size_t sz);
    bool empty() const;
    bool full() const;
    void push(int);
    int pop();
    private:
    queue<int> _que;
    size_t _queSize;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
};
}
#endif

