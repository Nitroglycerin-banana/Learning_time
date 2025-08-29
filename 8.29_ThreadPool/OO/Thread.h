#ifndef _Thread_H_
#define _Thread_H_

#include "Noncopyable.h"
#include <pthread.h>

namespace wgy{
class Thread
    :public Noncopyable
{
public:
    Thread();
    virtual ~Thread(){};
    void start();
    void join();
private:
    virtual void run()=0;
    static void* start_routine(void*);
private:
    pthread_t _pthid;
    bool _isRunning;
};
}
#endif

