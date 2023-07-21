#include "acceptor.h"

using namespace muduo::net;


Acceptor::Acceptor(int epollFd, const InetAddress &listenAddr)
: m_acceptorFd(socket(listenAddr.family(), SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP)),//设置非阻塞
  m_acceptorChannel(epollFd, m_acceptorFd)
{
    //可复用地址
    int flag = 1;
    setsockopt(m_acceptorFd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    //绑定端口
    sockaddr_in addr = listenAddr.address();
    int ret = bind(m_acceptorFd, (struct sockaddr *)&addr, sizeof(addr));
    assert(ret >= 0);

    //将handleRead函数注册进m_acceptorChannel中
    m_acceptorChannel.set_read_callback(
        std::bind(&Acceptor::handle_read, this)
    );
}

void Acceptor::run_listen()
{
    //开启监听事件
    int ret = listen(m_acceptorFd, 256);
    assert(ret >= 0);
    m_acceptorChannel.enable_reading();
}

void Acceptor::handle_read()
{
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    //LT触发模式    
    int connFd = accept(m_acceptorFd, (sockaddr*)&clientAddr, &clientAddrLen);
    assert(connFd >= 0);

    //绑定新连接
    if(m_newConnectionCallback)
        m_newConnectionCallback(connFd);

}
