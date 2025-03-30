#pragma once

#include <string>

namespace Debug
{
    template <typename LogMessage>
    std::string ToString(const LogMessage& a_message) { return fmt::to_string(a_message); }

    template <>
    inline std::string ToString<std::string>(const std::string& a_message) { return a_message; }

    template <typename... LogMessage>
    void Logger::Log(const LogLevel a_level, const FormatLocation a_formatLoc, LogMessage&&... a_message)
    {
        LogInternal(a_level, a_formatLoc.location,
                    fmt::vformat(a_formatLoc.format, fmt::make_format_args(a_message...)));
    }

    template <typename... LogMessage>
    void Logger::LogVerbose(const FormatLocation a_formatLoc, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::VERBOSE, a_formatLoc.location,
                    fmt::vformat(a_formatLoc.format, fmt::make_format_args(a_message...)));
    }

    template <typename... LogMessage>
    void Logger::LogInfo(const FormatLocation a_formatLoc, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::INFO, a_formatLoc.location,
                    fmt::vformat(a_formatLoc.format, fmt::make_format_args(a_message...)));
    }

    template <typename... LogMessage>
    void Logger::LogWarning(const FormatLocation a_formatLoc, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::WARNING, a_formatLoc.location,
                    fmt::vformat(a_formatLoc.format, fmt::make_format_args(a_message...)));
    }

    template <typename... LogMessage>
    void Logger::LogError(const FormatLocation a_formatLoc, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::ERROR, a_formatLoc.location,
                    fmt::vformat(a_formatLoc.format, fmt::make_format_args(a_message...)));
    }

    template <typename... LogMessage>
    void Logger::LogCritical(const FormatLocation a_formatLoc, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::CRITICAL, a_formatLoc.location,
                    fmt::vformat(a_formatLoc.format, fmt::make_format_args(a_message...)));
    }

    template <typename... LogMessage>
    void Logger::HandleAssertion(const bool a_condition, const AssertLevel a_level, const FormatLocation a_formatLoc, LogMessage&&... a_message)
    {
        if (!a_condition)
        {
            const LogLevel l_logLevel = a_level == AssertLevel::ERROR ? LogLevel::ERROR : LogLevel::WARNING;
            const std::string l_formattedMessage = fmt::vformat(a_formatLoc.format, fmt::make_format_args(a_message...));

            LogInternal(l_logLevel, a_formatLoc.location, l_formattedMessage);
            FlushLogs();

            if (a_level == AssertLevel::ERROR)
            {
                m_logFile.flush();

                #ifdef _DEBUG
                    DEBUG_BREAK();
                #else
                    std::abort();
                #endif
            }
        }
    }
}
