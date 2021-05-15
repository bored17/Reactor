#include "./Thread.hpp"
#include <stdio.h>


Thread::~Thread()
{
    if(_isRunning) 
    {
        pthread_detach(_pthid);
    }
}

void Thread::start()
{
    if (pthread_create(&_pthid, NULL, threadFunc, this)) 
    {
        perror("pthread_create");
    }
      	 
    _isRunning = true;
}

void Thread::join()
{
    if(_isRunning) 
    {
        pthread_join(_pthid, nullptr);
        _isRunning = false;
    }
}

void* Thread::threadFunc(void* arg)
{
    Thread *dog = static_cast<Thread*>(arg);
    if(dog) 
    {
        dog->_cb();
    }
    pthread_exit(nullptr);
}


