#include "inet_address.h"

using namespace muduo::net;

InetAddress::InetAddress(uint16_t port)
{
    //默认为IPV4
    memset(&m_addr, 0, sizeof(m_addr));
    m_addr.sin_family = AF_INET;
    m_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_addr.sin_port = htons(port);
}