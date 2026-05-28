#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>
#include <vector>

#include "../include/logic/Point.h"
#include "../include/view/PointShape.h"

class Graph : public sf::Drawable {
public:
    /// @brief Creates a graph view with the requested middle row count.
    Graph(int midRowCount);
    /// @brief Default destructor.
    ~Graph() override;

    /// @brief Stores the logical points (line slots) to be laid out and drawn.
    void setPoints(const std::vector<Point>& newPoints);
    /// @brief Clears all points.
    void clear();
    /// @brief Builds a centered hex layout using the window size and point data.
    ///        Rows grow to the middle then shrink, spacing is 2*LINE_LENGTH horizontally
    ///        and sin(60°)*dx vertically. The layout is centered in the window.
    void createPoints(sf::Vector2u windowSize);
    /// @brief Rotates the first point under the cursor (true=clockwise, false=counter).
    void rotatePointAt(const sf::Vector2f& position, bool clockwise);

private:
    /// @brief Draws all point shapes.
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    /// @brief Builds the row counts for the hex layout.
    //TODO: temporary func
    std::vector<int> buildRowCounts() const;
    /// @brief Computes line slots based on neighbour positions for the given row/col.
    //TODO: temporary func
    Point::LineSlots computeLineSlots(int row, int col, const std::vector<int>& rowCounts) const;

    int m_midRowCount;
    std::vector<Point> pointData;
    std::vector<PointShape> points;
};