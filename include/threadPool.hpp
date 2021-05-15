#ifndef _THREADPOOL_H__
#define _THREADPOOL_H__
#include "Thread.hpp"
#include "taskQueue.hpp"
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

namespace dog 
{
class ThreadPool 
{
    friend class WorkerThread;
public:
    ThreadPool(size_t threadNum, size_t queSize);
    ~ThreadPool();
    void start();
    void stop();
    void addTask(ElemType &&);
private:
    ElemType getTask();

    void threadFunc();

protected:
    

private:
    size_t _threadNum;
    size_t _queSize;

    vector<unique_ptr<Thread>> _threads;

    TaskQueue _taskque;
    bool _isExit;

};
    
}
#endif
