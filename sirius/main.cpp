#include "net/member/sub_member/event_loop.h"
#include "net/member/sub_member/inet_address.h"
#include "net/tcp_server.h"
#include "net/redis_connection_pool.h"

using namespace sirius;

int main(int argc, char* argv[])
{
    Log->log_info("\n --------------------------------------- \n");
    //监听端口
    InetAddress listenAddr(9006);

    //实例化服务器
    TcpServer tcpServer(listenAddr);
    int freeKernal = int(sysconf(_SC_NPROCESSORS_ONLN)) - 2;
    Log->log_info("%d kernels used",freeKernal);
    tcpServer.set_thread_nums(freeKernal);//设置线程数
    tcpServer.start();

    return 0;
}