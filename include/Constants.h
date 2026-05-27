#pragma once
#include <string>

// Number of directions in a hexagonal grid (flat-top, clockwise from NE).
// 0=NE  1=E  2=SE  3=SW  4=W  5=NW
inline constexpr int NUM_DIRECTIONS = 6;
inline constexpr float PI = 3.14159265358979f;

static constexpr float HEX_SPACING_X = 80.f;
static constexpr float HEX_SPACING_Y = 70.f;

// Replaces boolean parameters in rotate().
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