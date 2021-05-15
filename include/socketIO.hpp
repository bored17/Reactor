#ifndef __SOCKEIO_H__
#define __SOCKEIO_H__
namespace dog
{
    class SocketIO 
    {
    public:
        SocketIO(int fd): _fd(fd) {}

        int readn(char * buff, int len);
        int readline(char * buff, int maxlen);
        int writen(const char * buff, int len);
        
    protected:
        
    
    private:
        int _fd;
        
    };
}
#endif
