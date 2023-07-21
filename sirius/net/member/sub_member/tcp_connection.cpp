#include "tcp_connection.h"

using namespace sirius;

TcpConnection::TcpConnection(int epollFd, int sockFd)
:sockFd_(sockFd),
 httpConn_(sockFd),
 m_channel(epollFd, sockFd)
{
    //操作系统来监测是否断开
    int flag = 1;
    setsockopt(sockFd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag));
    //设置非阻塞IO
    {
        int oldOption = fcntl(sockFd_, F_GETFL );
        int newOption = oldOption | O_NONBLOCK;
        assert(fcntl( sockFd_, F_SETFL, newOption) == 0);
    }
    //绑定 读写 异常 关闭 回调事件
    m_channel.set_read_callback(
        std::bind(&TcpConnection::handle_read, this)
    );
    m_channel.set_write_callback(
        std::bind(&TcpConnection::handle_write, this)
    );
    m_channel.set_close_callback(
        std::bind(&TcpConnection::handle_close, this)
    );
    
}

void TcpConnection::establish_connect()
{
    //将Channel绑定到相应的EventLoop上(或者说是 将Channel对应的sockFd绑定到EventLoop对应的epollFd)
    m_channel.enable_reading();
}

void TcpConnection::close_connect()
{
    m_channel.disableAll();
}

void TcpConnection::handle_read()
{   
    int len = httpConn_.read();
    
    if(len < 0){
        log->log_error("%lx recv error %s", pthread_self(), strerror(errno));
        handle_close();
    }
    else if (len == 0){//没有可读的数据了
        log->log_info("%lx recv none", pthread_self());
        handle_close();
    } else {
        // 处理数据
        handle_process();
    }

}

void TcpConnection::handle_write()
{
    int ret = httpConn_.write();
    if(httpConn_.writeable_chars() == 0){
        if(httpConn_.is_alived()){
            log->log_info("%lx is alived , enable reading", pthread_self());
            m_channel.enable_reading();
            return;
        }
    }
    else if(ret == -2){//暂时无法写,等一段时间再写
        m_channel.enable_writing();
        return;
    }
    //1. 发送完毕,且连接非活跃 2. 发送失败
    handle_close();
}

void TcpConnection::handle_close()
{
    shared_ptr<TcpConnection> conn = shared_from_this();
    closeCB_(conn);
    log->log_info("%lx close one", pthread_self());
}

void TcpConnection::handle_process()
{
    if(httpConn_.process()){
        m_channel.enable_writing();
    }
    else{
        m_channel.enable_reading();
    }
}
