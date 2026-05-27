#include <iostream>
#include <fstream>
#include <exception>
// #include "GameManager.h"

int main() {
    try {
        // GameManager game;
        // game.run();

    }
    catch (const std::exception& e) {
        std::ofstream logFile("log.txt", std::ios::app);
        if (logFile.is_open()) {
            logFile << "CRITICAL ERROR: " << e.what() << "\n";
            logFile.close();
        }
        std::cerr << "Game crashed! Check log.txt for details.\n";
        return EXIT_FAILURE;
    }
    catch (...) {
        std::ofstream logFile("log.txt", std::ios::app);
        if (logFile.is_open()) {
            logFile << "CRITICAL ERROR: Unknown exception occurred.\n";
            logFile.close();
        }
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}