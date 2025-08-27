#ifndef _Condition_H_
#define _Condition_H_

#include <iostream>
#include <string>
#include "Noncopyable.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


namespace byk{
class MutexLock;//类的前向声明
class Condition:
    Noncopyable{
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

