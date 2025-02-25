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

        void SetLogLevel(const LogLevel& a_level);

        void SetLogFile(const std::string& a_filename);

        template<typename LogMessage>
        void LogDebug(const LogMessage& a_message = "Empty Message");

        template<typename LogMessage>
        void LogInfo(const LogMessage& a_message = "Empty Message");

        template<typename LogMessage>
        void LogWarning(const LogMessage& a_message = "Empty Message");

        template<typename LogMessage>
        void LogError(const LogMessage& a_message = "Empty Message");

        template<typename LogMessage>
        void LogCritical(const LogMessage& a_message = "Empty Message");

        template<typename LogMessage>
        void Log(const LogLevel& a_level = LogLevel::VERBOSE, const LogMessage& a_message = "Empty Message");

    private:
        explicit Logger() = default;

        ~Logger();

        Logger(const Logger&);

        Logger &operator=(const Logger&);

        static std::string FormatMessage(const LogLevel& a_level, const std::string& a_message);

        static std::string LogLevelToString(const LogLevel& a_level);

        std::ofstream m_logFile{};
        std::mutex m_logMutex{};
        LogLevel m_minLogLevel{LogLevel::VERBOSE};
    };
}

#include "Logger.tpp"
#endif
