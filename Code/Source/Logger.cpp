#include "Logger.hpp"

#include "LogColor.hpp"

#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

#include "fmt/format.h"

constexpr std::string_view LATEST_LOG_FILE_SUFFIX{"-latest"};
constexpr std::string_view LOG_FILE_FORMAT{".log"};
constexpr std::string_view OLD_FILE_SUFFIX{".old"};

namespace Debug
{
    // Creates a queue with a capacity of 100 messages.
    Logger::Logger() :
        m_logQueue(64) {}

    Logger::~Logger()
    {
        StopThread();

        if (m_logFile.is_open())
            m_logFile.close();
    }

    Logger::Logger(const Logger& a_copy) :
        m_logQueue(a_copy.m_logQueue.GetCapacity()),
        m_logFilename(a_copy.m_logFilename),
        m_maxFileSize(a_copy.m_maxFileSize),
        m_maxFiles(a_copy.m_maxFiles),
        m_useColors(a_copy.m_useColors) { std::cout << "Logger was copied then deleted!\n"; }

    Logger& Logger::operator=(const Logger& a_other)
    {
        if (this != &a_other)
        {
            std::lock_guard<std::mutex> l_lock(m_logMutex);

            m_logFilename = a_other.m_logFilename;
            m_maxFileSize = a_other.m_maxFileSize;
            m_maxFiles = a_other.m_maxFiles;
            m_useColors = a_other.m_useColors;

            std::cout << "Logger assigned\n";
        }
        return *this;
    }

    Logger& Logger::GetInstance()
    {
        static Logger s_instance{ };
        return s_instance;
    }

    void Logger::Init(const std::string& a_filename, const size_t a_maxFileSize, const size_t a_maxFiles,
                      const bool a_useColors)
    {
        std::lock_guard<std::mutex> l_lock(m_logMutex);
        m_logFilename = a_filename;
        m_maxFileSize = a_maxFileSize;
        m_maxFiles = a_maxFiles;
        m_useColors = a_useColors;

        m_logFile.open(fmt::format("{}{}{}", a_filename, LATEST_LOG_FILE_SUFFIX, LOG_FILE_FORMAT),
                       std::ios::out | std::ios::app | std::ios::ate);

        if (!m_logFile.is_open())
            fmt::print(stderr, "File {}{}{} could not be opened!\n", m_logFilename, LATEST_LOG_FILE_SUFFIX,
                       LOG_FILE_FORMAT);

        m_loggingThread = std::thread(&Logger::PrintLogs, this);
    }

    void Logger::LogInternal(const LogLevel a_level, const std::source_location& a_location,
                             const std::string& a_message)
    {
        std::lock_guard l_lock(m_logMutex);
        const std::string l_formattedConsoleMsg{FormatMessage(a_level, ToString(a_message), true, a_location)};
        const std::string l_formattedLogFileMsg{FormatMessage(a_level, ToString(a_message), false, a_location)};

        m_logQueue.PushLogToQueue(l_formattedLogFileMsg);
        fmt::print("{}\n", l_formattedConsoleMsg);
    }

    void Logger::PrintLogs()
    {
        while (!m_stopLogger)
        {
            if (std::optional<std::string> l_log = m_logQueue.PopLogFromQueue())
            {
                if (!m_logFile.is_open()) { m_logFile << *l_log << "\n"; }
            }
            else { std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
        }
        CheckLogFileSize();
    }

    void Logger::StopThread()
    {
        m_stopLogger = true;

        if (m_loggingThread.joinable())
            m_loggingThread.join();
    }

    void Logger::RotateLogs()
    {
        m_logFile.close();
        for (size_t i = m_maxFiles - 1; i > 0; --i)
        {
            std::filesystem::rename(fmt::format("{}-{}{}{}", m_logFilename, i, OLD_FILE_SUFFIX, LOG_FILE_FORMAT),
                                    fmt::format("{}-{}{}{}", m_logFilename, i + 1, OLD_FILE_SUFFIX, LOG_FILE_FORMAT));
        }
        std::filesystem::rename(fmt::format("{}{}{}", m_logFilename, LATEST_LOG_FILE_SUFFIX, LOG_FILE_FORMAT),
                                fmt::format("{}-1{}{}", m_logFilename, OLD_FILE_SUFFIX, LOG_FILE_FORMAT));

        m_logFile.open(fmt::format("{}{}{}", m_logFilename, LATEST_LOG_FILE_SUFFIX, LOG_FILE_FORMAT), std::ios::trunc);
    }

    void Logger::CheckLogFileSize()
    {
        const std::string l_latestFile = std::format("{}{}{}", m_logFilename, LATEST_LOG_FILE_SUFFIX, LOG_FILE_FORMAT);
        if (std::filesystem::exists(l_latestFile) && std::filesystem::file_size(l_latestFile) >= m_maxFileSize)
            RotateLogs();
    }

    std::string Logger::GetSourceLocation(const std::source_location& a_location = std::source_location::current())
    {
        const std::string_view l_filePath{a_location.file_name()};

        const std::filesystem::path l_fullPath = std::filesystem::path(l_filePath);
        const std::filesystem::path l_baseDir = FindProjectRoot(l_fullPath);

        const std::filesystem::path l_relativePath = std::filesystem::relative(l_fullPath, l_baseDir);

        return fmt::format("{} ({}:{}) - {}", l_relativePath.string(), a_location.line(), a_location.column(), a_location.function_name());
    }

    std::filesystem::path Logger::FindProjectRoot(const std::filesystem::path& a_startPath)
    {
        std::filesystem::path l_currentPath = a_startPath;

        while (l_currentPath != l_currentPath.root_path())
        {
            if (std::filesystem::exists(l_currentPath / "CMakeLists.txt"))
                return l_currentPath;

            l_currentPath = l_currentPath.parent_path();
        }

        return std::filesystem::path{"/"};
    }

    std::string Logger::FormatMessage(const LogLevel a_level, const std::string& a_message, const bool a_useColors,
                                      const std::source_location& a_location) const
    {
        const std::chrono::time_point l_now{std::chrono::system_clock::now()};
        std::string l_timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", l_now);

        if (m_useColors && a_useColors)
            return fmt::format("{} [{}] - [{}] : {}{}{}", LogLevelToString(a_level, true), l_timestamp,
                               GetSourceLocation(a_location), s_LogLevelColor.at(a_level), a_message,
                               ColorMap.at(ColorEnum::RESET));

        return fmt::format("{} [{}] - [{}] : {}", LogLevelToString(a_level, false), l_timestamp,
                           GetSourceLocation(a_location), a_message);
    }

    std::string Logger::LogLevelToString(const LogLevel a_level, const bool a_useColors) const
    {
        const std::unordered_map<LogLevel, std::string> l_logLevelStrings
        {
            {LogLevel::VERBOSE, "VERBOSE    "},
            {LogLevel::INFO, "INFO       "},
            {LogLevel::WARNING, "WARNING    "},
            {LogLevel::ERROR, "ERROR      "},
            {LogLevel::CRITICAL, "CRITICAL   "}
        };

        std::string l_levelStr = l_logLevelStrings.at(a_level);
        return (m_useColors && a_useColors)
            ? fmt::format("{}{}{}", s_LogLevelColor.at(a_level), l_levelStr, ColorMap.at(ColorEnum::RESET))
            : l_levelStr;
    }
}
