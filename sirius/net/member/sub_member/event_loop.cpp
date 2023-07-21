#include "event_loop.h"


using namespace sirius;

EventLoop::EventLoop():
quit_(false)
{
    //创建epoll实例
    epollFd_ = epoll_create1(EPOLL_CLOEXEC);
    assert(epollFd_ >= 0);
}

void EventLoop::loop()
{
    while(!quit_){
        int number = epoll_wait(epollFd_, m_events, MAX_EVENT_NUMBER, -1);//不超时
        for(int i=0; i<number; i++){
            Channel* channel = static_cast<Channel*>(m_events[i].data.ptr);
            channel->set_revent(m_events[i].events);
            channel->handle_event();     
        }
    }
}

void EventLoop::run_in_loop(Functor cb)
{
    cb();
}

void EventLoop::add_tcpConnection(int fd)
{
    shared_ptr<TcpConnection> conn(new TcpConnection(epollFd_, fd));
    {
        unique_lock<mutex> lock(mtx_);//conns_上锁
        if(conns_.find(conn->get_sockFd()) != conns_.end()){
            erase_tcpConnection(conns_.at(conn->get_sockFd()));//万一存在之前未清理的conn
            log->log_warn("%lx close one", pthread_self());
        }
        conn->set_close_callback(std::bind(&EventLoop::erase_tcpConnection, this, _1));
        conns_[conn->get_sockFd()] = conn;
        conn->establish_connect();
    }
}

void EventLoop::erase_tcpConnection(shared_ptr<TcpConnection>& conn)
{
    unique_lock<mutex> lock(mtx_);
    conn->close_connect();
    conns_.erase(conn->get_sockFd());
}
