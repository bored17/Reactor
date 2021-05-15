#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__


#include "nonCopyAble.hpp"
#include "socket.hpp"
#include "socketIO.hpp"
#include "InetAddress.hpp"
#include "eventLoop.hpp"
#include <string>
#include <memory>
#include <functional>

using std::string;


namespace dog
{
class TcpConnection;
        using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
        using TcpConnectionCallback =  std::function<void(const TcpConnectionPtr &)>;
    class TcpConnection : NonCopyAble , public std::enable_shared_from_this<TcpConnection>
    {
    public:
        TcpConnection(int fd, EventLoop * loop);
        string receive();
        void send(const string & msg);
        void sendInLoop(const string & msg);

        string toString() const;
        bool isClosed() const;

        void setConnectionCallback(const TcpConnectionCallback & cb);
        void setMessageCallback(const  TcpConnectionCallback & cb);
        void setCloseCallback(const TcpConnectionCallback & cb);

        void handleConnectionCallback();
        void handleMessageCallback();
        void handleCloseCallback();
        
    private:
        InetAddress getLockAddr();
        InetAddress getPeerAddr();
    protected:
    
    private:
        EventLoop * _loop;
        Socket _socket;
        InetAddress _lockAddr;
        InetAddress _peerAddr;
        SocketIO _sockeIO;
        bool _isShutdownWrite;


        TcpConnectionCallback _onConnectionCb;
        TcpConnectionCallback _onMessageCb;
        TcpConnectionCallback  _onCloseCb;
        
    };
}
#endif
