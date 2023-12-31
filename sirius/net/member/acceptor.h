#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>

#include "net/callback.h"
#include "sub_member/channel.h"
#include "sub_member/event_loop.h"
#include "sub_member/inet_address.h"

namespace sirius
{

class EventLoop;

class Acceptor
{
public:
    Acceptor(int epollFd, const InetAddress& listenAddr);

    void set_new_connection_callback(const NewConnectionCallback& cb){
        newConnectionCallback_ = cb;
    };
    void run_listen();

private:
    void handle_read();


private:
    int acceptorFd_;
    Channel acceptorChannel_;

    NewConnectionCallback newConnectionCallback_;

};

}

#endif