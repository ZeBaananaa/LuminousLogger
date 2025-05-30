#include <cmath>
#include <iostream>

#include "Logger.hpp"

using Debug::Utils::operator""_MiB;

void LoopTests()
{
    const std::chrono::time_point<std::chrono::steady_clock> l_startTime = std::chrono::steady_clock::now(); // Record start time
    const std::chrono::seconds l_duration = std::chrono::seconds(5); // Set logging duration to x seconds
    int l_messageCount = 0; // Number of messages logged

    while (true)
    {
        auto l_loopStartTime = std::chrono::steady_clock::now(); // Record the time of each iteration
        std::this_thread::sleep_for(std::chrono::milliseconds(125)); // Wait x ms after next print

        // Compute elapsed time
        auto l_elapsedTime = std::chrono::steady_clock::now() - l_loopStartTime;
        const long long l_elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(l_elapsedTime).count();

        std::string l_result = std::string("Message ") + std::to_string(l_messageCount) + " - Time since last print: "
            + std::to_string(l_elapsedMs) + "ms";
        DEBUG_LOG_INFO("{}", l_result);

        ++l_messageCount;

        std::cout << "Message " << l_messageCount
            << " - Time since last print: " << l_elapsedMs << "ms\n";

        // Check if we exceed the duration
        if (std::chrono::steady_clock::now() - l_startTime >= l_duration)
            break; // End loop
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    DEBUG_LOG_INFO("LOG ENDED");
    std::cout << "\nLoop ended after 15s\n";
    std::cout << "Total messages printed: " << l_messageCount;

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
}

int main()
{
    Debug::Logger& l_logger = Debug::Logger::GetInstance();
    l_logger.Init("app", 1_MiB, 5, true);

    /*float f = 0.5f;
    DEBUG_LOG_VERBOSE("{} {} {} {} {}, test", 0.1, true, f, 1585485145, -52);

    bool test = false;
    DEBUG_LOG_INFO("{}", test);
    DEBUG_LOG_WARNING("Warning message : {}", 5.025f);
    DEBUG_LOG_ERROR("Error message");
    DEBUG_LOG_CRITICAL("Critical message");

    long long t = -10456555854645;
    DEBUG_LOG(Debug::LogLevel::VERBOSE, "VERBOSE MESSAGE : {}", t);

    int health = -10;
    LOG_ASSERT(health >= 0, Debug::AssertLevel::WARN, "1. Health cannot be negative! Value: {}", health);
    int playerID = -1;
    LOG_ASSERT_WARN(playerID >= 0, "1. Player ID is invalid! ID: {}", playerID);

    LOG_ASSERT(health >= 0, Debug::AssertLevel::ERROR, "2. Health cannot be negative! Value: {}", health);
    LOG_ASSERT_ERROR(playerID >= 0, "2. Player ID is invalid! ID: {}", playerID);*/

    LoopTests();
    return 0;
}
