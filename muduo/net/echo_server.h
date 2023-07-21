#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H

#include "base/head.h"
#include "callback.h"
#include "event_loop.h"
#include "inet_address.h"
#include "tcp_server.h"

namespace muduo
{
namespace net
{

class EchoServer
{
public:
    EchoServer(EventLoop* eventLoop, const InetAddress& listenAddr);

    void start(){
        m_tcpServer.start();
    };

private:
    void on_connection(const TcpConnection& conn);

private:
    TcpServer m_tcpServer;


};

}
}


#endif