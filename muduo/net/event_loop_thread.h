#ifndef EVENT_LOOP_THREAD_H
#define EVENT_LOOP_THREAD_H

#include "callback.h"
#include "event_loop.h"

namespace muduo
{
namespace net
{

class EventLoopThread
{
 public:
    EventLoopThread(const ThreadInitCallback& cb);
    ~EventLoopThread();
    EventLoop* creat_loop();

 private:
    static void* worker(void* arg);

 private:
    EventLoop* loop_;
    pthread_t threadId_;

    mutex mtx_;
    condition_variable cv_;

    ThreadInitCallback callback_;

};

}
}

#endif