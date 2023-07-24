#include "acceptor.h"

using namespace sirius;


Acceptor::Acceptor(int epollFd, const InetAddress &listenAddr)
: acceptorFd_(socket(listenAddr.family(), SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP)),//设置非阻塞
  acceptorChannel_(epollFd, acceptorFd_)
{
    //可复用地址
    int flag = 1;
    setsockopt(acceptorFd_, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    //绑定端口
    sockaddr_in addr = listenAddr.address();
    int ret = bind(acceptorFd_, (struct sockaddr *)&addr, sizeof(addr));
    assert(ret >= 0);

    //将handleRead函数注册进acceptorChannel_中
    acceptorChannel_.set_read_callback(
        std::bind(&Acceptor::handle_read, this)
    );
}

void Acceptor::run_listen()
{
    //开启监听事件
    int ret = listen(acceptorFd_, 256);
    assert(ret >= 0);
    acceptorChannel_.enable_reading();
}

void Acceptor::handle_read()
{
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    //LT触发模式    
    int connFd = accept(acceptorFd_, (sockaddr*)&clientAddr, &clientAddrLen);
    assert(connFd >= 0);

    //绑定新连接
    if(newConnectionCallback_)
        newConnectionCallback_(connFd);

}
