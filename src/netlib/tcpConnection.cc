#include "tcpConnection.hpp"
#include "eventLoop.hpp"
#include <sstream>
#include <iostream>


namespace dog
{
    TcpConnection::TcpConnection(int fd, EventLoop * loop)
        :_socket(fd),
        _sockeIO(fd),
        _lockAddr(getLockAddr()),
        _peerAddr(getPeerAddr()),
        _isShutdownWrite(false),
        _loop(loop)
    {}

    string TcpConnection:: receive()
    {
        char buff[65535] = {0};
        _sockeIO.readline(buff, sizeof(buff));
        return string(buff);
    }

    void TcpConnection::send(const string & msg)
    {
        _sockeIO.writen(msg.c_str(), msg.size());
    }


    void TcpConnection::sendInLoop(const string & msg)
    {
        if(_loop) 
        {
            _loop->runInLoop(std::bind(&TcpConnection::send, this, msg));
        }
    }
    string TcpConnection::toString() const
    {
        std::ostringstream oss;
        oss << _lockAddr.ip() << ":" << _lockAddr.port() << "-->"
            << _peerAddr.ip() << ":" << _peerAddr.port();
        return oss.str();
    }

        
    InetAddress TcpConnection:: getLockAddr()
    {
        struct sockaddr_in addr;
        socklen_t len  = sizeof(struct sockaddr_in);
        if(getsockname(_socket.fd(), (struct sockaddr *)&addr, &len) == -1 ) 
        {
            perror("getsockname");
        }
        return InetAddress(addr);
    }

    InetAddress TcpConnection::getPeerAddr()
    {
        struct sockaddr_in addr;
        socklen_t len  = sizeof(struct sockaddr_in);
        if(getpeername(_socket.fd(), (struct sockaddr *)&addr, &len) == -1 ) 
        {
            perror("getsockname");
        }
        return InetAddress(addr);

    }
        void TcpConnection::setConnectionCallback(const TcpConnectionCallback & cb)
        {
            _onConnectionCb = std::move(cb);
        }

        
        void TcpConnection::setMessageCallback(const TcpConnectionCallback & cb)
        {
            _onMessageCb = std::move(cb);
        }
        void TcpConnection::setCloseCallback(const TcpConnectionCallback & cb)
        {
            _onCloseCb = std::move(cb);
        }

        bool TcpConnection::isClosed() const
        {
            int nret = -1;
            char buff[128] = {0};
            do {
                nret = ::recv(_socket.fd(), buff, sizeof(buff), MSG_PEEK);
            } while (nret == -1 && errno == EINTR);

            return nret == 0;
        }

        void TcpConnection::handleConnectionCallback()
        {
            if(_onConnectionCb) 
            {
              	_onConnectionCb(shared_from_this()); 
            }
        }

        void TcpConnection::handleMessageCallback()
        {
            if(_onMessageCb) 
            {
              	_onMessageCb(shared_from_this()); 
            }
        }
        void TcpConnection:: handleCloseCallback()
        {
            if(_onCloseCb) 
            {
              	_onCloseCb(shared_from_this());
            }
        }
}
