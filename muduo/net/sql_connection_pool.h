#ifndef SQL_CONNECTION_POOL_H
#define SQL_CONNECTION_POOL_H

// #include <mysql/mysql.h>
#include "base/head.h"
#include "base/soci/include/soci/soci.h"

using namespace soci;

#define oneSqlConn SqlConnectionPool::get_single_instance() 

class SqlConnectionPool
{
    friend class SqlConnRAII;
 private:
    SqlConnectionPool() { init(); };
    SqlConnectionPool(const SqlConnectionPool&) = delete;
    SqlConnectionPool& operator=(const SqlConnectionPool&) = delete;
    ~SqlConnectionPool(){ close_pool(); };
 public:
    static SqlConnectionPool* get_single_instance();

    
 private:
    void init();
    session* occupy_conn();
    void free_conn(session *conn);
    void close_pool();

 private:
    int maxConn_;

    list<session*> connList_;
    mutex mtx_;
    condition_variable cv_;
};


class SqlConnRAII
{
public:
    SqlConnRAII()
    {
        conn_ = oneSqlConn->occupy_conn();
    }

    auto operator<<(const string& sql)
    {
        return (*conn_) << sql;
    }
    session* operator->()
    {
        return conn_;
    }

    ~SqlConnRAII()
    {
        if(conn_){
            oneSqlConn->free_conn(conn_);
        }
    }
private:
    SqlConnRAII(const SqlConnRAII&) = delete;
    SqlConnRAII& operator=(const SqlConnRAII&) = delete;
    session *conn_;
};

#endif