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
    explicit PointShape(Point point);
    ~PointShape() override;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

    std::vector<LineShape> lines;
    Point pointData;
};
