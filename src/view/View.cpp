// #include ".../include/view/View.h"

#include "../include/view/View.h"
#include <optional>
#include "view/Graph.h"
#include "Board.h"
#include "constants.h"
#include <iostream>


View::View()
    : uiFont()
    , closeText(uiFont)
    , closeBox()
    , fontLoaded(false) {
    fontLoaded = uiFont.openFromFile("resources/Roboto-Medium.ttf");
    closeText.setString("X");
    closeText.setFillColor(sf::Color::White);
    closeText.setStyle(sf::Text::Bold);
}

View::~View() {}

void View::run(const std::vector<std::string>& levels) {
    const unsigned int windowWidth  = 800;
    const unsigned int windowHeight = 600;

    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "Hexagonal Loop");

    size_t currentLevelIndex = 0;
    Board board;
    GraphView graph;

    // Helper to load a level
    auto loadLevel = [&](size_t index) {
        if (index >= levels.size()) return false;
        std::string levelPath = "resources/" + levels[index];
        board.loadFromFile(levelPath);
        graph.createPoints(window.getSize(), board);
        window.setTitle("Hexagonal Loop - Level " + std::to_string(index + 1) + " of " + std::to_string(levels.size()));
        return true;
    };

    // Load initial level
    if (!loadLevel(currentLevelIndex)) {
        return;
    }

    bool levelCompleted = false;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                updateCloseButton(window.getSize());

                const sf::Vector2f mousePos(
                    static_cast<float>(mouseEvent->position.x),
                    static_cast<float>(mouseEvent->position.y)
                );

                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    if (closeBox.getGlobalBounds().contains(mousePos)) {
                        window.close();
                    } else if (!levelCompleted) {
                        int clickedIndex = graph.getPointIndexAt(mousePos);
                        if (clickedIndex != -1) {
                            board.rotateNode(clickedIndex, RotationDir::Clockwise);
                            graph.updatePointsFromBoard(board);
                        }
                    } else {
                        // Dialogue is shown, click to proceed
                        currentLevelIndex++;
                        if (currentLevelIndex < levels.size()) {
                            loadLevel(currentLevelIndex);
                            levelCompleted = false;
                        } else {
                            std::cout << "Congratulations! All levels completed successfully!\n";
                            window.close();
                        }
                    }
                }

                if (mouseEvent->button == sf::Mouse::Button::Right) {
                    if (!levelCompleted) {
                        int clickedIndex = graph.getPointIndexAt(mousePos);
                        if (clickedIndex != -1) {
                            board.rotateNode(clickedIndex, RotationDir::CounterClockwise);
                            graph.updatePointsFromBoard(board);
                        }
                    } else {
                        // Dialogue is shown, click to proceed
                        currentLevelIndex++;
                        if (currentLevelIndex < levels.size()) {
                            loadLevel(currentLevelIndex);
                            levelCompleted = false;
                        } else {
                            std::cout << "Congratulations! All levels completed successfully!\n";
                            window.close();
                        }
                    }
                }
            }

            if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
                if (levelCompleted) {
                    if (keyEvent->code == sf::Keyboard::Key::Space || keyEvent->code == sf::Keyboard::Key::Enter) {
                        currentLevelIndex++;
                        if (currentLevelIndex < levels.size()) {
                            loadLevel(currentLevelIndex);
                            levelCompleted = false;
                        } else {
                            std::cout << "Congratulations! All levels completed successfully!\n";
                            window.close();
                        }
                    }
                }
            }
        }

        // Check if level is completed
        if (!levelCompleted && board.isLevelComplete()) {
            levelCompleted = true;
        }

        updateCloseButton(window.getSize());

        window.clear(sf::Color(18, 18, 24));
        window.draw(graph);
        window.draw(closeBox);
        if (fontLoaded) {
            window.draw(closeText);
        }

        // Draw dialogue overlay and panel if level is finished
        if (levelCompleted) {
            sf::RectangleShape overlay(sf::Vector2f(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y)));
            overlay.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(overlay);

            float panelWidth = 400.0f;
            float panelHeight = 200.0f;
            sf::RectangleShape panel(sf::Vector2f(panelWidth, panelHeight));
            panel.setFillColor(sf::Color(28, 28, 36));
            panel.setOutlineThickness(3.0f);
            panel.setOutlineColor(sf::Color(255, 215, 0)); // Gold accent
            panel.setOrigin(sf::Vector2f(panelWidth / 2.0f, panelHeight / 2.0f));
            panel.setPosition(sf::Vector2f(static_cast<float>(window.getSize().x) / 2.0f, static_cast<float>(window.getSize().y) / 2.0f));
            window.draw(panel);

            if (fontLoaded) {
                sf::Text titleText(uiFont);
                titleText.setString("LEVEL FINISHED!");
                titleText.setCharacterSize(28);
                titleText.setFillColor(sf::Color::White);
                titleText.setStyle(sf::Text::Bold);
                const sf::FloatRect titleBounds = titleText.getLocalBounds();
                titleText.setOrigin(sf::Vector2f(
                    titleBounds.position.x + titleBounds.size.x / 2.0f,
                    titleBounds.position.y + titleBounds.size.y / 2.0f
                ));
                titleText.setPosition(sf::Vector2f(
                    static_cast<float>(window.getSize().x) / 2.0f,
                    static_cast<float>(window.getSize().y) / 2.0f - 30.0f
                ));
                window.draw(titleText);

                sf::Text subText(uiFont);
                subText.setString("Press Space or Click to continue");
                subText.setCharacterSize(16);
                subText.setFillColor(sf::Color(180, 180, 200));
                const sf::FloatRect subBounds = subText.getLocalBounds();
                subText.setOrigin(sf::Vector2f(
                    subBounds.position.x + subBounds.size.x / 2.0f,
                    subBounds.position.y + subBounds.size.y / 2.0f
                ));
                subText.setPosition(sf::Vector2f(
                    static_cast<float>(window.getSize().x) / 2.0f,
                    static_cast<float>(window.getSize().y) / 2.0f + 30.0f
                ));
                window.draw(subText);
            }
        }

        window.display();
    }
}

void View::updateCloseButton(sf::Vector2u windowSize) {
    const float buttonSize   = 24.0f;
    const float buttonMargin = 8.0f;

    sf::Vector2f buttonPos(
        static_cast<float>(windowSize.x) - buttonMargin - buttonSize,
        buttonMargin
    );

    closeBox.setSize(sf::Vector2f(buttonSize, buttonSize));
    closeBox.setPosition(buttonPos);
    closeBox.setFillColor(sf::Color::Red);

    closeText.setCharacterSize(18); // tweak as needed
    const sf::FloatRect textBounds = closeText.getLocalBounds();
    closeText.setOrigin(sf::Vector2f(
        textBounds.position.x + textBounds.size.x / 2.0f,
        textBounds.position.y + textBounds.size.y / 2.0f
    ));
    closeText.setPosition(sf::Vector2f(
        buttonPos.x + buttonSize / 2.0f,
        buttonPos.y + buttonSize / 2.0f
    ));
}