// #include ".../include/view/View.h"

#include "../include/view/View.h"
#include <optional>

View::View()
    : uiFont()
    , closeText(uiFont)
    , closeBox()
    , fontLoaded(false) {
    fontLoaded = uiFont.openFromFile("../resources/Roboto-Medium.ttf");
    closeText.setString("X");
    closeText.setFillColor(sf::Color::White);
    closeText.setStyle(sf::Text::Bold);
}

View::~View() {}

void View::run() {
    const unsigned int windowWidth  = 800;
    const unsigned int windowHeight = 600;

    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "Empty Window");

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    updateCloseButton(window.getSize());

                    const sf::Vector2f mousePos(
                        static_cast<float>(mouseEvent->position.x),
                        static_cast<float>(mouseEvent->position.y)
                    );

                    if (closeBox.getGlobalBounds().contains(mousePos)) {
                        window.close();
                    }
                }
            }
        }

        updateCloseButton(window.getSize());

        window.clear();
        window.draw(closeBox);
        if (fontLoaded) {
            window.draw(closeText);
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