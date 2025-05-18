#pragma once


#if defined(_WIN32)
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX
    #define NOAPISET
    #define NOCRYPT
    #define NOFORMATMESSAGE

    #include <windows.h>
    #undef FormatMessage
    #undef ERROR

    inline int GetProcessID() { return static_cast<int>(::GetCurrentProcessId()); }
#else
#include <unistd.h>

inline int GetProcessID() { return static_cast<int>(::getpid()); }
#endif