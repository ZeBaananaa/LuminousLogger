#include "Logger.hpp"

#include <iostream>
#include <source_location>

namespace Debug
{
    Logger& Logger::GetInstance()
    {
        static Logger s_instance{};

        return s_instance;
    }

    Logger::~Logger()
    {
        if (m_logFile.is_open())
            m_logFile.close();
    }

    void Logger::SetLogLevel(const LogLevel& a_level)
    {
        std::lock_guard l_lock(m_logMutex);
        m_minLogLevel = a_level;
    }

    void Logger::SetLogFile(const std::string& a_filename)
    {
        std::lock_guard l_lock(m_logMutex);
        m_logFile.open(a_filename, std::ios::app);
    }

    std::string GetSourceLocation(const std::source_location& a_location = std::source_location::current())
    {
        const std::string l_outputMsg = std::string(a_location.file_name()) + " (" + std::to_string(a_location.line()) +
            ":" +
            std::to_string(a_location.column()) + ")";

        return l_outputMsg;
    }

    std::string Logger::FormatMessage(const LogLevel& a_level, const std::string& a_message)
    {
        const std::chrono::time_point<std::chrono::system_clock> l_now = std::chrono::system_clock::now();
        const time_t l_time = std::chrono::system_clock::to_time_t(l_now);
        const std::string l_location = GetSourceLocation(std::source_location::current());

        std::string l_timeStr = std::ctime(&l_time);
        l_timeStr.pop_back();

        /*return LogLevelToString(a_level) + "[ " + l_timeStr + " ] " + " - " + " [ " + l_location + " ] " + ": " +
            a_message;*/

        return std::format("{} [ {} ] - [ {} ] : {}", LogLevelToString(a_level), l_timeStr, l_location, a_message);
    }

    std::string Logger::LogLevelToString(const LogLevel& a_level)
    {
        switch (a_level)
        {
        case LogLevel::VERBOSE:
            return std::format("{} VERBOSE       : ", ColorMap.at(ColorEnum::BOLD_BLUE));

        case LogLevel::INFO:
            return "INFO          : ";

        case LogLevel::WARNING:
            return "WARNING       : ";

        case LogLevel::ERROR:
            return "ERROR         : ";

        case LogLevel::CRITICAL:
            return "CRITICAL      : ";

        default:
            return "VERBOSE       : ";
        }
    }
}
