#include "InetAddress.hpp"
#include <string.h>

namespace dog
{
    InetAddress::InetAddress(const string & IP, unsigned short PORT) 
    {
        bzero(&_addr, sizeof(struct sockaddr_in));
        _addr.sin_family = AF_INET;
        _addr.sin_addr.s_addr = inet_addr(IP.c_str());
        _addr.sin_port = htons(PORT);
    }

    InetAddress::InetAddress(const struct sockaddr_in & addr)
        :_addr(addr)
    {}



    string InetAddress::ip() const
    {
        return string(inet_ntoa(_addr.sin_addr));//注意下--
    }

    unsigned short InetAddress::port() const
    {
        return ntohs(_addr.sin_port);
    }
    
    const struct sockaddr_in * InetAddress::getInetAddressPtr() const
    {
        return &_addr;
    }
}


