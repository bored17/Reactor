#include "mutelock.hpp"
#include <stdio.h>


namespace dog
{
MutexLock::MutexLock()
{
    if(pthread_mutex_init(&_mutex,nullptr)) 
    {
        perror("pthread_mutex_init");
    }
}

pthread_mutex_t* MutexLock::getPmutex()
{
    return &_mutex;
}

MutexLock::~MutexLock()
{
    if(pthread_mutex_destroy(&_mutex)) 
    {
        perror("pthread_mutex_destroy");
    }
}

void MutexLock::lock()
{
    
    if(pthread_mutex_lock(&_mutex))
    {
        perror("pthread_mutex_destroy");
    }
}

void MutexLock::unlock()
{
    if(pthread_mutex_unlock(&_mutex)) 
    {
        perror("pthread_mutex_unlock");
    }
}
}
