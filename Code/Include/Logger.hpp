#pragma once

#include <fstream>
#include <mutex>
#include <source_location>

#include "LogLevel.hpp"
#include "Utils.hpp"

using Debug::Utils::operator""_MiB;

namespace Debug
{
    class Logger
    {
    public:
        static Logger& GetInstance();

        void SetLogLevel(const LogLevel& a_level);
        void SetLogFile(const std::string& a_filename, const size_t& a_maxFileSize, const size_t& a_maxFiles);

        template <typename LogMessage>
        void LogDebug(const LogMessage& a_message = "Empty Message",
                      const std::source_location& a_location = std::source_location::current());

        template <typename LogMessage>
        void LogInfo(const LogMessage& a_message = "Empty Message",
                     const std::source_location& a_location = std::source_location::current());

        template <typename LogMessage>
        void LogWarning(const LogMessage& a_message = "Empty Message",
                        const std::source_location& a_location = std::source_location::current());

        template <typename LogMessage>
        void LogError(const LogMessage& a_message = "Empty Message",
                      const std::source_location& a_location = std::source_location::current());

        template <typename LogMessage>
        void LogCritical(const LogMessage& a_message = "Empty Message",
                         const std::source_location& a_location = std::source_location::current());

        template <typename LogMessage>
        void Log(const LogLevel& a_level = LogLevel::VERBOSE, const LogMessage& a_message = "Empty Message",
                 const std::source_location& a_location = std::source_location::current());

    private:
        explicit Logger() = default;
        ~Logger();

        void RotateLogs();
        void CheckLogFileSize();

        static std::string FormatConsoleMessage(const LogLevel& a_level, const std::string& a_message,
                                                const std::source_location& a_location);

        static std::string FormatLogFileMessage(const LogLevel& a_level, const std::string& a_message,
                                                const std::source_location& a_location);

        static std::string LogLevelToString(const LogLevel& a_level);
        static std::string ColoredLogLevelToString(const LogLevel& a_level);
        static std::string GetSourceLocation(const std::source_location& a_location);

        std::ofstream m_logFile{};
        std::mutex m_logMutex{};
        LogLevel m_minLogLevel{LogLevel::VERBOSE};

        std::string m_logFilename{};
        size_t m_maxFileSize{1_MiB};
        size_t m_maxFiles{5};
    };
}

#include "Logger.tpp"
