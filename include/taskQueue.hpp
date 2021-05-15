#ifndef __TASKQUEUE__H__
#define __TASKQUEUE__H__
#include "mutelock.hpp"
#include "conditon.hpp"
#include <queue>
#include <functional>

namespace dog
{
    using ElemType = std::function<void()>;
class TaskQueue 
{
public:
    TaskQueue(size_t size);
    ElemType  pop();
    void push(const ElemType &);
    bool empty() const;
    bool full() const;
   void wakeAll();
    

protected:
    

private:
    size_t _queSize;
    std::queue<ElemType> _que;
    MutexLock _mutex;
    Conditon _notFull;
    Conditon _notEmpty;
    bool _flag;
};

}
#endif
