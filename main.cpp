#include <Logger.hpp>

int main()
{
    Debug::Logger& logger = Debug::Logger::GetInstance();
    logger.SetLogFile("app.log");
    logger.SetLogLevel(Debug::LogLevel::VERBOSE);

    logger.LogDebug("Test Debug Message!");
    logger.LogInfo("Application is running");
    logger.LogWarning("Test Warning Message");
    logger.LogError("Test Error Message");
    logger.LogCritical("Test Critical Message");

    return 0;
}
