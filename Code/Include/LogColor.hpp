#pragma once

#include <string>
#include <unordered_map>

namespace Debug
{
    enum class ColorEnum : int
    {
        RESET = 0,
        BLACK,
        RED ,
        GREEN,
        YELLOW,
        BLUE,
        MAGENTA,
        CYAN,
        WHITE,
        GRAY,
        BOLD_BLACK,
        BOLD_RED,
        BOLD_GREEN,
        BOLD_YELLOW,
        BOLD_BLUE,
        BOLD_MAGENTA,
        BOLD_CYAN,
        BOLD_WHITE,
    };

    const std::unordered_map<ColorEnum, std::string> ColorMap =
    {
        {ColorEnum::RESET, "\033[0m"},
        {ColorEnum::BLACK, "\033[30m"},
        {ColorEnum::RED, "\033[31m"},
        {ColorEnum::GREEN, "\033[32m"},
        {ColorEnum::YELLOW, "\033[33m"},
        {ColorEnum::BLUE, "\033[34m"},
        {ColorEnum::MAGENTA, "\033[35m"},
        {ColorEnum::CYAN, "\033[36m"},
        {ColorEnum::WHITE, "\033[0m"},
        {ColorEnum::GRAY, "\033[37m"},
        {ColorEnum::BOLD_BLACK, "\033[1m\033[90m"},
        {ColorEnum::BOLD_RED, "\033[1m\033[91m"},
        {ColorEnum::BOLD_GREEN, "\033[1m\033[92m"},
        {ColorEnum::BOLD_YELLOW, "\033[1m\033[93m"},
        {ColorEnum::BOLD_BLUE, "\033[1m\033[94m"},
        {ColorEnum::BOLD_MAGENTA, "\033[1m\033[95m"},
        {ColorEnum::BOLD_CYAN, "\033[1m\033[96m"},
        {ColorEnum::BOLD_WHITE, "\033[1m\033[97m"}
    };

    const std::string ResetColor(ColorMap.at(ColorEnum::RESET));
}