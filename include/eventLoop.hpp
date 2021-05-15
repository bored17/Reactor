#ifndef __EVENTLOOP_H__
#define __EVENTLOOP_H__
#include <vector>
#include <map>
#include <memory>
#include <sys/epoll.h>
#include <unistd.h>
#include <functional>
#include "mutelock.hpp"

namespace dog
{
    class Acceptor;
    class TcpConnection;

        using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
        using TcpConnectionCallback =  std::function<void(const TcpConnectionPtr &)>;
        using Functor = std::function<void()>;
    class EventLoop 
    {
        using EventList =  std::vector<struct epoll_event>;
        using TcpConnsMap = std::map<int, TcpConnectionPtr>;
    public:
        EventLoop(Acceptor &);
        ~EventLoop();

        void loop();
        void unloop();
        void runInLoop(Functor &&);


        void waitEpollfd();
        void handleNewConnection();
        void handleMessage(int);

    
        int createEpollfd();

        void addEpollReadFd(int fd);
        void delEpollReadFd(int fd);
        void setConnectionCallback(TcpConnectionCallback && cb);
        void setMessageCallback(TcpConnectionCallback && cb);
        void setCloseCallback( TcpConnectionCallback && cb);

        int createEventFd();
        void wakeup();
        void handleRead();

        void doPendingFunctors();


    protected:
    
    private:

        int _efd;
        int _eventfd;
        Acceptor & _acceptor;
        bool _isLooping;
        EventList _evtlist;
        TcpConnsMap _conns;


        TcpConnectionCallback _onConnectionCb;
        TcpConnectionCallback _onMessageCb;
        TcpConnectionCallback  _onCloseCb;


        std::vector<Functor> _pendingFunctiors;
        MutexLock _mutex;
    };
}

#endif
