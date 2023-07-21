#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

#include "callback.h"
#include "channel.h"
#include "event_loop.h"
#include "inet_address.h"

namespace muduo
{
namespace net
{

class EventLoop;

class Acceptor
{
public:
    Acceptor(int epollFd, const InetAddress& listenAddr);

    void set_new_connection_callback(const NewConnectionCallback& cb){
        m_newConnectionCallback = cb;
    };
    void run_listen();

private:
    void handle_read();


private:
    int m_acceptorFd;
    Channel m_acceptorChannel;

    NewConnectionCallback m_newConnectionCallback;

};

}
}

#endif