#ifndef __PTHREAD_H__
#define __PTHREAD_H__
    
#include <pthread.h>
#include <functional>


using Callback = std::function<void()>;
class Thread 
{
public:
    Thread(Callback && cb)
        :_pthid(0)
         , _isRunning(false) 
         , _cb(std::move(cb))
    {}
    ~Thread();
    void start();
    void join();

    
private:
    static void* threadFunc(void*);

private:
    pthread_t _pthid;
    bool _isRunning;
    Callback _cb;

    
};
#endif



