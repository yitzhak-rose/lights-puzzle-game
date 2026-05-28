#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "../include/constants.h"
#include "../include/view/Shape.h"

class LineShape : public Shape {
public:
    /// @brief Creates a line segment with the given length and thickness.
    explicit LineShape(float length = LINE_LENGTH, float thickness = 4.0f);
    /// @brief Default destructor.
    ~LineShape() override = default;

    /// @brief Sets the line length.
    void setLength(float length);
    /// @brief Sets the line thickness.
    void setThickness(float thickness);
    /// @brief Sets the line color.
    void setColor(sf::Color color);

    /// @brief Returns the line length.
    float getLength() const;
    /// @brief Returns the line thickness.
    float getThickness() const;

private:
    /// @brief Draws the line.
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::RectangleShape lineRect;
    float lineLength;
    float lineThickness;
};
