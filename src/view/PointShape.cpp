#include "view/PointShape.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Angle.hpp>

PointShape::PointShape(const sf::Vector2f& position,
                       const LineSlots&   slots,
                       bool isLit,
                       bool isCentre)
    : m_slots(slots)
    , m_isLit(isLit)
    , m_isCentre(isCentre) {
    setPosition(position);
    buildLines();
}

void PointShape::update(const LineSlots& slots, bool isLit) {
    m_slots = slots;
    m_isLit = isLit;
    buildLines();
}

sf::Vector2f PointShape::getPosition() const {
    return sf::Transformable::getPosition();
}

void PointShape::buildLines() {
    m_lines.clear();
    const sf::Color color = m_isLit ? LIT_COLOR : UNLIT_COLOR;

    for (unsigned int i = 0; i < m_slots.size(); ++i) {
        if (!m_slots[i]) continue;
        LineShape line;
        line.setPosition(sf::Vector2f(0.0f, 0.0f));
        line.setRotation(sf::degrees(static_cast<float>(i) * 60.0f));
        line.setColor(color);
        m_lines.push_back(line);
    }
}

void PointShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();

    for (const LineShape& line : m_lines)
        target.draw(line, states);

    sf::CircleShape circle(POINT_RADIUS);
    circle.setOrigin(sf::Vector2f(POINT_RADIUS, POINT_RADIUS));
    circle.setPosition(sf::Vector2f(0.0f, 0.0f));
    circle.setFillColor(m_isCentre ? sf::Color(255, 215, 0)
                                   : (m_isLit ? LIT_COLOR : UNLIT_COLOR));
    target.draw(circle, states);
}
