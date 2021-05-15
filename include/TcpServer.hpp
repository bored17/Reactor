#ifndef ___TCPSERVER_H__
#define ___TCPSERVER_H__
#include "acceptor.hpp"
#include "eventLoop.hpp"
#include "tcpConnection.hpp"
namespace dog
{
    class TcpServer 
    {
    public:
        TcpServer(const string & ip, unsigned short port)
            :_acceptor(ip, port),
             _loop(_acceptor)
        {}
        void start()
        {
            _acceptor.ready(128);
            _loop.loop();
        }

        void stop()
        {
            _loop.unloop();
        }



        void setAllcallbacks(TcpConnectionCallback && onConnection, TcpConnectionCallback && onMessage, TcpConnectionCallback && onClosed)
        {
            _loop.setConnectionCallback(std::move(onConnection));
            _loop.setMessageCallback(std::move(onMessage));
            _loop.setCloseCallback(std::move(onClosed));
        }
    
    protected:
        
    
    private:
        Acceptor _acceptor;
        EventLoop _loop;
        
    };
}
#endif
