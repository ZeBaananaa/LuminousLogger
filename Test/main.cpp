#include "Logger.hpp"

using Debug::Utils::operator""_MiB;

int main()
{
    Debug::Logger& l_logger = Debug::Logger::GetInstance();

    // Define the log file name, the size (in MiB), the max file count and toggle color support
    l_logger.Init("app", 1_MiB, 5, true);

    DEBUG_LOG(Debug::LogLevel::CRITICAL, "test");
    DEBUG_LOG_CRITICAL("test2");

    l_logger.LogVerbose("Test Verbose Message!");
    l_logger.LogInfo("Application is running");
    l_logger.LogWarning("Test Warning Message");
    l_logger.LogError("Test Error Message");
    l_logger.LogCritical("Test Critical Message");
    l_logger.Log(Debug::LogLevel::VERBOSE, 1);
    l_logger.Log(Debug::LogLevel::VERBOSE, 1.025f);

    double test{1.2564484861878545};
    l_logger.Log(Debug::LogLevel::VERBOSE, test);

    float t{0.155654986465465f};
    l_logger.Log(Debug::LogLevel::VERBOSE, t);

    bool b{false};
    l_logger.Log(Debug::LogLevel::VERBOSE, b);

    // TODO : Remove in order to allow program to close properly
    getchar();
    return 0;
}
