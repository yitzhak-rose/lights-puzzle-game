#include "view/BoardRenderer.h"
#include "logic/Board.h"
#include "constants.h"
#include <cmath>

// Converts Node arm directions into PointShape slot convention (index 0 = right, CCW).
static PointShape::LineSlots slotsFromNode(const Node& node) {
    PointShape::LineSlots slots;
    for (int d = 0; d < 6; ++d)
        slots[static_cast<size_t>((d + 5) % 6)] = node.hasArm(d);
    return slots;
}

BoardRenderer::BoardRenderer() = default;

void BoardRenderer::init(sf::Vector2u windowSize, const Board& board) {
    m_points.clear();
    if (board.nodeCount() == 0) return;

    // Explicitly convert window size to float vector for calculations
    sf::Vector2f fWindowSize(windowSize);

    //  Find the logical boundaries
    sf::FloatRect bounds = calculateBoardBounds(board);

    //  Determine scale
    m_scale = calculateTargetScale(fWindowSize, bounds.size.x, bounds.size.y);

    //  Create and position shapes
    createPointShapes(fWindowSize, board, bounds);
}

sf::FloatRect BoardRenderer::calculateBoardBounds(const Board& board) const {
    float minX = 1e9f, maxX = -1e9f, minY = 1e9f, maxY = -1e9f;

    for (int i = 0; i < board.nodeCount(); ++i) {
        auto [lx, ly] = board.getNodePosition(i);
        if (lx < minX) minX = lx; if (lx > maxX) maxX = lx;
        if (ly < minY) minY = ly; if (ly > maxY) maxY = ly;
    }

    //  Rect constructor: {position, size}
    return sf::FloatRect({ minX, minY }, { maxX - minX, maxY - minY });
}

float BoardRenderer::calculateTargetScale(sf::Vector2f windowSize, float boardW, float boardH) const {
    if (boardW <= 0 || boardH <= 0) return 1.0f;

    const float margin = 120.0f;
    float targetW = windowSize.x - margin;
    float targetH = windowSize.y - margin;

    float scale = std::min(targetW / boardW, targetH / boardH);
    return std::min(scale, 1.2f);
}

void BoardRenderer::createPointShapes(sf::Vector2f windowSize, const Board& board, const sf::FloatRect& bounds) {

    sf::Vector2f boardCenter = bounds.position + (bounds.size / 2.0f);
    sf::Vector2f windowCenter = windowSize / 2.0f;

    for (int i = 0; i < board.nodeCount(); ++i) {
        auto [lx, ly] = board.getNodePosition(i);

        // Logical to screen conversion
        sf::Vector2f screenPos = (sf::Vector2f(lx, ly) - boardCenter) * m_scale + windowCenter;

        const Node& node = board.getGraph().getNode(i);
        bool isCentre = (i == board.getCenterNodeId());

        PointShape ps(screenPos, slotsFromNode(node), node.isLit(), isCentre);
        ps.setScale({ m_scale, m_scale });
        m_points.emplace_back(std::move(ps));
    }
}

int BoardRenderer::getNodeAt(const sf::Vector2f& mousePos) const {
    const float rSq = (38.0f * m_scale) * (38.0f * m_scale);
    for (size_t i = 0; i < m_points.size(); ++i) {
        const sf::Vector2f p = m_points[i].getPosition();
        const float dx = mousePos.x - p.x;
        const float dy = mousePos.y - p.y;
        if (dx * dx + dy * dy <= rSq)
            return static_cast<int>(i);
    }
    return -1;
}

void BoardRenderer::sync(const Board& board) {
    for (size_t i = 0; i < m_points.size(); ++i) {
        if (i >= static_cast<size_t>(board.nodeCount())) break;
        const Node& node = board.getGraph().getNode(static_cast<int>(i));
        m_points[i].update(slotsFromNode(node), node.isLit());
    }
}

void BoardRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const PointShape& ps : m_points)
        target.draw(ps, states);
}
