#include "config.h"


const string resourcePath = "/home/zhangyijun/Documents/network/My_muduo/resources";

//mysql连接池
const char* sqlType = "mysql";
const char * sqlHost = "localhost";//会被重复定义
const int sqlPort = 3306;
const char *sqlUser = "root";
const char *sqlPwd = "zyj1314520";
const char *sqlDb = "muduo";
const int sqlMaxConn = 8;

//redis连接池
const char *redisHost = "127.0.0.1";
const int redisPort = 6379;
const int redisMaxConn = 8;
