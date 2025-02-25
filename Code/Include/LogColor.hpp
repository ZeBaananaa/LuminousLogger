#ifndef LOGCOLOR_HPP
#define LOGCOLOR_HPP

#include <string>
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

    inline std::unordered_map<ColorEnum, std::string> m_colorMap =
    {
        { ColorEnum::RESET, "\033[0m" },
        { ColorEnum::BLACK, "\033[30m" },
        { ColorEnum::RED, "\033[31m" },
        { ColorEnum::GREEN, "\033[32m" },
        { ColorEnum::YELLOW, "\033[33m" },
        { ColorEnum::BLUE, "\033[34m" },
        { ColorEnum::MAGENTA, "\033[35m" },
        { ColorEnum::CYAN, "\033[36m" },
        { ColorEnum::WHITE, "\033[37m" },
        { ColorEnum::BOLD_BLACK, "\033[1m\033[30m" },
        { ColorEnum::BOLD_RED, "\033[1m\033[31m" },
        { ColorEnum::BOLD_GREEN, "\033[1m\033[32m" },
        { ColorEnum::BOLD_YELLOW, "\033[1m\033[33m" },
        { ColorEnum::BOLD_BLUE, "\033[1m\033[34m" },
        { ColorEnum::BOLD_MAGENTA, "\033[1m\033[35m" },
        { ColorEnum::BOLD_CYAN, "\033[1m\033[36m" },
        { ColorEnum::BOLD_WHITE, "\033[1m\033[37m" }
    };
}

#endif //LOGCOLOR_HPP
