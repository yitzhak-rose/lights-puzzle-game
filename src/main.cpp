#include "view/View.h"
#include <fstream>
#include <iostream>
#include <exception>

int main() {
    try {
        View view;
        view.run();
    }
    catch (const std::exception& e) {
        std::ofstream log("log.txt", std::ios::app);
        if (log) log << "CRITICAL ERROR: " << e.what() << "\n";
        std::cerr << "Game crashed! Check log.txt for details.\n";
        return EXIT_FAILURE;
    }
    catch (...) {
        std::ofstream log("log.txt", std::ios::app);
        if (log) log << "CRITICAL ERROR: Unknown exception.\n";
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
