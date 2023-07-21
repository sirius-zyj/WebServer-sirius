#include "echo_server.h"

using namespace muduo::net;

using std::placeholders::_1;

EchoServer::EchoServer(EventLoop* eventLoop, const InetAddress& listenAddr)
:m_tcpServer(listenAddr)
{
    // m_tcpServer.set_connection_callback(
    //     std::bind(&EchoServer::on_connection, this, _1)
    // );

}

void EchoServer::on_connection(const TcpConnection &conn)
{
    
}
