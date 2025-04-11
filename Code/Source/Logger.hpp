#pragma once

#include <filesystem>
#include <fstream>
#include <mutex>
#include <source_location>
#include <thread>

#include "AssertLevel.hpp"
#include "LockFreeQueue.hpp"
#include "LogLevel.hpp"
#include "Utils.hpp"
#include "fmt/color.h"
#include "fmt/os.h"

using Debug::Utils::operator""_MiB;

#if defined(_WIN32) || defined(_WIN64)
#include <intrin.h>
#define DEBUG_BREAK() __debugbreak()
#else
#include <csignal>
#define DEBUG_BREAK() std::raise(SIGTRAP)
#endif

#define DEBUG_LOG(level, ...) Debug::Logger::GetInstance().Log(level, ##__VA_ARGS__)
#define DEBUG_LOG_VERBOSE(...) Debug::Logger::GetInstance().LogVerbose(__VA_ARGS__)
#define DEBUG_LOG_INFO(...) Debug::Logger::GetInstance().LogInfo(__VA_ARGS__)
#define DEBUG_LOG_WARNING(...) Debug::Logger::GetInstance().LogWarning(__VA_ARGS__)
#define DEBUG_LOG_ERROR(...) Debug::Logger::GetInstance().LogError(__VA_ARGS__)
#define DEBUG_LOG_CRITICAL(...) Debug::Logger::GetInstance().LogCritical(__VA_ARGS__)

#define LOG_ASSERT(condition, level, ...) Debug::Logger::GetInstance().HandleAssertion((condition), (level), ##__VA_ARGS__)
#define LOG_ASSERT_WARN(condition, ...) Debug::Logger::GetInstance().HandleAssertion((condition), Debug::AssertLevel::WARN, ##__VA_ARGS__)
#define LOG_ASSERT_ERROR(condition, ...) Debug::Logger::GetInstance().HandleAssertion((condition), Debug::AssertLevel::ERROR, ##__VA_ARGS__)

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
         * @brief
         * @param a_filename Defines a custom name for log files. Do not specify the extension as all files are saved as .log
         * @param a_maxFileSize Defines the max size before rotating towards a new log file
         * @param a_maxFiles Defines the max amount of log files before deleting the oldest log file
         * @param a_useColors Whether we should enable color support for the logger, or not
         */
        void Init(const std::string& a_filename, size_t a_maxFileSize, size_t a_maxFiles, bool a_useColors);


        struct FormatLocation
        {
            std::string_view format{ };
            std::source_location location{ };

            template <typename LogMessage>
            FormatLocation(LogMessage&& a_logMessage,
                           const std::source_location a_location = std::source_location::current()) :
                format{std::forward<LogMessage>(a_logMessage)}, location{a_location} {}
        };


        template <typename... LogMessage>
        void LogVerbose(FormatLocation a_formatLoc, LogMessage&&... a_message);

        template <typename... LogMessage>
        void LogInfo(FormatLocation a_formatLoc, LogMessage&&... a_message);

        template <typename... LogMessage>
        void LogWarning(FormatLocation a_formatLoc, LogMessage&&... a_message);

        template <typename... LogMessage>
        void LogError(FormatLocation a_formatLoc, LogMessage&&... a_message);

        template <typename... LogMessage>
        void LogCritical(FormatLocation a_formatLoc, LogMessage&&... a_message);

        template <typename... LogMessage>
        void Log(LogLevel a_level, FormatLocation a_formatLoc, LogMessage&&... a_message);

        template <typename... LogMessage>
        void HandleAssertion(bool a_condition, AssertLevel a_level, FormatLocation a_formatLoc, LogMessage&&... a_message);

        void FlushLogs();

    private:
        explicit Logger() = default;
        ~Logger();

        Logger(const Logger&); // Prevents singleton copy
        Logger& operator=(const Logger&); // Prevents singleton assignment

        void PrintLogs();
        void StopThread();
        void LogInternal(LogLevel a_level, const std::source_location& a_location, const std::string& a_message);

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
        std::string FormatMessage(LogLevel a_level, const std::string& a_message, bool a_useColors,
                                  const std::source_location& a_location) const;

        /**
         * @brief
         * @param a_level The log level
         * @return A formatted string of the Log Level
         */
        static std::string LogLevelToString(LogLevel a_level);

        /**
         * @brief
         * @param a_level The log level to use
         * @return a color based on the log level
         */
        static fmt::text_style GetLogColor(LogLevel a_level);

        /**
         * @brief
         * @param a_location The location to show in the debug message
         * @return Either the specified location, or, in not specified, the location of the log message
         */
        static std::string GetSourceLocation(const std::source_location& a_location);

        /**
         * @brief Helper function to get a proper relative path
         * @param a_startPath The path from which we should start the recursive search (usually the current path)
         * @return The project path
         */
        static std::filesystem::path FindProjectRoot(const std::filesystem::path& a_startPath);

        std::thread m_loggingThread{ };
        LockFreeQueue m_logQueue{ 24 };

        bool m_stopLogger = false;
        std::ofstream m_logFile{ };
        std::string m_logFilename{"app"};
        std::mutex m_logMutex{ };

        size_t m_maxFileSize{1_MiB};
        size_t m_maxFiles{5};

        bool m_useColors{true};
    };
}

#include "Logger.tpp"
