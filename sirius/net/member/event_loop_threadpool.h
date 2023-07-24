#ifndef EVENT_LOOP_THREADPOOL
#define EVENT_LOOP_THREADPOOL

#include "plugin/head.h"

#include "net/callback.h"
#include "sub_member/event_loop_thread.h"

namespace sirius
{

class EventLoop;

class EventLoopThreadpool
{
public:
    EventLoopThreadpool(EventLoop* eventLoop);

    void set_thread_num(int numThread){ numThread_ = numThread;};
    void start(const ThreadInitCallback& cb);

    EventLoop* get_next_loop();

private:
    int numThread_;

    EventLoop* mainLoop_;
    vector<unique_ptr<EventLoopThread>> threads_;
    vector<EventLoop*> subLoops_;
    int next_;

};

}


#endif
