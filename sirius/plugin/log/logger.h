#ifndef LOGGER_H_
#define LOGGER_H_

#include <include/Category.hh>
#include <include/FileAppender.hh>
#include <include/OstreamAppender.hh>
#include <include/PatternLayout.hh>
#include <include/Priority.hh>

#include <plugin/config.h>

using namespace log4cpp;

#define log Logger::get_single_instance() 
namespace sirius
{
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
}
#endif