#ifndef CALLBACK_H
#define CALLBACK_H

#include <functional>
#include <memory>

#include "plugin/head.h"

using std::function;

namespace sirius
{
//EventLoop类
typedef function<void()> Functor;

//TcpServer类
class EventLoop;
class TcpConnection;
typedef function<void(const TcpConnection&)> ConnectionCallback;
typedef function<void(const TcpConnection&)> CloseCallback;
typedef function<void(EventLoop*)> ThreadInitCallback;


//Acceptor类
class InetAddress;
typedef function<void(int sockFd)> NewConnectionCallback;

//Channel类
typedef function<void()> ReadEventCallback;
typedef function<void()> WriteEventCallback;
typedef function<void()> CloseEventCallback;

//TcpConnection类
typedef function<void(shared_ptr<TcpConnection>&)> TcpCloseCallBack;

}

#endif