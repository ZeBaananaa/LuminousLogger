
# LuminousLogger

A simple async logger made for the LuminousEngine project @ ISART Digital Paris in second year of Game Programming.

## Features:
- Singleton-type logger
- Separated thread assigned to the logger to perform async file writing
- Uses the lock-free FIFO concept
- Prints the date, relative log path, line and column of the log
- Ability to chose the log file name
- Supports file rotation based on a customizable max file size
- Supports different levels of logging
- Ability to customize the maximum amount of logging files
- Assertion support
- Color support (this can be disabled)
- Support for both macros and logging functions
- Lightweight


## Usage/Examples

```cpp
#include "Logger.hpp"

int main()
{
    Debug::Logger& l_logger = Debug::Logger::GetInstance();
    l_logger.Init("app", 1_MiB, 5, true);

    float f = 0.5f;
    DEBUG_LOG_VERBOSE("{} {} {} {} {}, test", 0.1, true, f, 1585485145, -52);

    bool test = false;
    DEBUG_LOG_INFO("{}", test);
    DEBUG_LOG_WARNING("Warning message : {}", 5.025f);
    DEBUG_LOG_ERROR("Error message");
    DEBUG_LOG_CRITICAL("Crtical message");

    long long t = -10456555854645;
    DEBUG_LOG(Debug::LogLevel::VERBOSE, "VERBOSE MESSAGE : {}", t);

    int health = -10;
    LOG_ASSERT(health >= 0, Debug::AssertLevel::WARN, "1. Health cannot be negative! Value: {}", health);
    int playerID = -1;
    LOG_ASSERT_WARN(playerID >= 0, "1. Player ID is invalid! ID: {}", playerID);

    LOG_ASSERT(health >= 0, Debug::AssertLevel::ERROR, "2. Health cannot be negative! Value: {}", health);
    LOG_ASSERT_ERROR(playerID >= 0, "2. Player ID is invalid! ID: {}", playerID);
    return 0;
}
```


## Screenshots

![Console Output Screenshot](https://i.imgur.com/98pWgKC.png)


### Console Output

```log
VERBOSE     [2025-03-30 20:23:29] - [main.cpp (55:76) - int main()] : 0.1 true 0.5 1585485145 -52, test
INFO        [2025-03-30 20:23:29] - [main.cpp (58:30) - int main()] : false
WARNING     [2025-03-30 20:23:29] - [main.cpp (59:53) - int main()] : Warning message : 5.025
ERROR       [2025-03-30 20:23:29] - [main.cpp (60:36) - int main()] : Error message
CRITICAL    [2025-03-30 20:23:29] - [main.cpp (61:41) - int main()] : Crtical message
VERBOSE     [2025-03-30 20:23:29] - [main.cpp (64:66) - int main()] : VERBOSE MESSAGE : -10456555854645
WARNING     [2025-03-30 20:23:29] - [main.cpp (67:104) - int main()] : 1. Health cannot be negative! Value: -10
WARNING     [2025-03-30 20:23:29] - [main.cpp (69:79) - int main()] : 1. Player ID is invalid! ID: -1
ERROR       [2025-03-30 20:23:29] - [main.cpp (71:105) - int main()] : 2. Health cannot be negative! Value: -10
```
