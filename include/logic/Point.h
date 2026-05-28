#pragma once

#include <SFML/System/Vector2.hpp>
#include <array>
#include <stdexcept>
#include "../include/constants.h"

class Point {
public:
    using LineSlots = std::array<bool, 6>;

    /// @brief Each index 0..5 corresponds to one of the six possible line directions.
    ///        true means a line exists in that direction.
    Point(const sf::Vector2f& position, const LineSlots& lineSlots, bool isCentre);

    /// @brief Returns the point position in world coordinates.
    const sf::Vector2f& getPosition() const;

    /// @brief Returns whether this point is lit.
    bool getLitStatus() const;
    /// @brief Returns the active line slots for this point.
    const LineSlots& getLineSlots() const;

    /// @brief Sets whether the point is lit.
    void setLitStatus(bool lit);

    /// @brief Rotates line slots clockwise by one step (60 degrees).
    void rotateClockwise();
    /// @brief Rotates line slots counterclockwise by one step (60 degrees).
    void rotateCounterClockwise();

    /// @brief Updates the line slots.
    void setLineSlots(const LineSlots& newLineSlots);
    /// @brief Marks the point as a centre point.
    void setCentre(bool centre);

private:
    /// @brief Updates the point position.
    void setPosition(const sf::Vector2f& newPosition);

    sf::Vector2f position;
    LineSlots lineSlots;
    bool isLit;
    bool isCentre;
};