#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__
#include "./noCopyalbe.hpp"
#include <pthread.h>


namespace dog
{
class MutexLock : Nocopyalbe 
{
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
    pthread_mutex_t* getPmutex();
protected:

private:
    pthread_mutex_t _mutex;
};


class MutexLockGuard 
{
public:
    MutexLockGuard(MutexLock & mutex)
        :_mutex(mutex)
    {
        _mutex.lock();
    }

    ~MutexLockGuard()
    {
        _mutex.unlock();
    }

    
private:
    MutexLock & _mutex;
};
}//end of namespace dog
#endif
