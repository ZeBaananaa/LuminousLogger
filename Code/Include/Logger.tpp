#pragma once

#include <format>
#include <string>

namespace Debug
{
    template <typename LogMessage>
    std::string ToString(const LogMessage& a_message)
    {
        return std::to_string(a_message);
    }

    template <>
    inline std::string ToString<std::string>(const std::string& a_message)
    {
        return a_message;
    }

    template <>
    inline std::string ToString(const bool& a_message)
    {
        return a_message ? "true" : "false";
    }

    template <>
    inline std::string ToString(const float& a_message)
    {
        return std::format("{}f", a_message);
    }

    template <>
    inline std::string ToString(const double& a_message)
    {
        return std::format("{}", a_message);
    }

    inline std::string ToString(const char* a_char) { return a_char; }


    template <typename... LogMessage>
    void Logger::Log(const LogLevel a_level, const std::string_view a_format, LogMessage&&... a_message)
    {
        LogInternal(a_level, std::source_location::current(), std::vformat(a_format, std::make_format_args(a_message...)));
    }

    template <typename... LogMessage>
    void Logger::LogVerbose(const std::string_view a_format, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::VERBOSE, std::source_location::current(), std::vformat(a_format, std::make_format_args(a_message...)));
    }

    template <typename... LogMessage>
    void Logger::LogInfo(const std::string_view a_format, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::INFO, std::source_location::current(), std::vformat(a_format, std::make_format_args(a_message...)));
    }

    template <typename... LogMessage>
    void Logger::LogWarning(const std::string_view a_format, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::WARNING, std::source_location::current(), std::vformat(a_format, std::make_format_args(a_message...)));
    }

    template <typename... LogMessage>
    void Logger::LogError(const std::string_view a_format, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::ERROR, std::source_location::current(), std::vformat(a_format, std::make_format_args(a_message...)));
    }

    template <typename... LogMessage>
    void Logger::LogCritical(const std::string_view a_format, LogMessage&&... a_message)
    {
        LogInternal(LogLevel::CRITICAL, std::source_location::current(), std::vformat(a_format, std::make_format_args(a_message...)));
    }
}
