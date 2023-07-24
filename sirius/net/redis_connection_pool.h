#ifndef REDIS_CONNECTION_POOL_H
#define REDIS_CONNECTION_POOL_H

#include "plugin/head.h"
#include "plugin/redis/include/sw/redis++/redis++.h"

using namespace sw::redis;

#define oneRedisConn RedisConnectionPool::get_single_instance() 

namespace sirius
{

class RedisConnectionPool
{
    friend class RedisConnRAII;
 private:
    RedisConnectionPool() { init(); };
    RedisConnectionPool(const RedisConnectionPool&) = delete;
    RedisConnectionPool& operator=(const RedisConnectionPool&) = delete;
 public:
    ~RedisConnectionPool(){ close_pool(); };
 public:
    static RedisConnectionPool* get_single_instance();

 private:
    void init();
    Redis* occupy_conn();
    void free_conn(Redis *conn);
    void close_pool();

 private:
    int maxConn_;

    list<Redis*> connList_;
    mutex mtx_;
    condition_variable cv_;
};


class RedisConnRAII
{
 public:
    RedisConnRAII()
    {
        conn_ = oneRedisConn->occupy_conn();
    }

    Redis* operator->()
    {
        return conn_;
    }

    ~RedisConnRAII()
    {
        if(conn_){
            oneRedisConn->free_conn(conn_);
        }
    }
 private:
    RedisConnRAII(const RedisConnRAII&) = delete;
    RedisConnRAII& operator=(const RedisConnRAII&) = delete;
    Redis *conn_;
};

}
#endif