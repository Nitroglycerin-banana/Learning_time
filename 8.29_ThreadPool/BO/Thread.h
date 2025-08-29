#ifndef _Thread_H_
#define _Thread_H_

#include "Noncopyable.h"
#include <pthread.h>
#include "Task.h"

namespace wgy{
class Thread
    :public Noncopyable
{
public:
    Thread(Task::CallBack&& cb);
    ~Thread(){};
    void start();
    void join();
private:
    static void* start_routine(void*);
private:
    pthread_t _pthid;
    bool _isRunning;
    Task::CallBack  _cb;
};
}
#endif

