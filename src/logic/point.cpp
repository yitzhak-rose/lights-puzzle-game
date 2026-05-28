//
// Created by Benji Mandel on 5/25/26.
//

#include "logic/Point.h"

Point::Point(const sf::Vector2f& position, const LineSlots& lineSlots, bool isCentre)
    : position(sf::Vector2f())
    , lineSlots()
    , isLit(false)
    , isCentre(false) {
    setPosition(position);
    setLineSlots(lineSlots);
    setCentre(isCentre);

    if (this->isCentre) {
        setLitStatus(true);
    }
}

const sf::Vector2f& Point::getPosition() const {
    return position;
}

bool Point::getLitStatus() const {
    return isLit;
}

const Point::LineSlots& Point::getLineSlots() const {
    return lineSlots;
}

void Point::setLitStatus(bool lit) {
    isLit = lit;
}

void Point::setPosition(const sf::Vector2f& newPosition) {
    position = newPosition;
}

void Point::setLineSlots(const LineSlots& newLineSlots) {
    lineSlots = newLineSlots;
}

void Point::setCentre(bool centre) {
    isCentre = centre;
}

void Point::rotateClockwise() {
    const bool last = lineSlots[5];

    for (int index = 5; index > 0; --index) {
        lineSlots[static_cast<size_t>(index)] = lineSlots[static_cast<size_t>(index - 1)];
    }

    lineSlots[0] = last;
}

void Point::rotateCounterClockwise() {
    const bool first = lineSlots[0];

    for (size_t index = 0; index + 1 < lineSlots.size(); ++index) {
        lineSlots[index] = lineSlots[index + 1];
    }

    lineSlots[5] = first;
}
