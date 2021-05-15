#include "taskQueue.hpp"

namespace dog
{
    TaskQueue::TaskQueue(size_t sz)
    : _queSize(sz)
    , _que()
    , _mutex()
    , _notEmpty(_mutex)
    , _notFull(_mutex)
    , _flag(true)
    {}


    bool TaskQueue::empty() const
    {
        return _que.size() == 0;
    }

    bool TaskQueue::full() const
    {
        return _que.size() == _queSize; 
    }

    void TaskQueue::push(const ElemType & e)
    {
        MutexLockGuard autolock(_mutex);
        /* _mutex.lock(); */
       while(full()) 
        {
            _notFull.wait();
        }
       _que.push(e);

       _notEmpty.notify();//通知消费者线程取数据
        /* _mutex.unlock(); */

    }
   ElemType TaskQueue::pop()
    {
        MutexLockGuard autolock(_mutex);
#if 1
       while(_flag && empty()) 
        {
            _notEmpty.wait();
        }

       if(_flag) 
       {
       //从队列中获取数据
        ElemType tmp = _que.front();
        _que.pop();
        _notFull.notify();
        return tmp;
       }
       else {
           return nullptr;
       }
#endif

//又卡死的bug
//要改一下
#if 0
       while(empty()) 
        {
            _notEmpty.wait();
        }

       //从队列中获取数据
        ElemType tmp = _que.front();
        _que.pop();
        _notFull.notify();
        return tmp;
    
#endif

    }


   void TaskQueue::wakeAll()
   {
       _flag = false;
       _notEmpty.notifyall();
   }
}

