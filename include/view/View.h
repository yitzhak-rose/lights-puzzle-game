#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "../include/logic/Point.h"

class View {
public:
    /// @brief Creates the view and loads UI assets.
    View();
    /// @brief Default destructor.
    ~View();

    /// @brief Runs the view loop for the provided points and layout.
    void run(const std::vector<Point>& points, int midRowCount);

private:
    /// @brief Updates the close button bounds and text position.
    void updateCloseButton(sf::Vector2u windowSize);

    sf::Font uiFont;
    sf::Text closeText;
    sf::RectangleShape closeBox;
    bool fontLoaded;
};