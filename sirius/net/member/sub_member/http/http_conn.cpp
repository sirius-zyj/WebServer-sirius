#include "http_conn.h"

using namespace sirius;

const char* HttpConn::srcDir;
std::atomic<int> HttpConn::userCount;

void HttpConn::init()
{
    ++userCount;
    readBuffer_.retrieve_all();
    writeBuffer_.retrieve_all();
    isClose_ = false;
}

ssize_t HttpConn::read()
{
    ssize_t len = readBuffer_.read_fd(fd_);
    if(len < 0){
        log->log_error("read error %s", strerror(errno));
        return -1;
    }
    return len;
}

ssize_t HttpConn::write()
{
    ssize_t len = -1;
    do{
        len = writev(fd_, iov_, iovCnt_);
        if(len < 0){
            if(errno == EAGAIN){//当前暂时无法写入
                return -2;
            }
            log->log_error("write error %s", strerror(errno));
            return -1;
        }
        if(iov_[0].iov_len + iov_[1].iov_len == 0){
            break;
        }
        else if(static_cast<size_t>(len) > iov_[0].iov_len){
            iov_[1].iov_base = static_cast<char*>(iov_[1].iov_base) + (len - iov_[0].iov_len);
            iov_[1].iov_len -= (len - iov_[0].iov_len);
            if(iov_[0].iov_len){
                writeBuffer_.retrieve_all();
                iov_[0].iov_len = 0;
            }
        }
        else{
            iov_[0].iov_base = static_cast<char*>(iov_[0].iov_base) + len;
            iov_[0].iov_len -= len;
            writeBuffer_.retrieve(len);
        }
    }
    while( writeable_chars() > 10240);
    return len;
}

bool HttpConn::process()
{
    //请求
    request_.Init();
    if(readBuffer_.readable_chars() <= 0){
        return false;
    }
    else if(request_.parse(readBuffer_)){
        response_.Init(srcDir, request_.path(), request_.IsKeepAlive(), 200);
    }
    else{
        response_.Init(srcDir, request_.path(), false, 400);
    }
    //响应
    response_.MakeResponse(writeBuffer_);
    iov_[0].iov_base = const_cast<char*>(writeBuffer_.peek());
    iov_[0].iov_len = writeBuffer_.readable_chars();
    iovCnt_ = 1;

    if(response_.FileLen() > 0 && response_.File()){
        iov_[1].iov_base = response_.File();
        iov_[1].iov_len = response_.FileLen();
        iovCnt_ = 2;
    }
    log->log_info("filesize:%d, %d  to %d", response_.FileLen(), iovCnt_, iov_[0].iov_len);
    return true;

}
