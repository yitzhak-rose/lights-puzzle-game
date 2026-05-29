#include "view/LineShape.h"

LineShape::LineShape(float length, float thickness)
    : m_rect()
    , m_length(length)
    , m_thickness(thickness) {
    setLength(length);
    setThickness(thickness);
    m_rect.setFillColor(UNLIT_COLOR);
}

void LineShape::setLength(float length) {
    m_length = length;
    m_rect.setSize(sf::Vector2f(m_length, m_thickness));
    m_rect.setOrigin(sf::Vector2f(0.0f, m_thickness / 2.0f));
}

void LineShape::setThickness(float thickness) {
    m_thickness = thickness;
    m_rect.setSize(sf::Vector2f(m_length, m_thickness));
    m_rect.setOrigin(sf::Vector2f(0.0f, m_thickness / 2.0f));
}

void LineShape::setColor(sf::Color color) {
    m_rect.setFillColor(color);
}

float LineShape::getLength()    const { return m_length; }
float LineShape::getThickness() const { return m_thickness; }

void LineShape::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(m_rect, states);
}
