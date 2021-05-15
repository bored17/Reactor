#include "eventLoop.hpp"
#include "acceptor.hpp"
#include "tcpConnection.hpp"
#include <unistd.h>
#include <iostream>
#include <sys/eventfd.h>
using std::cout;
using std::endl;
namespace dog
{

    EventLoop::EventLoop(Acceptor & acceptor)
        :_efd(createEpollfd()),
        _eventfd(createEventFd()),
        _acceptor(acceptor),
        _isLooping(false),
        _evtlist(1024)
    {
        addEpollReadFd(acceptor.listenFd());
        addEpollReadFd(_eventfd);
    }

    EventLoop::~EventLoop()
    {
        if(_efd) 
        {
          	close(_efd);
        }
    }

    int EventLoop::createEpollfd()
    {
        int fd = epoll_create(1);
        if(fd < 0) 
        {
          	perror("epoll_create");
        }
        return fd;
    }


    void EventLoop::loop()
    {
        _isLooping = true;
        while(_isLooping) 
        {
            waitEpollfd();
        }
    }
    void EventLoop::unloop()
    {
        _isLooping = false;
    }


    void EventLoop::runInLoop(Functor && cb)
    {
        //1.打包回调函数， 注册给IO线程
        {
        MutexLockGuard autolock(_mutex);
        _pendingFunctiors.push_back(std::move(cb));
        }
        //2.通知IO线程进行发送
        wakeup();
    }


    void EventLoop::waitEpollfd()
    {
        int nready = 0;
        do {
            nready = ::epoll_wait(_efd,  &*_evtlist.begin(), _evtlist.size(), 5000);
        } while (nready == -1 && errno == EINTR);

        if(nready == -1) 
        {
            perror("epoll_wait");
            return;
        }
        else if(nready == 0) 
        {
            cout << "epoll_wait timeout" << endl;
        }
        else
        {
            //nready > 0
            if(nready ==  _evtlist.size()) 
            {
                _evtlist.resize(2 * nready);
            }

            for(int idx = 0; idx < nready; ++idx) 
            {
                int fd  = _evtlist[idx].data.fd;
                if(fd == _acceptor.listenFd() && _evtlist[idx].events & EPOLLIN) 
                {
                    handleNewConnection();
                }
                else if(fd == _eventfd) 
                {

                    handleRead();
                    doPendingFunctors();
                }
                else
                {
                    if(_evtlist[idx].events & EPOLLIN) 
                    {
                        handleMessage(_evtlist[idx].data.fd);
                    }
                }
            }
        }


    }
    void EventLoop::handleNewConnection()
    {
        int peerfd = _acceptor.accept();
        addEpollReadFd(peerfd);
        TcpConnectionPtr conn(new TcpConnection(peerfd, this));


        conn->setConnectionCallback(_onConnectionCb);
        conn->setMessageCallback(_onMessageCb);
        conn->setCloseCallback(_onCloseCb);

        _conns.insert(std::make_pair(peerfd, conn));


        conn->handleConnectionCallback();


    }
    void EventLoop::handleMessage(int fd)
    {
        //1. 通过fd 找到 TcpConnection
        auto iter = _conns.find(fd);
        if(iter != _conns.end()) 
        {
            //2.判断该链接是否断开
            bool isClosed = iter->second->isClosed();
            //1.如果连接断开， 执行连接断开时的事件处理
            if(isClosed) 
            {
                iter->second->handleCloseCallback();
                delEpollReadFd(fd);
                _conns.erase(iter);
            }
            //2.如果没有断开，执行消息到达处理器
            else
            {
                iter->second->handleMessageCallback();
            }
        }
    }



    void EventLoop::addEpollReadFd(int fd)
    {
        struct epoll_event ev;
        ev.events = EPOLLIN;
        ev.data.fd = fd;
        int ret = ::epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &ev);
        if(ret == -1) 
        {
            perror("epoll_ctl");
        }
    }

    void EventLoop::delEpollReadFd(int fd)
    {
        struct epoll_event ev;
        ev.data.fd = fd;
        int ret = ::epoll_ctl(_efd, EPOLL_CTL_DEL, fd, &ev);
        if(ret == -1) 
        {
            perror("epoll_ctl");
        }
    }

    void EventLoop::setConnectionCallback(TcpConnectionCallback && cb)
    {
        _onConnectionCb = std::move(cb);
    }


    void EventLoop::setMessageCallback(TcpConnectionCallback && cb)
    {
        _onMessageCb = std::move(cb);
    }
    void EventLoop::setCloseCallback(TcpConnectionCallback && cb)
    {
        _onCloseCb = std::move(cb);
    }


    int EventLoop::createEventFd()
    {
        int fd = eventfd(0, 0);
        if(fd < 0) 
        {
          	perror("eventfd");
        }
        return fd;
    }


    void EventLoop::wakeup()
    {
        uint64_t one = 1;
        int ret = write(_eventfd, &one, sizeof(one));
        if(ret != sizeof(one) ) 
        {
            perror("write");
        }
    }


    void EventLoop::handleRead()
    {
        uint64_t howmay = 0; 
        int ret = read(_eventfd, &howmay, sizeof(howmay));
        if(ret != sizeof(howmay)) 
        {
            perror("read");
        }
    }


    void EventLoop::doPendingFunctors()
    {
        std::vector<Functor> tmp;
        {
        MutexLockGuard autoLock(_mutex);
        tmp.swap(_pendingFunctiors);
        }
        for(auto & cb : tmp)
        {
            cb();
        }
    }
}

