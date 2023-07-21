#ifndef CHANNEL_H
#define CHANNEL_H

#include "sys/epoll.h"
#include <functional>
#include "poll.h"
#include <fcntl.h>

#include "net/callback.h"

namespace sirius
{

enum State{
    kNew = -1,
    kAdded = 1,
    kDeleted = 2
};
enum Mode :uint32_t {
    noneEvent = 0,
    readEvent = EPOLLIN | EPOLLPRI | EPOLLRDHUP | EPOLLERR,
    writeEvent = EPOLLOUT | EPOLLRDHUP | EPOLLERR
};

class Channel
{

public:
    Channel(int epollFd, int fd);

    void set_read_callback(ReadEventCallback cb){
        readCallback_ = cb;
    }
    void set_write_callback(WriteEventCallback cb){
        writeCallback_ = cb;
    }
    void set_close_callback(CloseEventCallback cb){
        closeCallback_ = cb;
    }

    void enable_reading(){
        m_event = readEvent; 
        update();
    }
    void enable_writing(){
        m_event = writeEvent; 
        update();
    }
    void disableAll() { 
        m_event = noneEvent; 
        update(); 
    }


    void set_revent(int revent){
        m_revent = revent;
    };
    void handle_event();

private:
    void update();


private:

    EventLoop* m_eventLoop;
    int epollFd_;
    int fd_;
    
    uint32_t m_event;
    uint32_t m_revent=0;

    State m_updateIndex;

    ReadEventCallback readCallback_;
    WriteEventCallback writeCallback_;
    CloseEventCallback closeCallback_;


};

}



#endif