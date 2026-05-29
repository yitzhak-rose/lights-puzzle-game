#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <array>
#include <vector>

#include "../include/view/LineShape.h"
#include "../include/constants.h"

// Drawable hex node: a circle with up to 6 arm lines.
// Inherits directly from SFML Drawable + Transformable (no intermediate Shape base).
class PointShape : public sf::Drawable, public sf::Transformable {
public:
    using LineSlots = std::array<bool, 6>;

    PointShape(const sf::Vector2f& position,
               const LineSlots&   slots,
               bool isLit,
               bool isCentre);
    ~PointShape() override = default;

    // Syncs visual state after the board changes (lit status + arm directions).
    void update(const LineSlots& slots, bool isLit);

    sf::Vector2f getPosition() const;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void buildLines();

    LineSlots            m_slots;
    bool                 m_isLit;
    bool                 m_isCentre;
    std::vector<LineShape> m_lines;
};
