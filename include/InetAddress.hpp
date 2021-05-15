#ifndef __INEADDRESS_H__
#define __INEADDRESS_H__
    
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
using std::string;

namespace dog
{
    class InetAddress
    {
        public:
            InetAddress(const string & IP, unsigned short PORT);
            InetAddress(const struct sockaddr_in & addr);

            string ip() const;
            unsigned short port() const;
            const struct sockaddr_in * getInetAddressPtr() const;

        private:
            struct sockaddr_in _addr;
    };
}
#endif


