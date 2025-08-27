#include <iostream>
#include "TaskQueue.h"
#include "ProducerThread.h"
#include "ConsumerThread.h"
#include <memory>
using std::cout;
using std::endl;
using std::unique_ptr;

int main()
{
    byk::TaskQueue taskque(10);
    unique_ptr<byk::Thread> producer(new ProducerThread(taskque));
    unique_ptr<byk::Thread> consumer(new ConsumerThread(taskque));
    producer->start();
    consumer->start();
    producer->join();
    consumer->join();
    return 0;
}


