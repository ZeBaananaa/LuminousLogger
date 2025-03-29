#pragma once

namespace Debug
{
    enum class LogLevel : unsigned int
    {
        VERBOSE = 0,
        INFO = 1,
        WARNING = 2,
        ERROR = 3,
        CRITICAL = 4
    };


    enum class LogFlag : unsigned int
    {
        None = 0,
        NoFunc = 1 << 0,
        NoTimestamp = 1 << 1,
        NoSource = 1 << 2,
        NoLevel = 1 << 3
    };


    inline LogFlag operator|(LogFlag a_leftString, LogFlag a_rightString)
    {
        return static_cast<LogFlag>(static_cast<uint8_t>(a_leftString) | static_cast<uint8_t>(a_rightString));
    }

    inline LogFlag operator&(LogFlag a_lString, LogFlag a_rightString)
    {
        return static_cast<LogFlag>(static_cast<uint8_t>(a_lString) & static_cast<uint8_t>(a_rightString));
    }

    inline LogFlag& operator|=(LogFlag& a_lString, LogFlag a_rightString)
    {
        a_lString = a_lString | a_rightString;
        return a_lString;
    }

    inline LogFlag& operator&=(LogFlag& a_lString, LogFlag a_rightString)
    {
        a_lString = a_lString & a_rightString;
        return a_lString;
    }
}
