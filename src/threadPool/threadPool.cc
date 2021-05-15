#include "threadPool.hpp"


#include <unistd.h>


namespace dog
{
    ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
        : _threadNum(threadNum),
        _queSize(queSize),
        _taskque(_queSize),
        _isExit(false)
    {
        _threads.reserve(_threadNum);
    }

    ThreadPool::~ThreadPool()
    {
        if(!_isExit) 
        {
            stop(); 
        }
    }


    void ThreadPool::start()
    {
        for(int i = 0; i !=  _threadNum; ++i) 
        {
            unique_ptr<Thread> up(new Thread(std::bind(&ThreadPool::threadFunc, this)));
            _threads.push_back(std::move(up));
        }

        for(auto & pthread : _threads)
        {
            pthread->start();
        }
    }


    void ThreadPool::addTask(ElemType && cb)
    {
        if(cb) 
        {
            _taskque.push(std::move(cb));
        }
    }


    ElemType  ThreadPool::getTask()
    {
        return _taskque.pop();
    }

    void ThreadPool::stop()
    {
        while (!_taskque.empty()) 
        {
            sleep(1);
        }
        _isExit = true;
        _taskque.wakeAll();
        for(auto & pthread : _threads)
        {
            pthread->join();
        }
    }

    void ThreadPool::threadFunc()
    {
        while(!_isExit) 
        {
            ElemType task = getTask();
            if(task) 
            {
              	task(); 
            }
        }
    }
}

