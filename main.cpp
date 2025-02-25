#include <Logger.hpp>

int main()
{
    Debug::Logger& m_logger = Debug::Logger::GetInstance();
    m_logger.SetLogFile("app.log");
    m_logger.SetLogLevel(Debug::LogLevel::VERBOSE);

    m_logger.LogDebug("Test Debug Message!");
    m_logger.LogInfo("Application is running");
    m_logger.LogWarning("Test Warning Message");
    m_logger.LogError("Test Error Message");
    m_logger.LogCritical("Test Critical Message");
    m_logger.Log(Debug::LogLevel::VERBOSE, 1);
    m_logger.Log(Debug::LogLevel::VERBOSE, 1.025f);
    double test = 1.2564484861878545;
    m_logger.Log(Debug::LogLevel::VERBOSE, test);
    float t = 0.155654986465465f;
    bool b = false;
    m_logger.Log(Debug::LogLevel::VERBOSE, b);
    m_logger.Log(Debug::LogLevel::VERBOSE, t);

    return 0;
}
