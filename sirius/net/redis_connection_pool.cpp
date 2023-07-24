#include "redis_connection_pool.h"

using namespace sirius;

RedisConnectionPool *RedisConnectionPool::get_single_instance()
{
    static RedisConnectionPool pool;
    return &pool;
}

void RedisConnectionPool::init()
{
    maxConn_ = redisMaxConn;
    char priority[100];
    snprintf(priority, sizeof(priority), "tcp://%s:%d", redisHost, redisPort);
    for(int i=0; i<maxConn_; ++i){
        Redis *conn = new Redis(priority);
        connList_.push_back(conn);
    }
}

Redis* RedisConnectionPool::occupy_conn()
{
    Redis *redis = nullptr;
    {
        unique_lock<mutex> lock(mtx_);
        cv_.wait(lock, [this](){return !connList_.empty();});
        redis = connList_.front();
        connList_.pop_front();
    }

    return redis;
}

void RedisConnectionPool::free_conn(Redis *conn)
{
    {
        unique_lock<mutex> lock(mtx_);
        connList_.push_back(conn);
    }
    cv_.notify_one();
}

void RedisConnectionPool::close_pool()
{
    unique_lock<mutex> lock(mtx_);
    int maxC = maxConn_;
    cv_.wait(lock, [this, maxC](){return connList_.size() == maxC;});//等待子线程返还所有的conn,确保连接全被删除
    for(auto &conn : connList_){
        delete conn;
        conn = nullptr;
    }
    log->log_info("redis connection pool closed");
}
