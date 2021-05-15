#include "socketIO.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <iostream>

namespace dog
{
    int SocketIO::readn(char * buff, int len)
    {
        int left = len;
        char * p =buff;

        int ret = -1;
        while(left > 0) 
        {
            /* ret = recv(_fd, p, left, 0); */
            ret = read(_fd, p, left);
            if(ret == -1 && errno == EINTR) 
            {
                continue;
            }
            else if(ret == -1) 
            {
                perror("read");
                return len - left;
            }
            else if(ret == 0) 
            {
                break; 
            }
            else
            {
                p +=ret;
                left -= ret;
            }
        }
        return len - left;
    }

    int SocketIO::readline(char * buff, int maxlen)
    {
        int left = maxlen - 1;
        char *p = buff;
        int ret = -1;
        int total = 0;
        while(left > 0) 
        {
            ret =recv(_fd, p, left, MSG_PEEK);
            if(ret == -1 && errno == EINTR) 
            {
                continue;
            }
            else if(ret == -1) 
            {
                perror("recv");
            }
            else if(ret ==0) 
            {
                break; 
            }
            else
            {
                for(int idx = 0; idx < ret; ++idx) 
                {
                    if(p[idx] == '\n') 
                    {
                        int sz = idx + 1;
                        readn(p, sz);
                        p += sz;
                        *p = '\0';
                        return total + sz;
                    }
                }

                readn(p, ret);
                left -=ret;
                total +=ret;
                p += ret;
            }//end else
        }//end while
        *p = '\0';
        return total - left;
    }

    int SocketIO::writen(const char * buff, int len)
    {
        int left =len;
        const char * p = buff;

        int ret = -1;
        while(left > 0) 
        {
            /* ret = send(_fd, p, left, 0); */
            ret = write(_fd, p, left);
            if(ret == -1 && errno == EINTR) 
            {
                continue;
            }
            else if(ret == -1) 
            { 
                perror("send");
                return len - left;
            }
            else
            {
                p += ret;
                left -= ret;
            }
        }
        return len - left;
    }
}

