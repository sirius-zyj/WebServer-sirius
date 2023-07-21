#ifndef CONFIG_H
#define CONFIG_H

#include <string>
using std::string;

#define ENABLE_LOG_INFO
#define ENABLE_LOG_WARN
#define ENABLE_LOG_ERROR


extern const string resourcePath;//只会被定义一次

//数据库相关
extern const char* sqlType;
extern const char * sqlHost;//会被重复定义
extern const int sqlPort;
extern const char *sqlUser;
extern const char *sqlPwd;
extern const char *sqlDb;
extern const int sqlMaxConn;

#endif

