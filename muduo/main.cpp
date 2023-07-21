#include "net/event_loop.h"
#include "net/inet_address.h"
#include "net/echo_server.h"

using namespace muduo::net;

int main(int argc, char* argv[])
{
    log->log_info("\n --------------------------------------- \n");
    // //监听端口
    InetAddress listenAddr(9006);

    //实例化服务器
    TcpServer tcpServer(listenAddr);
    int freeKernal = int(sysconf(_SC_NPROCESSORS_ONLN)) - 1;
    log->log_info("%d kernels used",freeKernal);
    tcpServer.set_thread_nums(freeKernal);//设置线程数
    tcpServer.start();

    return 0;
}