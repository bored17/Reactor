    
#include "acceptor.hpp"
#include "string.h"
namespace dog
{
    Acceptor::Acceptor(const string & ip, unsigned short port)
        : _socket(), _addr(ip, port)
    {}

    void Acceptor::setReuseAddr(bool flag)
    {
        if(flag) 
        {
            int reust = 1;
            if(setsockopt(_socket.fd(),SOL_SOCKET, SO_REUSEADDR, &reust, sizeof(int)))
            {
                perror("setsockopt");
            }
        }
    }

    void Acceptor::setReusePort(bool flag)
    {
        if(flag) 
        {
            int reust = 1;
            
           if(setsockopt(_socket.fd(),SOL_SOCKET, SO_REUSEADDR, &reust, sizeof(int)))
           {
               perror("setsockopt");
           }
        }
    }



        
        void Acceptor::bind()
        {
            if(::bind(_socket.fd(), (sockaddr *)_addr.getInetAddressPtr(), sizeof(struct sockaddr_in)))
            {
                perror("bind");
            }
        }

        void Acceptor::listen(int backlog)
        {
            if(::listen(_socket.fd(), backlog))
            {
                perror("listen");
            }
        }

        int Acceptor::accept()
        {
           int newFd =  ::accept(_socket.fd(), nullptr, nullptr);
           if(newFd == -1) 
           {
             	perror("accept");
           }

           return newFd;
        }

        int  Acceptor::listenFd() const
        {
            return _socket.fd();
        }

        void Acceptor::ready(int backlog)
        {
            setReuseAddr(true);
            setReuseAddr(true);
            bind();
            listen(backlog);
        }

}

