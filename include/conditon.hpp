#ifndef __CONDITON__INLCUDE__
#define __CONDITON__INLCUDE__
#include "mutelock.hpp"
#include "noCopyalbe.hpp"
#include <pthread.h>

namespace dog
{
class Conditon : Nocopyalbe
{
public:
    Conditon(MutexLock &mutex);
    ~Conditon();
    void wait();
    void notify();
    void notifyall();

protected:

private:
    pthread_cond_t _cond;
    MutexLock &_mutex;
};
}

#endif
