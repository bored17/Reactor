#include "conditon.hpp"

namespace dog
{
Conditon::Conditon(MutexLock &mutex):_mutex(mutex)
{
    pthread_cond_init(&_cond, NULL);
}

Conditon::~Conditon()
{
    pthread_cond_destroy(&_cond);
}

void Conditon::wait()
{
    pthread_cond_wait(&_cond, _mutex.getPmutex());
}

void Conditon::notify()
{
    pthread_cond_signal(&_cond);
}

void Conditon::notifyall()
{
    pthread_cond_broadcast(&_cond);
}

}

