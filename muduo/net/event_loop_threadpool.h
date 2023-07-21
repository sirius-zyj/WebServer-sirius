#ifndef EVENT_LOOP_THREADPOOL
#define EVENT_LOOP_THREADPOOL

#include "callback.h"
#include "event_loop_thread.h"
#include "base/head.h"

namespace muduo
{
namespace net
{

class EventLoop;

class EventLoopThreadpool
{
public:
    EventLoopThreadpool(EventLoop* eventLoop);

    void set_thread_unm(int numThread){ numThread_ = numThread;};
    void start(const ThreadInitCallback& cb);

    EventLoop* get_next_loop();

private:
    int numThread_;

    EventLoop* m_mainLoop;
    vector<unique_ptr<EventLoopThread>> threads_;
    vector<EventLoop*> subLoops_;
    int next_;

};

}
}


#endif
