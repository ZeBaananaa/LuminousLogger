
# LuminousLogger

A simple async logger made for the LuminousEngine project @ ISART Digital Paris in second year of Game Programming.

## Features:
- Singleton-type logger
- Async log writing to file to avoid locking the main thread
- Prints the date, relative log path, line and column of the log
- Ability to manually flush the loggingQueue when parsing critical logs
- Ability to chose the log file name
- Supports file rotation based on a customizable max file size
- Supports different levels of logging
- Ability to customize the maximum amount of logging files
- Lightweight


## Usage/Examples

```cpp
#include "Logger.hpp"

void PrintLogs()
{
    Debug::Logger& l_logger = Debug::Logger::GetInstance();

    // Define the log file name, the size (in MiB) and the max file count
    l_logger.SetLogFile("app", 1_MiB, 5);

    l_logger.LogVerbose("Test Verbose Message!");
    l_logger.LogInfo("Application is running");
    l_logger.LogWarning("Test Warning Message");
    l_logger.LogError("Test Error Message");
    l_logger.LogCritical("Test Critical Message")
    ;
    l_logger.Log(Debug::LogLevel::VERBOSE, 1);
    l_logger.Log(Debug::LogLevel::VERBOSE, 1.025f);

    double test{1.2564484861878545};
    l_logger.Log(Debug::LogLevel::VERBOSE, test);

    float t{0.155654986465465f};
    l_logger.Log(Debug::LogLevel::VERBOSE, t);

    bool b{false};
    l_logger.Log(Debug::LogLevel::VERBOSE, b);
}
```


## Screenshots

![Console Output Screenshot](https://i.imgur.com/YP0GNJu.png)

![Log File Screenshot](https://i.imgur.com/ClK0zRq.png)