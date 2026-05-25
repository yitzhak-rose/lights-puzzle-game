#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

class Shape : public sf::Drawable, public sf::Transformable {
public:
    ~Shape() override = default;
};