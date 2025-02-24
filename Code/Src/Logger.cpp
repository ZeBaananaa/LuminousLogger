#include "Logger.hpp"

#include <iostream>

Debug::Logger &Debug::Logger::GetInstance()
{
    static Logger s_instance{};

    return s_instance;
}

Debug::Logger::~Logger()
{
    if (m_logFile.is_open())
        m_logFile.close();
}

void Debug::Logger::SetLogLevel(const LogLevel &a_level)
{
    std::lock_guard l_lock(m_logMutex);
    m_minLogLevel = a_level;
}

void Debug::Logger::SetLogFile(const std::string &a_filename)
{
    std::lock_guard l_lock(m_logMutex);
    m_logFile.open(a_filename, std::ios::app);
}

void Debug::Logger::Log(const LogLevel &a_level, const std::string &a_message)
{
    if (a_level < m_minLogLevel)
        return;

    std::lock_guard l_lock(m_logMutex);
    const std::string l_formattedMsg = FormatMessage(a_level, a_message);

    std::cout << l_formattedMsg << "\n";

    if (m_logFile.is_open())
        m_logFile << l_formattedMsg << "\n";
}

void Debug::Logger::LogDebug(const std::string &a_message) { Log(LogLevel::VERBOSE, a_message); }
void Debug::Logger::LogInfo(const std::string &a_message) { Log(LogLevel::INFO, a_message); }
void Debug::Logger::LogWarning(const std::string &a_message) { Log(LogLevel::WARNING, a_message); }
void Debug::Logger::LogError(const std::string &a_message) { Log(LogLevel::ERROR, a_message); }
void Debug::Logger::LogCritical(const std::string &a_message) { Log(LogLevel::CRITICAL, a_message); }

std::string Debug::Logger::FormatMessage(const LogLevel &a_level, const std::string &a_message)
{
    const std::chrono::time_point<std::chrono::system_clock> l_now = std::chrono::system_clock::now();
    const time_t l_time = std::chrono::system_clock::to_time_t(l_now);

    std::string timeStr = std::ctime(&l_time);
    timeStr.pop_back();

    return LogLevelToString(a_level) + "[" + timeStr + "] " + ": " + a_message;
}

std::string Debug::Logger::LogLevelToString(const LogLevel &a_level)
{
    switch (a_level)
    {
        case LogLevel::VERBOSE:
            return "VERBOSE       : ";

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
