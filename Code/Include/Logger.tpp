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
    inline std::string ToString<std::string>(const std::string& a_value)
    {
        return a_value;
    }

    template <>
    inline std::string ToString(const bool& a_value)
    {
        return a_value ? "true" : "false";
    }

    template <>
    inline std::string ToString(const float& a_value)
    {
        return std::format("{}f", a_value);
    }

    template <>
    inline std::string ToString(const double& a_value)
    {
        return std::format("{}", a_value);
    }

    inline std::string ToString(const char* a_char) { return a_char; }

    template <typename LogMessage>
    void Logger::Log(const LogLevel& a_level, const LogMessage& a_message,
                     const std::source_location& a_location)
    {
        if (a_level < m_minLogLevel)
            return;

        std::lock_guard l_lock(m_logMutex);
        const std::string l_formattedMsg =
            FormatMessage(a_level, ToString(a_message), a_location);

        std::cout << l_formattedMsg << "\n";

        if (m_logFile.is_open())
            m_logFile << l_formattedMsg << "\n";
    }

    template <typename LogMessage>
    void Logger::LogDebug(const LogMessage& a_message, const std::source_location& a_location)
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
    }
} // namespace Debug
