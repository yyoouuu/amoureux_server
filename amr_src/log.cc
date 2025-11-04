#include "log.h"

namespace amr
{
    Logger::Logger(const std::string &name) : m_name(name)
    {
    }

    void Logger::log(LogLevel::Level level, LogEvent::s_ptr event)
    {
        if (level >= m_level)
        {
            for (auto &i : m_appenders)
            {
                i->log(level, event);
            }
        }
    }

    void Logger::debug(LogEvent::s_ptr event)
    {
        log(LogLevel::DEBUG, event);
    }
    void Logger::info(LogEvent::s_ptr event)
    {
        log(LogLevel::INFO, event);
    }
    void Logger::warn(LogEvent::s_ptr event)
    {
        log(LogLevel::WARN, event);
    }
    void Logger::error(LogEvent::s_ptr event)
    {
        log(LogLevel::ERROR, event);
    }
    void Logger::fatal(LogEvent::s_ptr event)
    {
        log(LogLevel::FATAL, event);
    }
    // 增删日志输出地
    void Logger::addAppender(LogAppender::s_ptr appender)
    {
        m_appenders.push_back(appender);
    }
    void Logger::deleteAppender(LogAppender::s_ptr appender)
    {
        for (auto it = m_appenders.begin(); it != m_appenders.end();) // 迭代器遍历
        {
            if (*it == appender)
            {
                m_appenders.erase(it);
                break;
            }
            ++it;
        }
    }
    // 输出到控制台的Appender
    void StdoutLogAppender::log(LogLevel::Level level, LogEvent::s_ptr event)
    {
        if (level >= m_level)
        {
            // 格式化后输出到控制台
            std::cout << m_formatter->format(event);
        }
    }

    // 输出到文件的Appender:需要文件地址
    FileoutLogAppender::FileoutLogAppender(const std::string &filename) : m_filename(filename)
    {
    }
    //
    bool FileoutLogAppender::reopen()
    {
        if(m_filestream)
        {
            m_filestream.close();
        }
        m_filestream.open(m_filename);
        return !!m_filestream;//!! 是一个技巧，把流对象 转换为 bool，确保返回值是 true/false。
    }
    void FileoutLogAppender ::log(LogLevel::Level level, LogEvent::s_ptr event)
    {
        if (level >= m_level)
        {
            // 格式化后输出到文件
            m_filestream << m_formatter->format(event);
        }
    }

}