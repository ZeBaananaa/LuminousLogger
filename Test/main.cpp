#include <cmath>
#include <iostream>

#include "Logger.hpp"

using Debug::Utils::operator""_MiB;

void LoopTests()
{
    Debug::Logger& l_logger = Debug::Logger::GetInstance();

    // Define the log file name, the size (in MiB), the max file count and toggle color support
    l_logger.Init("app", 1_MiB, 5, true);

    std::chrono::time_point<std::chrono::steady_clock> l_startTime = std::chrono::steady_clock::now();
    // Record start time
    const std::chrono::seconds l_duration = std::chrono::seconds(15); // Set logging duration to x seconds
    int l_messageCount = 0; // Amount of messages logged

    while (true)
    {
        auto l_loopStartTime = std::chrono::steady_clock::now(); // Record the time of each iteration
        std::this_thread::sleep_for(std::chrono::milliseconds(0)); // Wait x ms after next print

        // Compute elapsed time
        auto l_elapsedTime = std::chrono::steady_clock::now() - l_loopStartTime;
        const long long l_elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(l_elapsedTime).count();

        std::string l_result = std::string("Message ") + std::to_string(l_messageCount) + " - Time since last print: "
                               + std::to_string(l_elapsedMs) + "ms";
        DEBUG_LOG_INFO("{}", l_result);

        ++l_messageCount;

        std::cout << "Message " << l_messageCount
                << " - Time since last print: " << l_elapsedMs << "ms\n";

        // Check if we exceed duration
        if (std::chrono::steady_clock::now() - l_startTime >= l_duration)
            break; // End loop
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    std::cout << "\nLoop ended after 15s\n";
    std::cout << "Total messages printed: " << l_messageCount;
}

int main()
{
    LoopTests();
    getchar();
    return 0;
}
