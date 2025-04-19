#include "Logger.hpp"

#include <chrono>
#include <csignal>
#include <filesystem>
#include <iostream>
#include <thread>

#include "fmt/chrono.h"
#include "fmt/color.h"
#include "fmt/format.h"

constexpr std::string_view LATEST_LOG_FILE_SUFFIX{"-latest"};
constexpr std::string_view LOG_FILE_FORMAT{".log"};
constexpr std::string_view OLD_FILE_SUFFIX{".old"};

namespace Debug
{
    std::atomic<bool> Logger::s_emergencyFlush{false};

    void Logger::SignalHandler(const int a_signal)
    {
        s_emergencyFlush = true;
        auto& l_logger = GetInstance();
        l_logger.WriteBufferToFile();
        l_logger.FlushLogs();

        std::signal(a_signal, SIG_DFL);
        std::raise(a_signal);
    }

    Logger::~Logger()
    {
        StopThread();

        if (m_logFile.is_open())
            m_logFile.close();
    }

    Logger::Logger(const Logger& a_copy) : m_logQueue(a_copy.m_logQueue.GetCapacity()), m_logFilename(a_copy.m_logFilename), m_maxFileSize(a_copy.m_maxFileSize), m_maxFiles(a_copy.m_maxFiles), m_useColors(a_copy.m_useColors)
    {
        std::cout << "Logger was copied then deleted!\n";
    }

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

    void Logger::Init(const std::string& a_filename, const size_t a_maxFileSize, const size_t a_maxFiles, const bool a_useColors)
    {
        std::lock_guard<std::mutex> l_lock(m_logMutex);
        m_logFilename = a_filename;
        m_maxFileSize = a_maxFileSize;
        m_maxFiles = a_maxFiles;
        m_useColors = a_useColors;

        m_logBuffer.reserve(batchSize);

        m_logFile.open(fmt::format("{}{}{}", a_filename, LATEST_LOG_FILE_SUFFIX, LOG_FILE_FORMAT), std::ios::out | std::ios::app | std::ios::ate);

        if (!m_logFile.is_open())
            fmt::print(stderr, "File {}{}{} could not be opened!\n", m_logFilename, LATEST_LOG_FILE_SUFFIX, LOG_FILE_FORMAT);

        std::signal(SIGSEGV, SignalHandler);
        std::signal(SIGABRT, SignalHandler);
        std::signal(SIGFPE, SignalHandler);
        std::signal(SIGILL, SignalHandler);

        m_loggingThread = std::thread(&Logger::PrintLogs, this);
    }

    void Logger::LogInternal(const LogLevel a_level, const std::source_location& a_location, const std::string& a_message)
    {
        std::lock_guard l_lock(m_logMutex);
        const std::string l_formattedConsoleMsg{FormatMessage(a_level, ToString(a_message), true, a_location)};
        const std::string l_formattedLogFileMsg{FormatMessage(a_level, ToString(a_message), false, a_location)};

        while (!m_logQueue.TryEnqueue(l_formattedLogFileMsg))
            std::this_thread::sleep_for(std::chrono::microseconds(50));

        fmt::print("{}\n", l_formattedConsoleMsg);
    }

