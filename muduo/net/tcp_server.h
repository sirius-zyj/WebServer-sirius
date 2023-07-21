#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include "base/head.h"
#include "callback.h"
#include "event_loop.h"
#include "inet_address.h"
#include "acceptor.h"
#include "event_loop_threadpool.h"
#include "tcp_connection.h"

namespace muduo
{
namespace net
{

class TcpServer
{
public:
    TcpServer(const InetAddress& listenAddr);


    void set_thread_nums(int numThread);
    void start();

private:
    void new_connection(int sockfd);
    void thread_callback(EventLoop*);

private:
    unique_ptr<EventLoop> mainLoop_;

    unique_ptr<EventLoopThreadpool> threadpool_;
    unique_ptr<Acceptor> acceptor_;//智能指针
};

}
}

#endif