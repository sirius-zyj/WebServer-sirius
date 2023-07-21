#include "buffer.h"

using namespace sirius;

void Buffer::append(const char *str, size_t len)
{
    make_space(len);
    std::copy(str, str + len, begin() + writeIndex_);
    writeIndex_ += len;
}

void Buffer::make_space(size_t len)
{
    if(writable_chars() + prepandable_chars() < len){
        buffer_.resize(writeIndex_ + len);
    }
    else{
        size_t readable = readable_chars();
        std::copy(begin() + readIndex_, begin() + writeIndex_, begin());
        readIndex_ = 0;
        writeIndex_ = readIndex_ + readable;
    }
}

ssize_t Buffer::read_fd(int fd)
{
    char extraBuf[65535];
    struct iovec vec[2];
    const size_t writable = writable_chars();//可写的字节数
    vec[0].iov_base = begin() + writeIndex_;
    vec[0].iov_len = writable;
    vec[1].iov_base = extraBuf;
    vec[1].iov_len = sizeof(extraBuf);

    const ssize_t len = readv(fd, vec, 2);
    if(len < 0){
        // log->log_error("readv error %s", strerror(errno));
        return -1;
    } 
    else if(static_cast<size_t>(len) < writable){
        writeIndex_ += len;
    } 
    else {
        writeIndex_ = buffer_.size();
        append(extraBuf, len - writable);
    }
    
    return len;
}

ssize_t Buffer::write_fd(int fd)
{
    ssize_t len = write(fd, begin() + readIndex_, readable_chars());
    if(len < 0){
        log->log_error("write error %s", strerror(errno));
        return -1;
    }
    readIndex_ += len;
    return len;
}
