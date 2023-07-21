#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include <sys/epoll.h>

#include "base/head.h"
#include "callback.h"
#include "channel.h"
#include "tcp_connection.h"


namespace muduo
{
namespace net
{
const int MAX_EVENT_NUMBER = 65535; //最大事件数

class EventLoop
{
public:
    EventLoop();

    void loop();
    void run_in_loop(Functor);
    int get_epollFd(){return epollFd_;};
    pthread_t get_thread(){
        if(!m_pthread)
            m_pthread = pthread_self();
        return m_pthread;
    };
    void add_tcpConnection(int fd);
    // void insert_tcpConnection(shared_ptr<TcpConnection>& conn);
    void erase_tcpConnection(shared_ptr<TcpConnection>& conn);

    void quit() { quit_ = true; };

private:
    int epollFd_;
    epoll_event m_events[MAX_EVENT_NUMBER];

    pthread_t m_pthread;

    bool quit_;

    mutex mtx_;
    map<int, shared_ptr<TcpConnection>> conns_;

};


}    
}


#endif