#include "socket.hpp"
#include <stdio.h>
#include <unistd.h>
/* #include <sys/types.h> */

namespace dog
{
    Socket::Socket()
        :_fd(socket(AF_INET, SOCK_STREAM, 0))
    {
        if(_fd < 0) 
        {
            perror("socket");
        }
    }

    Socket::Socket(int fd)
        :_fd(fd)
    {
    }


    Socket::~Socket()
    {
        close(_fd);
    }

    void Socket::shutdownWrite()
    {
        shutdown(_fd, SHUT_WR);
    }
}//end of namesapce dog

