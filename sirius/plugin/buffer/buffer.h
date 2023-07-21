#ifndef BUFFER_H
#define BUFFER_H

#include <sys/uio.h>

#include "plugin/head.h"

namespace sirius
{

class Buffer
{
 public:
  explicit Buffer(size_t initialSize = 1024):
  buffer_(initialSize), readIndex_(0), writeIndex_(0)
  {};
  ~Buffer() = default;

  void append(const string& str) { append(str.data(), str.size()); }
  void retrieve(size_t len){ readIndex_ += len; }
  void retrieve_until(const char* end){ retrieve(end - peek()); }
  // void retrieve_all(){ bzero(&buffer_, buffer_.size()); readIndex_ = writeIndex_ = 0; }//换成这句有问题
  void retrieve_all(){ std::fill_n(buffer_.begin(), buffer_.size(), 0); readIndex_ = writeIndex_ = 0; }
  ssize_t read_fd(int fd);
  ssize_t write_fd(int fd);

  size_t readable_chars() const { return writeIndex_ - readIndex_; }
  size_t writable_chars() const { return buffer_.size() - writeIndex_; }
  size_t prepandable_chars() const {return readIndex_; }
  const char* peek() { return begin() + readIndex_; }
  const char* begin_write() { return begin() + writeIndex_; }

 private:
  char* begin() { return &(*buffer_.begin()); }

  void append(const char* str, size_t len);
  void make_space(size_t len);
    

 private:
  vector<char> buffer_;
  size_t readIndex_;
  size_t writeIndex_;

};
}

#endif