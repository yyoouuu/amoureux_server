/*
Log4j

Logger(定义日志类别)
    |
    |————Formatter(日志格式)
    |
Appender(日志输出地方)
*/

#pragma once
#include <string>
#include <stdint.h>
#include <memory>
#include <vector>
#include <fstream>
#include <iostream>
namespace amr
{

    // 日志事件
    class LogEvent
    {

    public:
        typedef std::shared_ptr<LogEvent> s_ptr;
        LogEvent();

    private:
        const char *m_file = nullptr; // 文件名
        int32_t m_line = 0;           // 行号
        uint32_t m_elapse = 0;        // 程序启动开始到现在的毫秒数
        uint32_t m_threadId = 0;      // 线程id
        uint32_t m_fiberId = 0;       // 协程id
        uint64_t m_time = 0;          // 时间戳
        std::string m_content;        // 日志内容
    };

    // 日志級別:C++11之后支持enum类
    class LogLevel
    {
    public:
        enum Level
        {
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5
        };
    };

    // 日志格式器
    class LogFormatter
    {
    public:
        typedef std::shared_ptr<LogFormatter> s_ptr;
        std::string format(LogEvent::s_ptr event);

    private:
    };

    // 日志器
    class Logger
    {
    public:
        typedef std::shared_ptr<Logger> s_ptr;
        Logger(const std::string &name = "root");
        void log(LogLevel::Level level, LogEvent::s_ptr event);
        // 输出日志
        void debug(LogEvent::s_ptr event);
        void info(LogEvent::s_ptr event);
        void warn(LogEvent::s_ptr event);
        void error(LogEvent::s_ptr event);
        void fatal(LogEvent::s_ptr event);
        // 增删日志输出地
        void addAppender(LogAppender::s_ptr appender);
        void deleteAppender(LogAppender::s_ptr appender);

        LogLevel::Level getLoglevel() const { return m_level; }
        void setLoglevel(LogLevel::Level level) { m_level = level; }

    private:
        std::string m_name;                          // 日志名称
        LogLevel::Level m_level;                     // 日志级别
        std::vector<LogAppender::s_ptr> m_appenders; // 日志输出地集合
    };

    // 日志输出地
    class LogAppender
    {
    public:
        typedef std::shared_ptr<LogAppender> s_ptr;
        virtual ~LogAppender() {}

        virtual void log(LogLevel::Level level, LogEvent::s_ptr event) = 0;
        void setLogFormatter(LogFormatter::s_ptr formatter) { m_formatter = formatter; }
        LogFormatter::s_ptr getLogFormatter() const { return m_formatter; }

    protected:
        LogLevel::Level m_level;
        LogFormatter::s_ptr m_formatter;
    };

    // 输出到控制台的Appender
    class StdoutLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<StdoutLogAppender> s_ptr;
        virtual void log(LogLevel::Level level, LogEvent::s_ptr evnent) override;
    };

    // 输出到文件的Appender:需要文件地址
    class FileoutLogAppender : public LogAppender
    {
    public:
        typedef std::shared_ptr<FileoutLogAppender> s_ptr;
        FileoutLogAppender(const std::string &filename);
        virtual void log(LogLevel::Level level, LogEvent::s_ptr evnent) override;
        bool reopen();
    private:
        std::string m_filename;
        std::ofstream m_filestream;
    };
}
