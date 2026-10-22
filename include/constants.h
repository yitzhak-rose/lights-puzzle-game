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
    inline const std::string ROWS = "ROWS";
    inline const std::string ROW = "ROW";
    inline const std::string CENTER = "CENTER";
    inline const std::string NODE = "NODE";
    inline const std::string ROTATION = "ROTATION";
}