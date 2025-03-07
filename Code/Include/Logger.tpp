#pragma once

#include <format>
#include <iostream>
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
        const std::source_location l_location = std::source_location::current();
        LogInternal(a_level, l_location, std::vformat(a_format, std::make_format_args(a_message...)));
    }

    /*template <typename LogMessage>
    void Logger::LogVerbose(const LogMessage& a_message, const std::source_location& a_location)
    {
        Log(LogLevel::VERBOSE, a_message, a_location);
    }

    template <typename LogMessage>
    void Logger::LogInfo(const LogMessage& a_message, const std::source_location& a_location)
    {
        Log(LogLevel::INFO, a_message, a_location);
    }

    template <typename LogMessage>
    void Logger::LogWarning(const LogMessage& a_message, const std::source_location& a_location)
    {
        Log(LogLevel::WARNING, a_message, a_location);
    }

    template <typename LogMessage>
    void Logger::LogError(const LogMessage& a_message, const std::source_location& a_location)
    {
        Log(LogLevel::ERROR, a_message, a_location);
    }

    template <typename LogMessage>
    void Logger::LogCritical(const LogMessage& a_message, const std::source_location& a_location)
    {
        Log(LogLevel::CRITICAL, a_message, a_location);
    }*/
}
