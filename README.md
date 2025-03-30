
# LuminousLogger

A simple async logger made for the LuminousEngine project @ ISART Digital Paris in second year of Game Programming.

## Features:
- Singleton-type logger
- Async log writing to file to avoid locking the main thread
- Prints the date, relative log path, line and column of the log
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
    l_logger.Init("app", 1_MiB, 5, true);
    float f = 0.5f;
    DEBUG_LOG_INFO("{} {} {} {} {}, test", 0.1, true, f, 1585485145, -52);

    int health = -10;
    LOG_ASSERT(health >= 0, Debug::AssertLevel::WARN, "1. Health cannot be negative! Value: {}", health);
    int playerID = -1;
    LOG_ASSERT_WARN(playerID >= 0, "1. Player ID is invalid! ID: {}", playerID);

    LOG_ASSERT(health >= 0, Debug::AssertLevel::ERROR, "2. Health cannot be negative! Value: {}", health);
    LOG_ASSERT_ERROR(playerID >= 0, "2. Player ID is invalid! ID: {}", playerID);
}
```


## Screenshots

![Console Output Screenshot](https://i.imgur.com/YP0GNJu.png)

![Log File Screenshot](https://i.imgur.com/ClK0zRq.png)
