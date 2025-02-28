#include "Logger.hpp"

int main()
{
    Debug::Logger& l_logger = Debug::Logger::GetInstance();
    l_logger.SetLogFile("app.log");
    l_logger.SetLogLevel(Debug::LogLevel::VERBOSE);

    l_logger.LogDebug("Test Debug Message!");
    l_logger.LogInfo("Application is running");
    l_logger.LogWarning("Test Warning Message");
    l_logger.LogError("Test Error Message");
    l_logger.LogCritical("Test Critical Message");
    l_logger.Log(Debug::LogLevel::VERBOSE, 1);
    l_logger.Log(Debug::LogLevel::VERBOSE, 1.025f);
    double test = 1.2564484861878545;
    l_logger.Log(Debug::LogLevel::VERBOSE, test);
    float t = 0.155654986465465f;
    bool b = false;
    l_logger.Log(Debug::LogLevel::VERBOSE, b);
    l_logger.Log(Debug::LogLevel::VERBOSE, t);
    l_logger.LogCritical("Test Critical Message");

    // TODO : Remove in order to allow program to close properly
    getchar();
    return 0;
}
