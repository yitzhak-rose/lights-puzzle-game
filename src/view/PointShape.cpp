#include "view/PointShape.h"

#include <SFML/System/Angle.hpp>

PointShape::PointShape(Point point)
    : lines()
    , pointData(point) {
    setPosition(pointData.getPosition());
    buildLines(pointData.getLineSlots());
}

PointShape::~PointShape() {}

void PointShape::buildLines(const Point::LineSlots& slots) {
    lines.clear();
    sf::Color currentColor = pointData.getLitStatus() ? LIT_COLOR : UNLIT_COLOR;

    for (unsigned int index = 0; index < slots.size(); ++index) {
        if (!slots[index]) {
            continue;
        }

        LineShape line;
        line.setPosition(sf::Vector2f(0.0f, 0.0f));
        line.setRotation(sf::degrees(static_cast<float>(index) * 60.0f));
        line.setColor(currentColor);
        lines.push_back(line);
    }
}

void PointShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    sf::CircleShape pointCircle(POINT_RADIUS);
    pointCircle.setOrigin(sf::Vector2f(POINT_RADIUS, POINT_RADIUS));
    pointCircle.setPosition(sf::Vector2f(0.0f, 0.0f));
    pointCircle.setFillColor(pointData.getLitStatus() ? LIT_COLOR : UNLIT_COLOR);

    for (const LineShape& line : lines) {
        target.draw(line, states);
    }

    target.draw(pointCircle, states);
}

void PointShape::rotateClockwise() {
    pointData.rotateClockwise();
    buildLines(pointData.getLineSlots());
}

void PointShape::rotateCounterClockwise() {
    pointData.rotateCounterClockwise();
    buildLines(pointData.getLineSlots());
}

void PointShape::updateData(Point point) {
    pointData = point;
    buildLines(pointData.getLineSlots());
}
