#include "view/Graph.h"

#include <algorithm>
#include <random>

Graph::Graph(int midRowCount)
    : m_midRowCount(midRowCount)
    , pointData()
    , points() {}

Graph::~Graph() {}

void Graph::setPoints(const std::vector<Point>& newPoints) {
    pointData = newPoints;
}

void Graph::clear() {
    points.clear();
    pointData.clear();
}

std::vector<int> Graph::buildRowCounts() const {
    std::vector<int> counts;

    const int minCount     = 3;
    const int effectiveMid = (m_midRowCount < minCount) ? minCount : m_midRowCount;
    const int rowsAbove    = effectiveMid - minCount;
    const int totalRows    = rowsAbove * 2 + 1;

    counts.reserve(static_cast<size_t>(totalRows));

    for (int row = 0; row < totalRows; ++row) {
        const int count = (row <= rowsAbove)
            ? minCount + row
            : minCount + (2 * rowsAbove - row);
        counts.push_back(count);
    }

    return counts;
}

Point::LineSlots Graph::computeLineSlots(int row, int col, const std::vector<int>& rowCounts) const {
    Point::LineSlots slots{false, false, false, false, false, false};
    std::vector<int> availableSlots;

    const size_t rowIndex = static_cast<size_t>(row);
    const int    rowCount = rowCounts[rowIndex];

    if (col + 1 < rowCount) {
        availableSlots.push_back(0);
    }
    if (col - 1 >= 0) {
        availableSlots.push_back(3);
    }

    if (row > 0) {
        const size_t aboveIndex = static_cast<size_t>(row - 1);
        const int    aboveCount = rowCounts[aboveIndex];

        if (aboveCount < rowCount) {
            if (col - 1 >= 0) {
                availableSlots.push_back(4);
            }
            if (col < aboveCount) {
                availableSlots.push_back(5);
            }
        } else {
            if (col < aboveCount) {
                availableSlots.push_back(4);
            }
            if (col + 1 < aboveCount) {
                availableSlots.push_back(5);
            }
        }
    }

    if (row + 1 < static_cast<int>(rowCounts.size())) {
        const size_t belowIndex = static_cast<size_t>(row + 1);
        const int    belowCount = rowCounts[belowIndex];

        if (belowCount < rowCount) {
            if (col - 1 >= 0) {
                availableSlots.push_back(2);
            }
            if (col < belowCount) {
                availableSlots.push_back(1);
            }
        } else {
            if (col < belowCount) {
                availableSlots.push_back(2);
            }
            if (col + 1 < belowCount) {
                availableSlots.push_back(1);
            }
        }
    }

    if (availableSlots.empty()) {
        slots[0] = true;
        return slots;
    }

    static std::mt19937 rng(1337);
    std::shuffle(availableSlots.begin(), availableSlots.end(), rng);

    int maxCount = static_cast<int>(availableSlots.size());
    if (maxCount == 6) {
        maxCount = 5;
    }

    std::uniform_int_distribution<int> countDist(1, maxCount);
    const int selectedCount = countDist(rng);

    for (int index = 0; index < selectedCount; ++index) {
        const int slotIndex = availableSlots[static_cast<size_t>(index)];
        slots[static_cast<size_t>(slotIndex)] = true;
    }

    return slots;
}

void Graph::createPoints(sf::Vector2u windowSize) {
    points.clear();

    const std::vector<int> rowCounts = buildRowCounts();
    const int              rowsAbove = static_cast<int>(rowCounts.size() / 2);

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
            const bool  isCentre = (row == static_cast<size_t>(rowsAbove)) && (col == count / 2);

            const bool useData = (dataIndex < pointData.size());
            const Point::LineSlots slots = useData
                ? pointData[dataIndex].getLineSlots()
                : computeLineSlots(static_cast<int>(row), col, rowCounts);

            points.emplace_back(Point(sf::Vector2f(xPos, yPos), slots, isCentre));
            ++dataIndex;
        }
    }
}

void Graph::rotatePointAt(const sf::Vector2f& position, bool clockwise) {
    const float radius   = POINT_RADIUS;
    const float radiusSq = radius * radius;

    for (PointShape& pointShape : points) {
        const sf::Vector2f pointPos = pointShape.getPosition();
        const float dx              = position.x - pointPos.x;
        const float dy              = position.y - pointPos.y;
        const float distSq          = dx * dx + dy * dy;

        if (distSq <= radiusSq) {
            if (clockwise) {
                pointShape.rotateClockwise();
            } else {
                pointShape.rotateCounterClockwise();
            }
            return;
        }
    }
}

void Graph::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const PointShape& pointShape : points) {
        target.draw(pointShape, states);
    }
}
