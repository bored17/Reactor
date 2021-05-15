#ifndef __SOCKET_H__
#define __SOCKET_H__
#include <sys/socket.h>
#include "nonCopyAble.hpp"
namespace dog
{
    class Socket : public NonCopyAble
    {
    public:
        Socket();
        Socket(int fd);
        ~Socket();

        int fd() const
        {
            return _fd;
        }

        void shutdownWrite();
    
    protected:
        
    
    private:
        int _fd;
        
    };
}//end of namespace dog;
#endif
