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

    const std::vector<int>& rowCounts = board.getRowSizes();
    if (rowCounts.empty()) return;

    const int   totalRows = static_cast<int>(rowCounts.size());
    const float dx        = LINE_LENGTH * 2.0f;
    const float dy        = dx * 0.8660254f; // sqrt(3)/2 for hex grid

    int maxCols = 0;
    for (int c : rowCounts) maxCols = std::max(maxCols, c);

    const float gridW = static_cast<float>(maxCols - 1) * dx;
    const float gridH = static_cast<float>(totalRows - 1) * dy;

    const float targetW = static_cast<float>(windowSize.x) - 120.0f;
    const float targetH = static_cast<float>(windowSize.y) - 160.0f;

    m_scale = std::min(
        gridW > 0.0f ? targetW / gridW : 1.0f,
        gridH > 0.0f ? targetH / gridH : 1.0f);
    if (m_scale > 1.0f) m_scale = 1.0f;

    const float cx = static_cast<float>(windowSize.x) / 2.0f;
    const float cy = static_cast<float>(windowSize.y) / 2.0f;

    int idx = 0;
    for (int row = 0; row < totalRows; ++row) {
        const int   count  = rowCounts[row];
        const float rowW   = static_cast<float>(count - 1) * dx;
        for (int col = 0; col < count; ++col) {
            const float x = cx + (-rowW / 2.0f + col * dx) * m_scale;
            const float y = cy + (-gridH / 2.0f + row * dy) * m_scale;

            const Node& node     = board.getGraph().getNode(idx);
            const bool  isCentre = (idx == board.getCenterNodeId());

            PointShape ps(sf::Vector2f(x, y), slotsFromNode(node), node.isLit(), isCentre);
            ps.setScale(sf::Vector2f(m_scale, m_scale));
            m_points.emplace_back(std::move(ps));
            ++idx;
        }
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
