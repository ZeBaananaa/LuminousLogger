#include <cmath>

#include "Logger.hpp"

using Debug::Utils::operator""_MiB;

int main()
{
    Debug::Logger& l_logger = Debug::Logger::GetInstance();

    // Define the log file name, the size (in MiB), the max file count and toggle color support
    l_logger.Init("app", 1_MiB, 5, true);

    bool b0 = true;

    DEBUG_LOG(Debug::LogLevel::CRITICAL, "Test, {} {} {}", b0, 50.f, 0.248558654);
    //DEBUG_LOG_CRITICAL("test2");

    //l_logger.LogVerbose("Test Verbose Message!");
    //l_logger.LogInfo("Application is running");
    //l_logger.LogWarning("Test Warning Message");
    //l_logger.LogError("Test Error Message");
    //l_logger.LogCritical("Test Critical Message");
    //l_logger.Log(Debug::LogLevel::VERBOSE, 1);
    //l_logger.Log(Debug::LogLevel::VERBOSE, 1.025f);

    /*double test{1.2564484861878545};
    l_logger.Log(Debug::LogLevel::VERBOSE, test);

    float t{0.155654986465465f};
    l_logger.Log(Debug::LogLevel::VERBOSE, t);

    bool b{false};
    l_logger.Log(Debug::LogLevel::VERBOSE, b);*/


    //DEBUG_LOG_INFO("test", b, t, test);

    // TODO : Remove in order to allow program to close properly
    getchar();
    return 0;
}

void LoopTests()
{
    Debug::Logger& l_logger = Debug::Logger::GetInstance();

    // Define the log file name, the size (in MiB), the max file count and toggle color support
    l_logger.Init("app", 1_MiB, 5, true);

    auto start_time = std::chrono::steady_clock::now(); // Enregistrer l'heure de départ
    auto duration = std::chrono::seconds(15); // Durée de 1 minute
    int message_count = 0; // Compteur des messages imprimés

    while (true)
    {
        auto loop_start_time = std::chrono::steady_clock::now(); // Enregistrer l'heure de chaque itération
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Pause de 500ms

        // Calcul du temps écoulé depuis la dernière itération
        auto elapsed_time = std::chrono::steady_clock::now() - loop_start_time;
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed_time).count();

        std::string result = std::string("Message ") + std::to_string(message_count) + " - Temps depuis dernier print: "
            + std::to_string(elapsed_ms) + "ms";
        ++message_count;
        //DEBUG_LOG(result);

        std::cout << "Message " << ++message_count
            << " - Temps depuis dernier print: " << elapsed_ms << "ms" << std::endl;

        // Vérifier si une minute s'est écoulée
        if (std::chrono::steady_clock::now() - start_time >= duration)
        {
            break; // Sortir de la boucle après 1 minute
        }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    /*std::cout << "\nLoop ended after 15s\n";
    std::cout << "Total messages printed: " << message_count;*/

    //DEBUG_LOG("\n\n\nLoop ended after 15s\n");
    //DEBUG_LOG("Total messages printed: " + 5 + 10 + 50);
}
