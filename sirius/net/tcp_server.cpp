#include "tcp_server.h"

using namespace sirius;

TcpServer::TcpServer(const InetAddress &listenAddr)
:mainLoop_(new EventLoop),
 acceptor_(new Acceptor(mainLoop_->get_epollFd(), listenAddr)),
 threadpool_(new EventLoopThreadpool(mainLoop_.get()))
{
    HttpConn::userCount = 0;
    HttpConn::srcDir = resourcePath.c_str();//getcwd(nullptr, 256);
    
    //将new_connection注册到acceptor_上
    acceptor_->set_new_connection_callback(
        std::bind(&TcpServer::new_connection, this, _1)
    );
}

void TcpServer::set_thread_nums(int numThread)
{
    assert(numThread >= 0);
    threadpool_->set_thread_unm(numThread);
}

void TcpServer::start()
{
    //线程池创建sub_loop
    threadpool_->start(
        std::bind(&TcpServer::thread_callback, this, _1)
    );

    mainLoop_->run_in_loop(
        std::bind(&Acceptor::run_listen, acceptor_.get())
    );

    //main_loop开启循环
    mainLoop_->loop();

}


void TcpServer::new_connection(int sockFd)
{
    //采用轮询的方法获取EventLoop
    EventLoop* ioLoop = threadpool_->get_next_loop();
    try{
        ioLoop->add_tcpConnection(sockFd);
    }
    catch (const std::runtime_error& err) {
        log->log_error(err.what());
    }
}


void TcpServer::thread_callback(EventLoop *loop)
{
    
}
