#include "event_loop_thread.h"

using namespace sirius;

EventLoopThread::EventLoopThread(const ThreadInitCallback &cb)
:loop_(nullptr),
 callback_(cb),
 threadId_(0),
 mtx_(),
 cv_()
{
}

EventLoopThread::~EventLoopThread()
{
    if(loop_ != nullptr && threadId_ != 0){
        loop_->quit();
        pthread_join(threadId_, NULL);
    }
}

EventLoop* EventLoopThread::creat_loop()
{
    //创建一个线程并投入使用
    if(pthread_create(&threadId_, NULL, worker, this)){
        throw std::exception();
    }

    EventLoop* loop = nullptr;
    {
        unique_lock<mutex> lock(mtx_);
        cv_.wait(lock, [this](){ return this->loop_ != nullptr; });//会重新获取锁\并判断是否符合条件
        loop = loop_;
    }
    return loop;
}

void *EventLoopThread::worker(void *arg)
{
    EventLoopThread* thread = static_cast<EventLoopThread*>(arg);

    EventLoop loop;//新线程创建的EventLoop
    loop.get_thread();

    {
        unique_lock<mutex> lock(thread->mtx_);
        thread->loop_ = &loop;
    }
    thread->cv_.notify_one();//销毁锁之后唤醒阻塞线程

    //开启事件循环
    loop.loop();

    unique_lock<mutex> lock(thread->mtx_);
    thread->loop_ = nullptr;
}
