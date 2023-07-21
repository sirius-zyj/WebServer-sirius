#ifndef LOGGER_H_
#define LOGGER_H_

#include <log4cpp/Category.hh>
#include <log4cpp/FileAppender.hh>
#include <log4cpp/OstreamAppender.hh>
#include <log4cpp/PatternLayout.hh>
#include <log4cpp/Priority.hh>
#include <config.h>
using namespace log4cpp;

class Logger
{
private:
    Logger();
    //禁止拷贝构造
    Logger(const Logger&) = delete;
    ~Logger(){};
    void log_init();
    // std::string format_msg(const char* msg, ...);

public:
    static Logger* get_single_instance();
    void log_info(const char* msg, ...);
    void log_warn(const char* msg, ...);
    void log_error(const char* msg, ...);
    
private:
    Category& m_root;
};

#endif