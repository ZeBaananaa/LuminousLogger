#include "Logger.hpp"

#include "LogColor.hpp"

#include <filesystem>
#include <iomanip>
#include <sstream>

constexpr std::string PROJECT_FOLDER{"LuminousLogger/"};
constexpr std::string LATEST_LOG_FILE_NAME{"-latest"};
constexpr std::string LOG_FILE_FORMAT{".log"};
constexpr std::string OLD_FILE_TEXT{".old"};

namespace Debug
{
    Logger& Logger::GetInstance()
    {
        static Logger s_instance{};
        return s_instance;
    }

    Logger::~Logger()
    {
        if (m_logFile.is_open())
            m_logFile.close();
    }

    void Logger::SetLogLevel(const LogLevel& a_level)
    {
        std::lock_guard l_lock(m_logMutex);
        m_minLogLevel = a_level;
    }

    void Logger::SetLogFile(const std::string& a_filename, const size_t& a_maxFileSize, const size_t& a_maxFiles)
    {
        std::lock_guard l_lock(m_logMutex);
        m_logFilename = a_filename;
        m_maxFileSize = a_maxFileSize;
        m_maxFiles = a_maxFiles;

        m_logFile.open(a_filename + LATEST_LOG_FILE_NAME + LOG_FILE_FORMAT, std::ios::app);

        if (!m_logFile.is_open())
            LogError("File " + m_logFilename + LATEST_LOG_FILE_NAME + LOG_FILE_FORMAT + " could not be opened!" + "\n");
    }

    void Logger::RotateLogs()
    {
        m_logFile.close();
        for (size_t i{m_maxFiles - 1}; i > 0; --i)
        {
            std::string l_oldFile{m_logFilename + "-" + std::to_string(i) + OLD_FILE_TEXT += LOG_FILE_FORMAT};
            std::string l_newFile{m_logFilename + "-" + std::to_string(i + 1) + OLD_FILE_TEXT += LOG_FILE_FORMAT};
            if (std::filesystem::exists(l_oldFile))
                std::filesystem::rename(l_oldFile, l_newFile);
        }
        std::filesystem::rename(m_logFilename + LATEST_LOG_FILE_NAME + LOG_FILE_FORMAT,
                                m_logFilename + "-1" + OLD_FILE_TEXT + LOG_FILE_FORMAT);
        m_logFile.open(m_logFilename + LATEST_LOG_FILE_NAME + LOG_FILE_FORMAT, std::ios::trunc);
    }

    void Logger::CheckLogFileSize()
    {
        if (std::filesystem::exists(m_logFilename + LATEST_LOG_FILE_NAME + LOG_FILE_FORMAT) &&
            std::filesystem::file_size(m_logFilename + LATEST_LOG_FILE_NAME + LOG_FILE_FORMAT) >= m_maxFileSize)
            RotateLogs();
    }

    std::string Logger::GetSourceLocation(const std::source_location& a_location = std::source_location::current())
    {
        constexpr std::string_view l_folder{PROJECT_FOLDER};
        std::string_view l_filePath{a_location.file_name()};

        if (const size_t l_pos{l_filePath.find(l_folder)}; l_pos != std::string_view::npos)
            l_filePath.remove_prefix(l_pos + l_folder.size());

        return std::format("{} ({}:{})", l_filePath, a_location.line(), a_location.column());
    }

    std::string Logger::FormatConsoleMessage(const LogLevel& a_level, const std::string& a_message,
                                             const std::source_location& a_location = std::source_location::current())
    {
        const std::chrono::time_point<std::chrono::system_clock> l_now{std::chrono::system_clock::now()};
        const time_t l_time{std::chrono::system_clock::to_time_t(l_now)};

        std::ostringstream l_timeStr{};
        l_timeStr << std::put_time(std::localtime(&l_time), "%Y-%m-%d %H:%M:%S");

        return std::format("{} [ {} ] - [ {} ] : {}{}{}", ColoredLogLevelToString(a_level), l_timeStr.str(),
                           GetSourceLocation(a_location), ColorMap.at(ColorEnum::BOLD_BLACK), a_message,
                           ColorMap.at(ColorEnum::RESET));
    }

    std::string Logger::FormatLogFileMessage(const LogLevel& a_level, const std::string& a_message,
                                             const std::source_location& a_location = std::source_location::current())
    {
        const std::chrono::time_point l_now{std::chrono::system_clock::now()};
        const time_t l_time{std::chrono::system_clock::to_time_t(l_now)};

        std::ostringstream l_timeStr{};
        l_timeStr << std::put_time(std::localtime(&l_time), "%Y-%m-%d %H:%M:%S");

        return std::format("{} [ {} ] - [ {} ] : {}", LogLevelToString(a_level), l_timeStr.str(),
                           GetSourceLocation(a_location), a_message);
    }

    std::string Logger::LogLevelToString(const LogLevel& a_level)
    {
        switch (a_level)
        {
        case LogLevel::VERBOSE:
            return std::format("VERBOSE    :");

        case LogLevel::INFO:
            return std::format("INFO       :");

        case LogLevel::WARNING:
            return std::format("WARNING    :");

        case LogLevel::ERROR:
            return std::format("ERROR      :");

        case LogLevel::CRITICAL:
            return std::format("CRITICAL   :");

        default:
            return std::format("UNSPECIFIED    :");
        }
    }

    std::string Logger::ColoredLogLevelToString(const LogLevel& a_level)
    {
        switch (a_level)
        {
        case LogLevel::VERBOSE:
            return std::format("{}VERBOSE{}    :", ColorMap.at(ColorEnum::BOLD_BLUE), ResetColor);

        case LogLevel::INFO:
            return std::format("{}INFO{}       :", ColorMap.at(ColorEnum::BOLD_GREEN), ResetColor);

        case LogLevel::WARNING:
            return std::format("{}WARNING{}    :", ColorMap.at(ColorEnum::BOLD_YELLOW), ResetColor);

        case LogLevel::ERROR:
            return std::format("{}ERROR{}      :", ColorMap.at(ColorEnum::BOLD_RED), ResetColor);

        case LogLevel::CRITICAL:
            return std::format("{}CRITICAL{}   :", ColorMap.at(ColorEnum::BOLD_MAGENTA), ResetColor);

        default:
            return std::format("{}UNSPECIFIED{}    :", ColorMap.at(ColorEnum::BOLD_WHITE), ResetColor);
        }
    }
}
