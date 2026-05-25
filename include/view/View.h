#pragma once
#include <SFML/Graphics.hpp>

class View {
public:
    View();
    ~View();

    void run();

private:
    void updateCloseButton(sf::Vector2u windowSize);

    sf::Font uiFont;
    sf::Text closeText;
    sf::RectangleShape closeBox;
    bool fontLoaded;

};
