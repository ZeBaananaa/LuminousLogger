#ifndef LOGLEVEL_HPP
#define LOGLEVEL_HPP

namespace Debug
{
    enum class LogLevel : int
    {
        VERBOSE = 0,
        INFO = 1,
        WARNING = 2,
        ERROR = 3,
        CRITICAL = 4
    };
}

#endif //LOGLEVEL_HPP