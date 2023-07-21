#ifndef HTTP_CONN_H
#define HTTP_CONN_H

#include <netinet/in.h>
#include <atomic>

#include "plugin/head.h"
#include "plugin/buffer/buffer.h"
#include "http_request.h"
#include "http_response.h"

namespace sirius
{
class HttpConn
{
 public:
    HttpConn(int fd):fd_(fd) { init(); };

    void init();
    ssize_t read();
    ssize_t write();

    bool process();

    size_t writeable_chars() const { return iov_[0].iov_len + iov_[1].iov_len; }
    bool is_alived() const { return request_.IsKeepAlive(); }

 public:
    static const char* srcDir;
    static std::atomic<int> userCount;
 
 private:
    int fd_;
    struct sockaddr_in addr_;

    Buffer readBuffer_;
    Buffer writeBuffer_;

    bool isClose_;

    struct iovec iov_[2];
    int iovCnt_;

    HttpRequest request_;
    HttpResponse response_;
};

}
#endif
