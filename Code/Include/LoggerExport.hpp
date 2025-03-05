#pragma once

#ifdef LUMINOUS_LOGGER_EXPORTS
    #define LUMINOUS_LOGGER_API __declspec(dllexport)
#else
    #define LUMINOUS_LOGGER_API __declspec(dllimport)
#endif
