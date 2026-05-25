#include "view/PointShape.h"

PointShape::PointShape(Point point)
    : lines()
    , pointData(point) {
}

PointShape::~PointShape() {}

void PointShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  constexpr float radius = 6.0f;

  sf::CircleShape pointCircle(radius);
  pointCircle.setOrigin(sf::Vector2f(radius, radius));
  pointCircle.setPosition(pointData.getPosition());

  if (pointData.getLitStatus()) {
    pointCircle.setFillColor(sf::Color::Yellow);
  } else {
    pointCircle.setFillColor(sf::Color::White);
  }

  target.draw(pointCircle, states);

  // TODO(LineShape): when LineShape is drawable, draw each line here.
  // Example:
  // for (const LineShape& line : lines) {
  //     target.draw(line, states);
  // }
}