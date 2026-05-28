#pragma once

#include <SFML/Graphics/Color.hpp>
#include <string>

// UI constants
constexpr float        LINE_SIZE    = 24.0f;
constexpr float        LINE_LENGTH  = 48.0f;
constexpr float        POINT_RADIUS = 6.75f;
constexpr unsigned int MAX_LINES    = 8;
constexpr sf::Color    UNLIT_COLOR  = sf::Color::White;
constexpr sf::Color    LIT_COLOR    = sf::Color::Yellow;

// Logic constants
inline constexpr int NUM_DIRECTIONS = 6;
inline constexpr float PI = 3.14159265358979f;

static constexpr float HEX_SPACING_X = 80.f;
static constexpr float HEX_SPACING_Y = 70.f;

enum class RotationDir {
    Clockwise,
    CounterClockwise
};

namespace Keywords {
    const std::string ROWS = "ROWS";
    const std::string ROW = "ROW";
    const std::string CENTER = "CENTER";
    const std::string NODE = "NODE";
    const std::string ROTATION = "ROTATION";
}