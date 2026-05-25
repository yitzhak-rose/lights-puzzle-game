#pragma once

// Number of directions in a hexagonal grid (flat-top, clockwise from NE).
// 0=NE  1=E  2=SE  3=SW  4=W  5=NW
inline constexpr int NUM_DIRECTIONS = 6;

static constexpr float HEX_SPACING_X = 80.f;
static constexpr float HEX_SPACING_Y = 70.f;

// Replaces boolean parameters in rotate().
enum class RotationDir {
    Clockwise,
    CounterClockwise
};