#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__
    
#include "socket.hpp"
#include"InetAddress.hpp"
#include <string>
namespace dog
{
    class Acceptor 
    {
    public:
        Acceptor(const string & ip, unsigned short port);

        void setReuseAddr(bool);

        void setReusePort(bool);
        
        void bind();

        void listen(int backlog);

        int accept();

        int  listenFd() const;

        void ready(int backlog);

        
    protected:
        
    
    private:
        InetAddress _addr;
        Socket _socket;
        
    };

}

#endif
