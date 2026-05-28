#include "view/Graph.h"
#include "Board.h"
#include "constants.h"
#include <cmath>

GraphView::GraphView()
    : points()
    , m_scale(1.0f) {}

GraphView::~GraphView() {}

void GraphView::clear() {
    points.clear();
}

void GraphView::createPoints(sf::Vector2u windowSize, const Board& board) {
    points.clear();

    const std::vector<int>& rowCounts = board.getRowSizes();
    if (rowCounts.empty()) return;

    const int totalRows = static_cast<int>(rowCounts.size());

    // Calculate unscaled grid dimensions
    const float dx = LINE_LENGTH * 2.0f;
    const float dy = dx * 0.8660254f;

    int max_cols = 0;
    for (int count : rowCounts) {
        if (count > max_cols) {
            max_cols = count;
        }
    }

    const float gridWidth  = static_cast<float>(max_cols - 1) * dx;
    const float gridHeight = static_cast<float>(totalRows - 1) * dy;

    // Available target size (subtracting margins for nice framing)
    const float targetWidth  = static_cast<float>(windowSize.x) - 120.0f;
    const float targetHeight = static_cast<float>(windowSize.y) - 160.0f;

    // Determine scale factor
    float scaleX = 1.0f;
    if (gridWidth > 0.0f) {
        scaleX = targetWidth / gridWidth;
    }
    float scaleY = 1.0f;
    if (gridHeight > 0.0f) {
        scaleY = targetHeight / gridHeight;
    }
    m_scale = std::min(scaleX, scaleY);
    if (m_scale > 1.0f) {
        m_scale = 1.0f; // Do not scale up beyond default
    }

    const float centerX = static_cast<float>(windowSize.x) / 2.0f;
    const float centerY = static_cast<float>(windowSize.y) / 2.0f;

    size_t dataIndex = 0U;

    for (size_t row = 0; row < rowCounts.size(); ++row) {
        const int count = rowCounts[row];

        // Center row unscaled offset in X
        const float rowWidth = static_cast<float>(count - 1) * dx;

        for (int col = 0; col < count; ++col) {
            // Unscaled position relative to the grid center
            const float xOffset = -rowWidth / 2.0f + static_cast<float>(col) * dx;
            const float yOffset = -gridHeight / 2.0f + static_cast<float>(row) * dy;

            // Apply scaling to the offsets
            const float xPos = centerX + xOffset * m_scale;
            const float yPos = centerY + yOffset * m_scale;

            const bool isCentre = (static_cast<int>(dataIndex) == board.getCenterNodeId());

            if (dataIndex < static_cast<size_t>(board.nodeCount())) {
                const Node& node = board.getGraph().getNode(static_cast<int>(dataIndex));
                
                Point::LineSlots slots;
                for (int d = 0; d < 6; ++d) {
                    slots[static_cast<size_t>((d + 5) % 6)] = node.hasArm(d);
                }

                Point p(sf::Vector2f(xPos, yPos), slots, isCentre);
                p.setLitStatus(node.isLit());

                PointShape ps(p);
                ps.setScale(sf::Vector2f(m_scale, m_scale));
                points.emplace_back(ps);
            }
            ++dataIndex;
        }
    }
}

int GraphView::getPointIndexAt(const sf::Vector2f& position) const {
    const float radius   = 38.0f * m_scale; // More comfortable hit target for gameplay, scaled
    const float radiusSq = radius * radius;

    for (size_t i = 0; i < points.size(); ++i) {
        const sf::Vector2f pointPos = points[i].getPosition();
        const float dx              = position.x - pointPos.x;
        const float dy              = position.y - pointPos.y;
        const float distSq          = dx * dx + dy * dy;

        if (distSq <= radiusSq) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void GraphView::updatePointsFromBoard(const Board& board) {
    for (size_t i = 0; i < points.size(); ++i) {
        if (i >= static_cast<size_t>(board.nodeCount())) break;

        const Node& node = board.getGraph().getNode(static_cast<int>(i));
        
        Point::LineSlots slots;
        for (int d = 0; d < 6; ++d) {
            slots[static_cast<size_t>((d + 5) % 6)] = node.hasArm(d);
        }
        
        const bool isCentre = (static_cast<int>(i) == board.getCenterNodeId());
        Point p(points[i].getPosition(), slots, isCentre);
        p.setLitStatus(node.isLit());
        
        points[i].updateData(p);
        points[i].setScale(sf::Vector2f(m_scale, m_scale));
    }
}

void GraphView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const PointShape& pointShape : points) {
        target.draw(pointShape, states);
    }
}
