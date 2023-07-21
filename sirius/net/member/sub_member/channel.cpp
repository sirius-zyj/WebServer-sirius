#include "channel.h"

// using namespace muduo;
using namespace sirius;


Channel::Channel(int epollFd, int fd)
:epollFd_(epollFd),
 fd_(fd),
 m_updateIndex(kNew),
 m_event(noneEvent)
{
    
}

void Channel::handle_event()
{
    //POLLHUP表示连接被挂起(或者在读写过程中被关闭)
    if(m_revent & (EPOLLHUP | EPOLLRDHUP)){//当远程套接字关闭时，Linux 内核会触发两种事件：EPOLLIN 和 EPOLLRDHUP,所以需要先处理EPOLLRDHUP
        closeCallback_();
    }
    //出错
    else if(m_revent & (EPOLLERR)){
    }
    //检测到输入
    else if(m_revent & (EPOLLIN | EPOLLPRI)){
        readCallback_();
    }
    //检测到输出
    else if(m_revent & EPOLLOUT){
        writeCallback_();
    }
}

void Channel::update()
{
    //添加到epoll上
    if(m_updateIndex == kNew || m_updateIndex == kDeleted){
        struct epoll_event event;
        event.events = m_event;
        event.data.ptr = static_cast<void*>(this);
        if(epoll_ctl(epollFd_, EPOLL_CTL_ADD, fd_, &event) == -1){
            log->log_error("connect err");
            return;
        };
        m_updateIndex = kAdded;
        log->log_info("append fd : %d, ptr : %lx", fd_, event.data.ptr);
    }
    //更改或删除
    else{
        if(m_event == noneEvent){
            struct epoll_event event;
            event.events = m_event;
            event.data.ptr = static_cast<void*>(this);
            if(epoll_ctl(epollFd_, EPOLL_CTL_DEL, fd_, &event) == -1){//将fd从epoll中删除
                if(errno == ENOENT) log->log_warn("no register fd %d", fd_);
                close(fd_);//关闭文件描述符
                log->log_error("delete err beacuse %s", strerror(errno));
                return;
            };
            close(fd_);//关闭文件描述符
            log->log_info("delete fd : %d, ptr : %lx", fd_, event.data.ptr);
            m_updateIndex = kDeleted;
        }
        else{
            struct epoll_event event;
            event.events = m_event;
            event.data.ptr = static_cast<void*>(this);
            if(epoll_ctl(epollFd_, EPOLL_CTL_MOD, fd_, &event) == -1){//更改fd在epoll中的事件
                log->log_error("modify err");
                return;
            };
            log->log_info("modify fd : %d, ptr : %lx", fd_, event.data.ptr);
        }
    }
}
