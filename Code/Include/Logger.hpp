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
        void SetLogFile(const std::string& a_filename, const size_t& a_maxFileSize, const size_t& a_maxFiles);

        template <typename LogMessage>
        void LogVerbose(const LogMessage& a_message = "Empty Message",
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
         * @param a_useColor Whether the message should be printed using colors
         * @param a_location The location to show in the debug message
         * @return A formatted message with colors
         */
        static std::string FormatMessage(const LogLevel& a_level, const std::string& a_message,
                                         const bool& a_useColor = false, const std::source_location& a_location = std::source_location::current());

        static std::string LogLevelToString(const LogLevel& a_level, const bool& a_useColor = false);

        /**
         *
         * @param a_location The location to show in the debug message
         * @return Either the specified location, or, in not specified, the location of the log message
         */
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
