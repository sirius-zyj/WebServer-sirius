#include "event_loop_threadpool.h"

using namespace muduo::net;

EventLoopThreadpool::EventLoopThreadpool(EventLoop *eventLoop)
: m_mainLoop(eventLoop),
  numThread_(0),//默认全为main_eventLoop在处理
  next_(0)
{
    
}

void EventLoopThreadpool::start(const ThreadInitCallback& cb)
{
    for(int i=0; i<numThread_; i++){
        EventLoopThread* t = new EventLoopThread(cb);
        //创建新线程并投入线程池中:因为使用的unique_ptr智能指针,所以当线程池被销毁时,该容器内单线程也会被销毁
        threads_.push_back(unique_ptr<EventLoopThread>(t));
        //新线程中创建event_loop,并开始循环
        subLoops_.push_back(t->creat_loop());
    }

}

EventLoop *EventLoopThreadpool::get_next_loop()
{
    //单核cpu均用mainLoop
    EventLoop* loop = m_mainLoop;

    if(!subLoops_.empty()){
        loop = subLoops_[next_];
        ++next_;
        if(static_cast<size_t>(next_) >= subLoops_.size()){//类型安全转换
            next_ = 0;
        }
    }
    
    return loop;
}
