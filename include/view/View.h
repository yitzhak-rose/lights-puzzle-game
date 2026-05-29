#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

// Entry point of the entire application.
// Reads the playlist, owns the window, and runs the game loop.
class View {
public:
    View();
    ~View() = default;

    // Reads resources/playlist.txt and starts the game loop.
    void run();

private:
    // ── helpers ──────────────────────────────────
    std::vector<std::string> loadPlaylist() const;
    void updateCloseButton(sf::Vector2u windowSize);

    // ── UI assets ────────────────────────────────
    sf::Font           m_font;
    sf::Text           m_closeText;
    sf::RectangleShape m_closeBox;
    bool               m_fontLoaded = false;
};
