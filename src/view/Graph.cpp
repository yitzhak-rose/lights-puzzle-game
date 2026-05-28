#include "view/Graph.h"
#include "Board.h"
#include "constants.h"
#include <cmath>

GraphView::GraphView()
    : points() {}

GraphView::~GraphView() {}

void GraphView::clear() {
    points.clear();
}

void GraphView::createPoints(sf::Vector2u windowSize, const Board& board) {
    points.clear();

    const std::vector<int>& rowCounts = board.getRowSizes();
    if (rowCounts.empty()) return;

    const int rowsAbove = static_cast<int>(rowCounts.size() / 2);

    const float dx = LINE_LENGTH * 2.0f;
    const float dy = dx * 0.8660254f;

    const float centerX = static_cast<float>(windowSize.x) / 2.0f;
    const float centerY = static_cast<float>(windowSize.y) / 2.0f;
    const float startY  = centerY - static_cast<float>(rowsAbove) * dy;

    size_t dataIndex = 0U;

    for (size_t row = 0; row < rowCounts.size(); ++row) {
        const int count = rowCounts[row];

        const float rowWidth = static_cast<float>(count - 1) * dx;
        const float startX   = centerX - rowWidth / 2.0f;
        const float yPos     = startY + static_cast<float>(row) * dy;

        for (int col = 0; col < count; ++col) {
            const float xPos     = startX + static_cast<float>(col) * dx;
            const bool  isCentre = (static_cast<int>(dataIndex) == board.getCenterNodeId());

            if (dataIndex < static_cast<size_t>(board.nodeCount())) {
                const Node& node = board.getGraph().getNode(static_cast<int>(dataIndex));
                
                Point::LineSlots slots;
                for (int d = 0; d < 6; ++d) {
                    slots[d] = node.hasArm(d);
                }

                Point p(sf::Vector2f(xPos, yPos), slots, isCentre);
                p.setLitStatus(node.isLit());

                points.emplace_back(p);
            }
            ++dataIndex;
        }
    }
}

int GraphView::getPointIndexAt(const sf::Vector2f& position) const {
    const float radius   = 38.0f; // More comfortable hit target for gameplay
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
            slots[d] = node.hasArm(d);
        }
        
        const bool isCentre = (static_cast<int>(i) == board.getCenterNodeId());
        Point p(points[i].getPosition(), slots, isCentre);
        p.setLitStatus(node.isLit());
        
        points[i].updateData(p);
    }
}

void GraphView::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const PointShape& pointShape : points) {
        target.draw(pointShape, states);
    }
}
