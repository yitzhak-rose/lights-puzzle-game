#pragma once

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <vector>

#include "../include/logic/Point.h"
#include "../include/view/LineShape.h"
#include "../include/view/Shape.h"

class PointShape : public Shape {
public:
    /// @brief Creates a drawable point shape for a point.
    explicit PointShape(Point point);
    /// @brief Default destructor.
    ~PointShape() override;

    /// @brief Rotates the point and its lines clockwise by 60 degrees.
    void rotateClockwise();
    /// @brief Rotates the point and its lines counterclockwise by 60 degrees.
    void rotateCounterClockwise();

private:
    /// @brief Draws the point and its lines.
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    /// @brief Builds LineShape instances from the point's line slots.
    void buildLines(const Point::LineSlots& slots);

    std::vector<LineShape> lines;
    Point pointData;
};
