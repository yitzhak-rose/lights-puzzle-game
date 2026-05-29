#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "../include/view/PointShape.h"

class Board;

// Translates the logical Board into SFML drawables.
// Knows about pixels; Board knows nothing about pixels.
class BoardRenderer : public sf::Drawable {
public:
    BoardRenderer();
    ~BoardRenderer() override = default;

    // Builds the initial visual layout, scaled to fit the window.
    void init(sf::Vector2u windowSize, const Board& board);

    // Returns node index under the mouse, or -1 if none.
    int getNodeAt(const sf::Vector2f& mousePos) const;

    // Syncs lit/arm state after a rotation.
    void sync(const Board& board);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::vector<PointShape> m_points;
    float m_scale = 1.0f;
};
