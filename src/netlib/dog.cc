#include "acceptor.hpp"
#include "tcpConnection.hpp"
#include "eventLoop.hpp"
#include "TcpServer.hpp"
#include "threadPool.hpp"
#include <iostream>
#include <algorithm>
#include "readConfig.hpp"
 
using std::cout;
using std::endl;
using namespace dog;
class MyTask
{
    public:
        MyTask(const string & msg, TcpConnectionPtr conn)
            : _msg(msg),
              _conn(conn)
        {}

        //process方法在一个子线程中运行, 计算线程
        void process()
        {
            //decode
            //compute
            //encode
            //
            cout << "解析完毕, 开始回传" << endl;
            //假设是一个回写echo服务
            //如果要做到计算线程和IO线程的职责分离,不能在计算线程里面直接发送数据
            //采取的方案
            //1. 将send函数打包成一个回调函数, 注册给IO线程, 该操作只能交给TcpConnection对象实现.
            //2. 计算线程再通知IO线程进行数据的发送(怎么通知) //使用eventfd通知
            _conn->sendInLoop(_msg);//意味着TcpConnection对象知道Eventloop对象的存在
        }

    private:
        string _msg;
        TcpConnectionPtr _conn;
};


ThreadPool * gThreadpool = nullptr;

void onConnection(const TcpConnectionPtr & conn)
{
    cout << conn->toString()  << "has connected!"<<  endl;
}


void onMessage(const TcpConnectionPtr & conn)
{
    string msg = conn->receive();
    cout << " >> recv: " << msg;
    //简单的回传
    //decode
    //compute
    //encode
    /* conn->send(reutrnmsg); */

    //交给线程池运行
    MyTask task(msg, conn);
    gThreadpool->addTask(std::bind(&MyTask::process, task));
    
}

void onClose(const TcpConnectionPtr & conn)
{
    cout << conn->toString() << "has closed!" << endl;
}



void test1()
{
    //配置文件做出单例模式
    ThreadPool threadpool(4, 100);
    threadpool.start();
    gThreadpool = &threadpool;

    TcpServer server("127.0.0.1", 8888);
    server.setAllcallbacks(onConnection, onMessage, onClose);
    server.start();
}


class EchoServer 
{
    public:
        EchoServer(const string &ip, unsigned short port)
            :_threadpool(4, 1000)
             , _server(ip, port)
        {}
        void start()
        {
            using namespace std::placeholders;
            _threadpool.start();
            _server.setAllcallbacks(
                    std::bind(&EchoServer::onConnection, this, _1),
                    std::bind(&EchoServer::onMessage, this, _1),
                    std::bind(&EchoServer::onClose, this, _1));
            _server.start();
        }

        void stop()
        {
            
            _server.stop();
            _threadpool.stop();
        }



    private:
        void onConnection(const TcpConnectionPtr & conn)
        {
            cout << conn->toString() << "has connected! " <<  endl;
        }
        void onMessage(const TcpConnectionPtr & conn)
        {
            string msg = conn->receive();
            cout << ">>receive: " << msg ;
            MyTask task(msg, conn);
            _threadpool.addTask(std::bind(&MyTask::process, task));
        }
        void onClose(const TcpConnectionPtr & conn)
        {
            cout << conn->toString() << "has closed! " << endl;
        }


    private:
        ThreadPool _threadpool;
        TcpServer _server;

};

void test2()
{
    string confip = "ip";
    string confport = "port";
    string ip;
    string port;
    readConfigFile("./conf/myconf.conf", confip, ip);
    readConfigFile("./conf/myconf.conf", confport, port);
    
    EchoServer echo(ip, atoi(port.c_str()));
    echo.start();
}

int main(int argc, char **argv)
{
    test2();
    return 0;
}
