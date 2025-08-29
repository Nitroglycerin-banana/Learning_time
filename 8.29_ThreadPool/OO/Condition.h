#ifndef _Condition_H_
#define _Condition_H_

#include "Noncopyable.h"
#include <pthread.h>
namespace wgy{
class MutexLock;

class Condition
    :public Noncopyable
{
public:
    Condition(MutexLock&);
    ~Condition();

    void wait();
    void notify();
    void notifyAll();

private:
    pthread_cond_t _cond;
    MutexLock& _mutex;
};
}

#endif

