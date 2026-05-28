#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "../include/logic/Point.h"
#include "../include/view/PointShape.h"

class Board;

class GraphView : public sf::Drawable {
public:
    /// @brief Creates a graph view.
    GraphView();
    /// @brief Default destructor.
    ~GraphView() override;

    /// @brief Clears all points.
    void clear();
    
    /// @brief Builds a centered hex layout using the window size and the board's nodes.
    void createPoints(sf::Vector2u windowSize, const Board& board);

    /// @brief Returns the index of the point shape at the given mouse position, or -1.
    int getPointIndexAt(const sf::Vector2f& position) const;

    /// @brief Updates all visual point shapes from the logical board nodes.
    void updatePointsFromBoard(const Board& board);

private:
    /// @brief Draws all point shapes.
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    std::vector<PointShape> points;
    float m_scale;
};