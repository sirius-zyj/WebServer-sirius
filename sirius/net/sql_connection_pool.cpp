#include "sql_connection_pool.h"

using namespace sirius;

SqlConnectionPool *SqlConnectionPool::get_single_instance()
{
    static SqlConnectionPool pool;
    return &pool;
}

void SqlConnectionPool::init()
{
    maxConn_ = sqlMaxConn;
    char priority[100];
    snprintf(priority, sizeof(priority), "host=%s port=%d user=%s password=%s dbname=%s",
                               sqlHost, sqlPort, sqlUser, sqlPwd, sqlDb);
    for(int i=0; i<maxConn_; ++i){
        session *conn = new session(sqlType, priority);
        connList_.push_back(conn);
    }
}

session* SqlConnectionPool::occupy_conn()
{
    session *sql = nullptr;
    {
        unique_lock<mutex> lock(mtx_);
        cv_.wait(lock, [this](){return !connList_.empty();});
        sql = connList_.front();
        connList_.pop_front();
    }

    return sql;
}

void SqlConnectionPool::free_conn(session *conn)
{
    {
        unique_lock<mutex> lock(mtx_);
        connList_.push_back(conn);
    }
    cv_.notify_one();
}

void SqlConnectionPool::close_pool()
{
    unique_lock<mutex> lock(mtx_);
    int maxC = maxConn_;
    cv_.wait(lock, [this, maxC](){return connList_.size() == maxC;});//等待子线程返还所有的conn,确保连接全被删除
    for(auto &conn : connList_){
        conn->close();
        delete conn;
        conn = nullptr;
    }
    Log->log_info("sql connection pool closed");
}
