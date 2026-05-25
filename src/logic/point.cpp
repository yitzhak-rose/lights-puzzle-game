//
// Created by Benji Mandel on 5/25/26.
//

#include "logic/Point.h"

Point::Point(const sf::Vector2f& position, unsigned int outgoingLines)
    : position(position)
    , outgoingLines(outgoingLines)
    , isLit(false) {
    if (outgoingLines > MAX_LINES) {
        throw std::out_of_range("Point: outgoingLines exceeds MAX_LINES");
    }
}

sf::Vector2f& Point::getPosition() {
    return position;
}

const sf::Vector2f& Point::getPosition() const {
    return position;
}

bool Point::getLitStatus() const {
    return isLit;
}

unsigned int Point::getOutgoingLines() const {
    return outgoingLines;
}

void Point::setLitStatus(bool lit) {
    isLit = lit;
}