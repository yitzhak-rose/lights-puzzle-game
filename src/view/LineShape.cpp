#include "view/LineShape.h"

LineShape::LineShape(float length, float thickness)
    : lineRect()
    , lineLength(length)
    , lineThickness(thickness) {
    setLength(length);
    setThickness(thickness);
    lineRect.setFillColor(UNLIT_COLOR);
}

void LineShape::setLength(float length) {
    lineLength = length;
    lineRect.setSize(sf::Vector2f(lineLength, lineThickness));
    lineRect.setOrigin(sf::Vector2f(0.0f, lineThickness / 2.0f));
}

void LineShape::setThickness(float thickness) {
    lineThickness = thickness;
    lineRect.setSize(sf::Vector2f(lineLength, lineThickness));
    lineRect.setOrigin(sf::Vector2f(0.0f, lineThickness / 2.0f));
}

float LineShape::getLength() const {
    return lineLength;
}

float LineShape::getThickness() const {
    return lineThickness;
}

void LineShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(lineRect, states);
}
