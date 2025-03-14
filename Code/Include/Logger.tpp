#pragma once

#include <format>
#include <string>

namespace Debug
{
    template<typename LogMessage>
    std::string ToString(const LogMessage& a_message)
    {
        return std::to_string(a_message);
    }

    template<>
    inline std::string ToString<std::string>(const std::string& a_message)
    {
        return a_message;
    }

    template<typename... LogMessage>
    void Logger::Log(const LogLevel a_level, const FormatLocation a_formatLoc, LogMessage&&... a_message)
    {
        LogInternal(a_level, a_formatLoc.location,
                    std::vformat(a_formatLoc.format, std::make_format_args(a_message...)));
    }

    template<typename... LogMessage>
    void Logger::LogVerbose(const FormatLocation a_formatLoc, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::VERBOSE, a_formatLoc.location,
                    std::vformat(a_formatLoc.format, std::make_format_args(a_message...)));
    }

    template<typename... LogMessage>
    void Logger::LogInfo(const FormatLocation a_formatLoc, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::INFO, a_formatLoc.location,
                    std::vformat(a_formatLoc.format, std::make_format_args(a_message...)));
    }

    template<typename... LogMessage>
    void Logger::LogWarning(const FormatLocation a_formatLoc, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::WARNING, a_formatLoc.location,
                    std::vformat(a_formatLoc.format, std::make_format_args(a_message...)));
    }

    template<typename... LogMessage>
    void Logger::LogError(const FormatLocation a_formatLoc, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::ERROR, a_formatLoc.location,
                    std::vformat(a_formatLoc.format, std::make_format_args(a_message...)));
    }

    template<typename... LogMessage>
    void Logger::LogCritical(const FormatLocation a_formatLoc, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::CRITICAL, a_formatLoc.location,
                    std::vformat(a_formatLoc.format, std::make_format_args(a_message...)));
    }
}
