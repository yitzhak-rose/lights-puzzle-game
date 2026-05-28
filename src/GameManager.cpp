#include "GameManager.h"
#include "view/View.h"
#include "Board.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>

GameManager::GameManager() {}

GameManager::~GameManager() {}

void GameManager::startGame() {
    std::vector<std::string> levels;
    
    // Read the list of levels from the playlist.
    std::ifstream playlistFile("../resources/playlist.txt");
    if (playlistFile.is_open()) {
        std::string line;
        while (std::getline(playlistFile, line)) {
            // Trim carriage returns and spaces from the line
            while (!line.empty() && (line.back() == '\r' || line.back() == '\n' || line.back() == ' ')) {
                line.pop_back();
            }
            // Also trim leading spaces
            size_t startPos = line.find_first_not_of(" \t");
            if (startPos != std::string::npos) {
                line = line.substr(startPos);
            }
            
            if (!line.empty() && line[0] != '#') {
                levels.push_back(line);
            }
        }
        playlistFile.close();
    } else {
        std::cerr << "Warning: Could not open ../resources/playlist.txt, falling back to level1.txt" << std::endl;
        levels.push_back("level1.txt");
    }

    if (levels.empty()) {
        std::cerr << "Warning: Playlist is empty, falling back to level1.txt" << std::endl;
        levels.push_back("level1.txt");
    }

    View view;
    view.run(levels);
}
