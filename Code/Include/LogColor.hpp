#ifndef LOGCOLOR_HPP
#define LOGCOLOR_HPP

#include <unordered_map>

namespace Debug
{
    enum class ColorEnum : int
    {
        RESET = 0,
        BLACK = 1,
        RED = 2,
        GREEN = 3,
        YELLOW = 4,
        BLUE = 5,
        MAGENTA = 6,
        CYAN = 7,
        WHITE = 8,
        BOLD_BLACK = 9,
        BOLD_RED = 10,
        BOLD_GREEN = 11,
        BOLD_YELLOW = 12,
        BOLD_BLUE = 13,
        BOLD_MAGENTA = 14,
        BOLD_CYAN = 15,
        BOLD_WHITE = 16,
    };
}

#endif //LOGCOLOR_HPP
