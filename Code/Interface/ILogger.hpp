#ifndef LOGGER_HPP
#define LOGGER_HPP

#define LOG(logger, level, message) (logger).log(message, level, __FILE__, __LINE__)
#define LOG_DEBUG(logger, message) LOG(logger, logtard::LogLevel::DEBUG, message)
#define LOG_INFO(logger, message) LOG(logger, logtard::LogLevel::INFO, message)
#define LOG_WARNING(logger, message) LOG(logger, logtard::LogLevel::WARNING, message)
#define LOG_ERROR(logger, message) LOG(logger, logtard::LogLevel::ERROR, message)
#define LOG_CRITICAL(logger, message) LOG(logger, logtard::LogLevel::CRITICAL, message)

#include <mutex>
#include <string>
#include <filesystem>

namespace LuminousLogger
{
    enum class LogLevel { VERBOSE, INFO, WARNING, ERROR, CRITICAL };

    class Logger
    {
    public:
        virtual ~Logger() = default;

        virtual void Log(const std::string &a_message, LogLevel a_level, const std::filesystem::path &a_file,
                         int a_line) = 0;

    protected:
        std::mutex p_logMutex{};

        static std::string LogLevelToString(LogLevel a_level);
    };
}

#endif
