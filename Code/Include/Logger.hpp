#pragma once

#include <fstream>
#include <mutex>
#include <source_location>

#include "LoggerExport.hpp"
#include "LogLevel.hpp"
#include "Utils.hpp"

using Debug::Utils::operator""_MiB;

#define DEBUG_LOG(level, message) Debug::Logger::GetInstance().Log(level, message)
#define DEBUG_LOG_VERBOSE(message) Debug::Logger::GetInstance().LogVerbose(message)
#define DEBUG_LOG_INFO(message) Debug::Logger::GetInstance().LogInfo(message)
#define DEBUG_LOG_WARNING(message) Debug::Logger::GetInstance().LogWarning(message)
#define DEBUG_LOG_ERROR(message) Debug::Logger::GetInstance().LogError(message)
#define DEBUG_LOG_CRITICAL(message) Debug::Logger::GetInstance().LogCritical(message)

namespace Debug
{
    class LUMINOUS_LOGGER_API Logger
    {
    public:
        /**
          * @brief Function to get the instance of the logger
          * @return The instance of the logger. Useful to print messages
          */
        static Logger& GetInstance();

        /**
         *
         * @param a_filename Defines a custom name for log files. Do not specify the extension as all files are saved as .log
         * @param a_maxFileSize Defines the max size before rotating towards a new log file
         * @param a_maxFiles Defines the max amount of log files before deleting the oldest log file
         */
        void Init(const std::string& a_filename, const size_t& a_maxFileSize, const size_t& a_maxFiles, const bool& a_useColors);

        template<typename LogMessage>
        void LogVerbose(const LogMessage& a_message = "Empty Message",
                        const std::source_location& a_location = std::source_location::current());

        template<typename LogMessage>
        void LogInfo(const LogMessage& a_message = "Empty Message",
                     const std::source_location& a_location = std::source_location::current());

        template<typename LogMessage>
        void LogWarning(const LogMessage& a_message = "Empty Message",
                        const std::source_location& a_location = std::source_location::current());

        template<typename LogMessage>
        void LogError(const LogMessage& a_message = "Empty Message",
                      const std::source_location& a_location = std::source_location::current());

        template<typename LogMessage>
        void LogCritical(const LogMessage& a_message = "Empty Message",
                         const std::source_location& a_location = std::source_location::current());

        template<typename LogMessage>
        void Log(const LogLevel& a_level = LogLevel::VERBOSE, const LogMessage& a_message = "Empty Message",
                 const std::source_location& a_location = std::source_location::current());

    private:
        explicit Logger() = default;

        ~Logger();

        /**
         * @brief Helper function to trigger log file rotation
         */
        void RotateLogs();

        /**
         * @brief Helper function to check the log file size in order to trigger rotation
         */
        void CheckLogFileSize();

        /**
         * @brief Helper function to format message for console printing (With color support)
         * @param a_level The log level
         * @param a_message The log message
         * @param a_useColors Whether the message should be printed using colors
         * @param a_location The location to show in the debug message
         * @return A formatted message with or without colors
         */
        std::string FormatMessage(const LogLevel& a_level, const std::string& a_message,
                                         const bool& a_useColors = false,
                                         const std::source_location& a_location = std::source_location::current());

        /**
         * @brief
         * @param a_level The log level
         * @param a_useColors Whether the message should be printed using color
         * @return A formatted string of the Log Level
         */
        std::string LogLevelToString(const LogLevel& a_level, const bool& a_useColors = false);

        /**
         *
         * @param a_location The location to show in the debug message
         * @return Either the specified location, or, in not specified, the location of the log message
         */
        static std::string GetSourceLocation(const std::source_location& a_location);

        std::ofstream m_logFile{};
        std::string m_logFilename{"app"};
        std::mutex m_logMutex{};
        LogLevel m_minLogLevel{LogLevel::VERBOSE};

        size_t m_maxFileSize{1_MiB};
        size_t m_maxFiles{5};

        bool m_useColors{true};
    };
}

#include "Logger.tpp"
