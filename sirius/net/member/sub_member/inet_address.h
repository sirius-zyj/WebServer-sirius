#ifndef INET_ADDRESS_H
#define INET_ADDRESS_H

#include <netinet/in.h>

#include "plugin/head.h"


namespace sirius
{

class InetAddress
{
public:
    explicit InetAddress(uint16_t port = 9006);

    explicit InetAddress(const struct sockaddr_in& addr){
        m_addr = addr;
    };

    sa_family_t family() const { return m_addr.sin_family; }
    sockaddr_in address() const {return m_addr;}
private:
    struct sockaddr_in m_addr; 

};


}



#endif