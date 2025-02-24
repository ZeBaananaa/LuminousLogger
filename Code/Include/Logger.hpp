#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <fstream>
#include <mutex>

#include "LogLevel.hpp"

namespace Debug
{
    class Logger
    {
    public:
        static Logger &GetInstance();

        void SetLogLevel(const LogLevel &a_level);

        void SetLogFile(const std::string &a_filename);

        void Log(const LogLevel &a_level, const std::string &a_message);

        void LogDebug(const std::string &a_message);

        void LogInfo(const std::string &a_message);

        void LogWarning(const std::string &a_message);

        void LogError(const std::string &a_message);

        void LogCritical(const std::string &a_message);

    private:
        Logger() = default;

        ~Logger();

        Logger(const Logger &);

        Logger &operator=(const Logger &);

        std::ofstream m_logFile{};
        std::mutex m_logMutex{};
        LogLevel m_minLogLevel{LogLevel::VERBOSE};

        static std::string FormatMessage(const LogLevel &a_level, const std::string &a_message);

        static std::string LogLevelToString(const LogLevel &a_level);
    };
}

#endif
