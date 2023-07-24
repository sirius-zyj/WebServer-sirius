#include "logger.h"

using namespace sirius;

Logger::Logger() : m_root(Category::getRoot().getInstance("root"))
{
    log_init();
}

Logger *Logger::get_single_instance()
{
    //此处的static算是静态局部变量
    static Logger logger;
    return &logger;
}



void Logger::log_init()
{

    // OstreamAppender* osAppender= new OstreamAppender("osAppender",&std::cout);
    // osAppender->setLayout( new BasicLayout());

    // m_root.addAppender(osAppender);
    // m_root.setPriority(log4cpp::Priority::DEBUG);

    //创建一个Appender对象
    FileAppender *fileAppender = new FileAppender("fileAppender", "./logger.log");
    //创建一个Layout对象
    PatternLayout *patternLayout = new PatternLayout();
    patternLayout->setConversionPattern("%d:(%c) %p : %m%n");
    //将Layout对象绑定到Appender对象
    fileAppender->setLayout(patternLayout);
    //将Appender对象绑定到Category对象
    m_root.setAppender(fileAppender);
    //设置Category对象的优先级，低于此优先级的日志不被记录
    m_root.setPriority(Priority::DEBUG);
    
}



void Logger::log_warn(const char *msg, ...)
{
#ifdef ENABLE_LOG_WARN
    va_list args;
    va_start(args, msg);
    char buf[256];
    int len = vsnprintf(buf, sizeof(buf), msg, args);
    va_end(args);
    if (len >= sizeof(buf)) {
        // 如果输出的字符数超过缓冲区大小，则进行相应处理，比如扩大缓冲区或使用动态内存
        int newBufferSize = len + 1;  // 加上结尾的 null 字符
        char* newBuf = new char[newBufferSize];
        va_list args2;
        va_start(args2, msg);
        vsnprintf(newBuf, newBufferSize, msg, args2);
        va_end(args2);
        m_root.warn(std::string(newBuf));
        delete[] newBuf;
    } else {
        m_root.warn(std::string(buf));
    }
#endif
}

void Logger::log_error(const char *msg, ...)
{
#ifdef ENABLE_LOG_ERROR
    va_list args;
    va_start(args, msg);
    char buf[256];
    int len = vsnprintf(buf, sizeof(buf), msg, args);
    va_end(args);
    if (len >= sizeof(buf)) {
        // 如果输出的字符数超过缓冲区大小，则进行相应处理，比如扩大缓冲区或使用动态内存
        int newBufferSize = len + 1;  // 加上结尾的 null 字符
        char* newBuf = new char[newBufferSize];
        va_list args2;
        va_start(args2, msg);
        vsnprintf(newBuf, newBufferSize, msg, args2);
        va_end(args2);
        m_root.error(std::string(newBuf));
        delete[] newBuf;
    } else {
        m_root.error(std::string(buf));
    }
#endif
}

void Logger::log_info(const char *msg, ...)
{
#ifdef ENABLE_LOG_INFO
    va_list args;
    va_start(args, msg);
    char buf[256];
    int len = vsnprintf(buf, sizeof(buf), msg, args);
    va_end(args);
    if (len >= sizeof(buf)) {
        // 如果输出的字符数超过缓冲区大小，则进行相应处理，比如扩大缓冲区或使用动态内存
        int newBufferSize = len + 1;  // 加上结尾的 null 字符
        char* newBuf = new char[newBufferSize];
        va_list args2;
        va_start(args2, msg);
        vsnprintf(newBuf, newBufferSize, msg, args2);
        va_end(args2);
        m_root.info(std::string(newBuf));
        delete[] newBuf;
    } else {
        m_root.info(std::string(buf));
    }
#endif
}
