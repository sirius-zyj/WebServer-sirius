#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include "plugin/buffer/buffer.h"

#include "channel.h"
#include "event_loop.h"
#include "inet_address.h"
#include "http/http_conn.h"



namespace sirius
{
class EventLoop;

class TcpConnection: public std::enable_shared_from_this<TcpConnection>
{
public:
    TcpConnection(int epollFd, int sockFd);

    int get_sockFd() { return sockFd_; } 
    void establish_connect();
    void close_connect();
    void set_close_callback(TcpCloseCallBack cb){ closeCB_ = cb; }
    void handle_read();
    void handle_write();
    void handle_close();

private:
    void handle_process();

private:
    int sockFd_; 

    TcpCloseCallBack closeCB_;
    Channel m_channel;

    HttpConn httpConn_;

};

}


#endif