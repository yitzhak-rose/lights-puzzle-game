#include "../include/view/View.h"
#include "view/BoardRenderer.h"
#include "logic/Board.h"
#include "constants.h"
#include <fstream>
#include <iostream>
#include <optional>

// ──────────────────────────────────────────────────────────────
//  Constructor
// ──────────────────────────────────────────────────────────────

View::View()
    : m_font()
    , m_closeText(m_font)
    , m_closeBox() {
    m_fontLoaded = m_font.openFromFile("resources/Roboto-Medium.ttf");
    m_closeText.setString("X");
    m_closeText.setFillColor(sf::Color::White);
    m_closeText.setStyle(sf::Text::Bold);
}

// ──────────────────────────────────────────────────────────────
//  Playlist
// ──────────────────────────────────────────────────────────────

std::vector<std::string> View::loadPlaylist() const {
    std::vector<std::string> levels;
    std::ifstream file("resources/playlist.txt");

    if (!file.is_open()) {
        std::cerr << "Warning: cannot open playlist.txt, falling back to level1.txt\n";
        return {"level1.txt"};
    }

    std::string line;
    while (std::getline(file, line)) {
        // Trim trailing whitespace / CR
        while (!line.empty() && (line.back() == '\r' || line.back() == '\n' || line.back() == ' '))
            line.pop_back();
        // Trim leading whitespace
        const size_t start = line.find_first_not_of(" \t");
        if (start != std::string::npos)
            line = line.substr(start);

        if (!line.empty() && line[0] != '#')
            levels.push_back(line);
    }

    if (levels.empty()) {
        std::cerr << "Warning: playlist.txt is empty, falling back to level1.txt\n";
        return {"level1.txt"};
    }
    return levels;
}

// ──────────────────────────────────────────────────────────────
//  Main loop
// ──────────────────────────────────────────────────────────────

void View::run() {
    const std::vector<std::string> levels = loadPlaylist();

    sf::RenderWindow window(sf::VideoMode({800u, 600u}), "Hexagonal Loop");

    size_t        currentLevel   = 0;
    Board         board;
    BoardRenderer renderer;
    bool          levelCompleted = false;

    // Load level by index; returns false when out of bounds.
    auto loadLevel = [&](size_t index) -> bool {
        if (index >= levels.size()) return false;
        board.loadFromFile("resources/" + levels[index]);
        renderer.init(window.getSize(), board);
        window.setTitle("Hexagonal Loop - Level " + std::to_string(index + 1)
                        + " of " + std::to_string(levels.size()));
        return true;
    };

    // Advance to the next level (shared by all inputs).
    auto advanceLevel = [&]() {
        if (++currentLevel < levels.size()) {
            loadLevel(currentLevel);
            levelCompleted = false;
        } else {
            std::cout << "Congratulations! All levels completed!\n";
            window.close();
        }
    };

    if (!loadLevel(currentLevel)) return;

    // ── Event loop ───────────────────────────────
    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {

            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                updateCloseButton(window.getSize());
                const sf::Vector2f pos(static_cast<float>(mouse->position.x),
                                       static_cast<float>(mouse->position.y));

                if (m_closeBox.getGlobalBounds().contains(pos)) {
                    window.close();
                } else if (levelCompleted) {
                    advanceLevel();
                } else {
                    const RotationDir dir = (mouse->button == sf::Mouse::Button::Left)
                                           ? RotationDir::Clockwise
                                           : RotationDir::CounterClockwise;
                    const int idx = renderer.getNodeAt(pos);
                    if (idx != -1) {
                        board.rotateNode(idx, dir);
                        renderer.sync(board);
                    }
                }
            }

            if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (levelCompleted &&
                    (key->code == sf::Keyboard::Key::Space ||
                     key->code == sf::Keyboard::Key::Enter)) {
                    advanceLevel();
                }
            }
        }

        if (!levelCompleted && board.isLevelComplete())
            levelCompleted = true;

        // ── Render ──────────────────────────────
        updateCloseButton(window.getSize());
        window.clear(sf::Color(18, 18, 24));
        window.draw(renderer);
        window.draw(m_closeBox);
        if (m_fontLoaded) window.draw(m_closeText);

        if (levelCompleted) {
            // Dim overlay
            sf::RectangleShape overlay(sf::Vector2f(
                static_cast<float>(window.getSize().x),
                static_cast<float>(window.getSize().y)));
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(overlay);

            // Panel
            constexpr float PW = 400.0f, PH = 200.0f;
            sf::RectangleShape panel(sf::Vector2f(PW, PH));
            panel.setFillColor(sf::Color(28, 28, 36));
            panel.setOutlineThickness(3.0f);
            panel.setOutlineColor(sf::Color(255, 215, 0));
            panel.setOrigin(sf::Vector2f(PW / 2.0f, PH / 2.0f));
            panel.setPosition(sf::Vector2f(
                static_cast<float>(window.getSize().x) / 2.0f,
                static_cast<float>(window.getSize().y) / 2.0f));
            window.draw(panel);

            if (m_fontLoaded) {
                const sf::Vector2f wc(
                    static_cast<float>(window.getSize().x) / 2.0f,
                    static_cast<float>(window.getSize().y) / 2.0f);

                auto centredText = [&](const std::string& str,
                                       unsigned int size,
                                       sf::Color color,
                                       float yOffset) {
                    sf::Text t(m_font);
                    t.setString(str);
                    t.setCharacterSize(size);
                    t.setFillColor(color);
                    const sf::FloatRect b = t.getLocalBounds();
                    t.setOrigin(sf::Vector2f(b.position.x + b.size.x / 2.0f,
                                             b.position.y + b.size.y / 2.0f));
                    t.setPosition(wc + sf::Vector2f(0.0f, yOffset));
                    window.draw(t);
                };

                centredText("LEVEL FINISHED!", 28, sf::Color::White, -30.0f);
                centredText("Press Space or Click to continue", 16,
                            sf::Color(180, 180, 200), 30.0f);
            }
        }

        window.display();
    }
}

// ──────────────────────────────────────────────────────────────
//  Private helpers
// ──────────────────────────────────────────────────────────────

void View::updateCloseButton(sf::Vector2u windowSize) {
    constexpr float size   = 24.0f;
    constexpr float margin = 8.0f;
    const sf::Vector2f pos(
        static_cast<float>(windowSize.x) - margin - size, margin);

    m_closeBox.setSize(sf::Vector2f(size, size));
    m_closeBox.setPosition(pos);
    m_closeBox.setFillColor(sf::Color::Red);

    m_closeText.setCharacterSize(18);
    const sf::FloatRect tb = m_closeText.getLocalBounds();
    m_closeText.setOrigin(sf::Vector2f(
        tb.position.x + tb.size.x / 2.0f,
        tb.position.y + tb.size.y / 2.0f));
    m_closeText.setPosition(sf::Vector2f(pos.x + size / 2.0f, pos.y + size / 2.0f));
}