    void Logger::PrintLogs()
    {
        CheckLogFileSize();
        while (!m_stopLogger || !m_logQueue.IsEmpty())
        {
            std::optional<std::string> l_log = m_logQueue.PopLogFromQueue();
            if (l_log)
            {
                if (m_logFile.is_open())
                    m_logFile << *l_log << "\n";
            }
            else
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        if (m_logFile.is_open())
            m_logFile.flush();
    }

    void Logger::StopThread()
    {
        m_stopLogger = true;

        if (m_loggingThread.joinable())
        {
            m_loggingThread.join();
            FlushLogs();

            if (m_logFile.is_open())
                m_logFile.flush();
        }
    }

    void Logger::CheckLogFileSize()
    {
        const std::string l_latestFile = fmt::format("{}{}{}", m_logFilename, LATEST_LOG_FILE_SUFFIX, LOG_FILE_FORMAT);
        if (std::filesystem::exists(l_latestFile) && std::filesystem::file_size(l_latestFile) >= m_maxFileSize)
            RotateLogs();
    }

    void Logger::RotateLogs()
    {
        m_logFile.close();
        for (size_t l_i = m_maxFiles - 1; l_i > 0; --l_i)
        {
            std::string l_oldFile = fmt::format("{}-{}{}{}", m_logFilename, l_i, OLD_FILE_SUFFIX, LOG_FILE_FORMAT);
            std::string l_newFile = fmt::format("{}-{}{}{}", m_logFilename, l_i + 1, OLD_FILE_SUFFIX, LOG_FILE_FORMAT);

            if (std::filesystem::exists(l_oldFile))
                std::filesystem::rename(l_oldFile, l_newFile);
        }
        std::filesystem::rename(fmt::format("{}{}{}", m_logFilename, LATEST_LOG_FILE_SUFFIX, LOG_FILE_FORMAT),
                                fmt::format("{}-1{}{}", m_logFilename, OLD_FILE_SUFFIX, LOG_FILE_FORMAT));

        m_logFile.open(fmt::format("{}{}{}", m_logFilename, LATEST_LOG_FILE_SUFFIX, LOG_FILE_FORMAT), std::ios::trunc);
    }

    std::string Logger::GetSourceLocation(const std::source_location& a_location = std::source_location::current())
    {
        const std::filesystem::path l_fullPath = std::filesystem::path(a_location.file_name());
        const std::filesystem::path l_baseDir = FindProjectRoot(l_fullPath);
        const std::filesystem::path l_relativePath = std::filesystem::relative(l_fullPath, l_baseDir);

        return fmt::format("{} ({}:{}) - {}", l_relativePath.string(), a_location.line(), a_location.column(),
                           a_location.function_name());
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
        return "/";
    }

    std::string Logger::FormatMessage(const LogLevel a_level, const std::string& a_message, const bool a_useColors, const std::source_location& a_location) const
    {
        const std::chrono::time_point l_now = std::chrono::time_point_cast<std::chrono::seconds>(std::chrono::system_clock::now());
        const std::string l_timestamp = fmt::format("{:%Y-%m-%d %H:%M:%S}", l_now);

        if (m_useColors && a_useColors)
        {
            const fmt::text_style l_logStyle = GetLogColor(a_level);
            return fmt::format("{} [{}] - [{}] : {}", fmt::styled(LogLevelToString(a_level), l_logStyle), l_timestamp, GetSourceLocation(a_location), fmt::styled(a_message, l_logStyle));
        }
        return fmt::format("{} [{}] - [{}] : {}", LogLevelToString(a_level), l_timestamp, GetSourceLocation(a_location), a_message);
    }

    std::string Logger::LogLevelToString(const LogLevel a_level)
    {
        switch (a_level)
        {
        case LogLevel::VERBOSE :
            return "VERBOSE    ";
        case LogLevel::INFO :
            return "INFO       ";
        case LogLevel::WARNING :
            return "WARNING    ";
        case LogLevel::ERROR :
            return "ERROR      ";
        case LogLevel::CRITICAL :
            return "CRITICAL   ";
        default :
            return "VERBOSE    ";
        }
    }

    fmt::text_style Logger::GetLogColor(const LogLevel a_level)
    {
        switch (a_level)
        {
        case LogLevel::VERBOSE :
            return fmt::fg(fmt::color::gray) | fmt::emphasis::bold;
        case LogLevel::INFO :
            return fmt::fg(fmt::color::green) | fmt::emphasis::bold;
        case LogLevel::WARNING :
            return fmt::fg(fmt::color::yellow) | fmt::emphasis::bold;
        case LogLevel::ERROR :
            return fmt::fg(fmt::color::red) | fmt::emphasis::bold;
        case LogLevel::CRITICAL :
            return fmt::fg(fmt::color::purple) | fmt::emphasis::bold;
        default :
            return fmt::fg(fmt::color::white);
        }
    }

    void Logger::FlushLogs()
    {
        while (std::optional<std::string> l_log = m_logQueue.PopLogFromQueue())
            m_logFile << *l_log << "\n";
    }

    void Logger::WriteBufferToFile()
    {
        if (!m_logFile.is_open() || !m_logFile.good())
        {
            m_logFile.close();
            m_logFile.clear();
            m_logFile.open(fmt::format("{}{}{}", m_logFilename, LATEST_LOG_FILE_SUFFIX, LOG_FILE_FORMAT), std::ios::out | std::ios::app);

            if (!m_logFile.is_open())
            {
                std::ofstream l_emergency("emergency.log", std::ios::app);
                if (l_emergency.is_open())
                    for (const std::string& l_log : m_logBuffer)
                        l_emergency << l_log << '\n';

                return;
            }
        }

        for (const std::string& l_log : m_logBuffer)
            m_logFile << l_log << '\n';

        m_logFile.flush();
        m_logBuffer.clear();
        m_bufferSize = 0;

        CheckLogFileSize();
    }
}
