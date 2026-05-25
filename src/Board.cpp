#include "Board.h"
#include <stdexcept>
#include <fstream>
#include <cmath>
#include <queue>
#include <algorithm>
#include "Constants.h"

// ─── Level file format ───────────────────────────────────────────────────────
//
// Lines starting with '#' are comments and are skipped.
//
// ROWS <n>               – total number of rows
// ROW <r> <size>         – size of row r (one line per row)
// CENTER <id>            – node ID of the centre node
// NODE <id> <a0>..<a5> ROTATION <r>     – one line per node:
//     a0..a5 ∈ {0,1}: base arms before any rotation
//     r: initial scramble rotation (0-5), applied clockwise

//  Loading a level
void Board::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Board::loadFromFile – cannot open file: " + filename);

    // Read metadata and return temporary row sizes array
    std::vector<int> rowSizes = readLevelInfo(file);

    // Use the temporary array to compute and store (X,Y) positions for each hex
    computeNodePositions(rowSizes);

    // Parse the nodes (arms and rotation) and build them into the graph
    parseNodes(file);

    // Now that positions are set, check distances to connect neighbors
    buildAdjacency();

    // 5. Light up the board starting from the power source
    updateLighting();
}

//  read how many rows there are, their sizes, and who the power source is.
std::vector<int> Board::readLevelInfo(std::ifstream& file) {

    // Read total number of rows
    skipComments(file);
    std::string keyword;
    int totalRows;
    file >> keyword >> totalRows;
    if (file.fail() || keyword != "ROWS" || totalRows < 1)
        throw std::runtime_error("Board::parseMetadata – expected 'ROWS <n>'");

    // Read each row and store its size
    std::vector<int> rowSizes(totalRows);
    for (int r = 0; r < totalRows; r++) {
        skipComments(file);
        int rowIndex, rowSize;
        file >> keyword >> rowIndex >> rowSize;
        if (file.fail() || keyword != "ROW" || rowIndex != r || rowSize < 1)
            throw std::runtime_error("Board::parseMetadata – invalid ROW entry for row " + std::to_string(r));

        rowSizes[r] = rowSize;
    }

    // Read the power source (center node) ID
    skipComments(file);
    file >> keyword >> m_centerNodeId;
    if (file.fail() || keyword != "CENTER" || m_centerNodeId < 0)
        throw std::runtime_error("Board::parseMetadata – expected 'CENTER <id>'");

    return rowSizes;
}

// Geometric Math - Calculates exactly where each hexagon should be drawn on the screen.
void Board::computeNodePositions(const std::vector<int>& rowSizes) {
    int totalRows = static_cast<int>(rowSizes.size());
    float totalHeight = (totalRows - 1) * HEX_SPACING_Y;
    float startY = -totalHeight / 2.f;

    m_nodePositions.clear();

    // Build the geometric grid centered around (0,0)
    for (int r = 0; r < totalRows; r++) {
        int cols = rowSizes[r];
        float rowWidth = (cols - 1) * HEX_SPACING_X;
        float startX = -rowWidth / 2.f;
        float y = startY + r * HEX_SPACING_Y;

        for (int c = 0; c < cols; c++)
            m_nodePositions.emplace_back(startX + c * HEX_SPACING_X, y);
    }

    // Sanity check ensure the read center ID actually exists on the generated board
    if (m_centerNodeId >= static_cast<int>(m_nodePositions.size()))
        throw std::runtime_error("Board::computeNodePositions – CENTER id exceeds total node count");
}


// Creating the Bulbs - Reads the actual arms and rotation for every hexagon and adds it to our graph.
void Board::parseNodes(std::ifstream& file) {
    int expectedNodes = static_cast<int>(m_nodePositions.size());
    m_graph.clear();

    for (int id = 0; id < expectedNodes; id++) {
        skipComments(file);
        std::string nodeKeyword, rotKeyword;
        int fileId, rotation;
        file >> nodeKeyword >> fileId;

        if (file.fail() || nodeKeyword != "NODE")
            throw std::runtime_error("Board::parseNodes – expected 'NODE' at node " + std::to_string(id));
        if (fileId != id)
            throw std::runtime_error("Board::parseNodes – expected node id " + std::to_string(id) + " but got " + std::to_string(fileId));

        // Read the 6 directions and check for arms
        std::vector<bool> arms(NUM_DIRECTIONS, false);
        for (int d = 0; d < NUM_DIRECTIONS; d++) {
            int val = 0;
            file >> val;
            if (file.fail() || (val != 0 && val != 1))
                throw std::runtime_error("Board::parseNodes – invalid arm value for node " + std::to_string(id));
            arms[d] = (val == 1);
        }

        // Read the initial rotation (puzzle scramble)
        file >> rotKeyword >> rotation;
        if (file.fail() || rotKeyword != "ROTATION" || rotation < 0 || rotation >= NUM_DIRECTIONS)
            throw std::runtime_error("Board::parseNodes – invalid ROTATION for node " + std::to_string(id));

        // Build the node, apply the scramble rotation, and add it to the graph
        Node node(id, arms);
        for (int i = 0; i < rotation; i++)
            node.rotate(RotationDir::Clockwise);

        m_graph.addNode(node);
    }
}

// Automatically connects hexagons in the graph if they are drawn close to each other.
void Board::buildAdjacency() {
    int n = m_graph.nodeCount();
    // Define a safety margin: neighboring hexes should be roughly one hex width apart
    float minDist = HEX_SPACING_X * 0.85f;
    float maxDist = HEX_SPACING_X * 1.15f;

    // Iterate over all possible pairs of nodes to check their distance
    for (int i = 0; i < n; i++) {
        auto [xi, yi] = m_nodePositions[i];
        for (int j = i + 1; j < n; j++) {
            auto [xj, yj] = m_nodePositions[j];
            float dx = xj - xi;
            float dy = yj - yi;
            float dist = std::sqrt(dx * dx + dy * dy);

            // If they are within the correct distance, they are neighbors! Connect them both ways
            if (dist >= minDist && dist <= maxDist) {

                // Calculate the angle between the two neighbors
                float angleDeg = std::atan2(-dy, dx) * 180.f / 3.14159265f;
                if (angleDeg < 0.f) angleDeg += 360.f;

                // Divide into 6 pie slices (hexagon) and translate to direction index (0 to 5)
                int sector = static_cast<int>((angleDeg + 30.f) / 60.f) % NUM_DIRECTIONS;
                static const int sectorToDir[NUM_DIRECTIONS] = { 1, 0, 5, 4, 3, 2 };

                int dirItoJ = sectorToDir[sector];
                int dirJtoI = (dirItoJ + 3) % NUM_DIRECTIONS; // Opposite direction (plus half a turn)

                m_graph.addEdge(i, j, dirItoJ);
                m_graph.addEdge(j, i, dirJtoI);
            }
        }
    }
}

void Board::updateLighting() {
   
}


void Board::skipComments(std::ifstream& file) {
    // Skips any line that starts with a hash or is completely empty
    while (file.peek() == '#' || file.peek() == '\n' || file.peek() == '\r') {
        std::string line;
        std::getline(file, line);
    }
}
