#pragma once

#include <SFML/System/Vector2.hpp>
#include <stdexcept>
#include "../include/constants.h"

class Point {
public:
    Point(const sf::Vector2f& position, unsigned int outgoingLines);

    sf::Vector2f& getPosition();
    const sf::Vector2f& getPosition() const;

    bool getLitStatus() const;
    unsigned int getOutgoingLines() const;

    void setLitStatus(bool lit);

private:
    sf::Vector2f position;
    unsigned int outgoingLines;
    bool isLit;
};