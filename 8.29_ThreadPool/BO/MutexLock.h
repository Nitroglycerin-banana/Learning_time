#ifndef _MutexLock_H_
#define _MutexLock_H_

#include "Noncopyable.h"
#include <pthread.h>

namespace wgy{
class MutexLock
    :public Noncopyable
{
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();

    pthread_mutex_t* getMutexLockPtr(){return &_mutex;}
private:
    pthread_mutex_t _mutex;
};
}
#endif

