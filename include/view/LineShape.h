#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "../include/constants.h"

// A single arm line drawn from the center of a hex node outward.
// Inherits directly from SFML Drawable + Transformable (no intermediate Shape base).
class LineShape : public sf::Drawable, public sf::Transformable {
public:
    explicit LineShape(float length = LINE_LENGTH, float thickness = 4.0f);
    ~LineShape() override = default;

    void setLength(float length);
    void setThickness(float thickness);
    void setColor(sf::Color color);

    float getLength()    const;
    float getThickness() const;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::RectangleShape m_rect;
    float m_length;
    float m_thickness;
};
