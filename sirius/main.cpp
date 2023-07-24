#include "net/member/sub_member/event_loop.h"
#include "net/member/sub_member/inet_address.h"
#include "net/tcp_server.h"
#include "net/redis_connection_pool.h"

using namespace sirius;

int main(int argc, char* argv[])
{
    log->log_info("\n --------------------------------------- \n");
    //监听端口
    InetAddress listenAddr(9006);

    //实例化服务器
    TcpServer tcpServer(listenAddr);
    int freeKernal = int(sysconf(_SC_NPROCESSORS_ONLN)) - 2;
    log->log_info("%d kernels used",freeKernal);
    tcpServer.set_thread_nums(freeKernal);//设置线程数
    tcpServer.start();

    // RedisConnRAII redis;

    // string path = "/home/zhangyijun/Documents/network/My_muduo/resources/index.html";
    
    // auto val = redis->get(path);
    // if(val){
    //     log->log_info("Already in redis\n\n %s", val.value().c_str());
    // }
    // else{
    //     struct stat mmFileStat_;
    //     stat(path.data(), &mmFileStat_);
    //     int srcFd = open(path.data(), O_RDONLY);
    //     int* mmRet = (int*)mmap(0, mmFileStat_.st_size, PROT_READ, MAP_PRIVATE, srcFd, 0);
    //     char* mmFile_ = (char*)mmRet;
    //     redis->set(path, mmFile_);
    //     log->log_info("not in redis");
    //     close(srcFd);
    //     if(mmFile_) {
    //         munmap(mmFile_, mmFileStat_.st_size);
    //         mmFile_ = nullptr;
    //     }
    // }

    return 0;
}