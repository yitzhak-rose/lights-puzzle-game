#include "logic/Board.h"
#include "logic/BFS.h"
#include "constants.h"
#include <stdexcept>
#include <fstream>
#include <cmath>
#include <queue>
#include <algorithm>

// Main entry point for loading a level from a text file.
// Coordinates the entire initialization process.
void Board::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        throw std::runtime_error("Board::loadFromFile – cannot open file: " + filename);

    // 1. Read metadata (total rows, center ID) and get the size of each row
    m_rowSizes = readLevelInfo(file);

    // 2. Calculate the physical (X, Y) screen coordinates for each hexagon
    computeNodePositions(m_rowSizes);

    // 3. Read the internal data for each node (arms, initial rotation) and build the graph
    parseNodes(file);

    // 4. Connect neighboring nodes based on their calculated physical distances
    buildAdjacency();

    // 5. Initialize the game state by simulating the electricity flow from the center
    updateLighting();
}

// Reads the top-level configuration: total rows and the center power source ID.
std::vector<int> Board::readLevelInfo(std::ifstream& file) {
    int totalRows = getData(file, Keywords::ROWS);
    std::vector<int> rowSizes = getRowSizes(file, totalRows);
    m_centerNodeId = getData(file, Keywords::CENTER);

    return rowSizes;
}

// Helper function to read a specific keyword and its corresponding integer value.
int Board::getData(std::ifstream& file, const std::string& expectedKeyword) {
    skipComments(file);
    std::string keyword;
    int value;
    file >> keyword >> value;

    if (file.fail() || keyword != expectedKeyword) {
        throw std::runtime_error("Board::getData - Parsing error or unexpected keyword.");
    }
    if (keyword == Keywords::ROWS && value < 1) {
        throw std::runtime_error("Board::getData - Rows must be at least 1.");
    }
    if (keyword == Keywords::CENTER && value < 0) {
        throw std::runtime_error("Board::getData - CENTER must be non-negative.");
    }
    return value;
}

// Reads the specific number of hexagons (columns) for each individual row.
std::vector<int> Board::getRowSizes(std::ifstream& file, int totalRows) {
    std::vector<int> rowSizes(totalRows);

    for (int r = 0; r < totalRows; r++) {
        skipComments(file);
        std::string keyword;
        int rowIndex, rowSize;

        file >> keyword >> rowIndex >> rowSize;

        if (file.fail() || keyword != Keywords::ROW || rowIndex != r || rowSize < 1)
            throw std::runtime_error("Board::getRowSizes – invalid " + Keywords::ROW + " entry for row " + std::to_string(r));

        rowSizes[r] = rowSize;
    }

    return rowSizes;
}

// Calculates the (X, Y) coordinates for every node to visually center the entire grid around (0,0).
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

    // Sanity check: Ensure the center ID from the file actually fits within our generated board
    if (m_centerNodeId >= static_cast<int>(m_nodePositions.size()))
        throw std::runtime_error("Board::computeNodePositions – CENTER id exceeds total node count");
}

// Parses the detailed configuration of each node (which arms exist and its starting rotation).
void Board::parseNodes(std::ifstream& file) {
    int expectedNodes = static_cast<int>(m_nodePositions.size());
    m_graph.clear();

    for (int id = 0; id < expectedNodes; id++) {
        skipComments(file);
        std::string nodeKeyword, rotKeyword;
        int fileId, rotation;

        file >> nodeKeyword >> fileId;

        if (file.fail() || nodeKeyword != Keywords::NODE || fileId != id)
            throw std::runtime_error("Board::parseNodes – expected '" + Keywords::NODE + "' at node " + std::to_string(id));

        // Read the 6 possible arms (1 for active, 0 for inactive)
        std::vector<bool> arms(NUM_DIRECTIONS, false);
        for (int d = 0; d < NUM_DIRECTIONS; d++) {
            int val = 0;
            file >> val;
            if (file.fail() || (val != 0 && val != 1))
                throw std::runtime_error("Board::parseNodes – invalid arm value for node " + std::to_string(id));
            arms[d] = (val == 1);
        }

        // Read the initial scramble rotation
        file >> rotKeyword >> rotation;
        if (file.fail() || rotKeyword != Keywords::ROTATION || rotation < 0 || rotation >= NUM_DIRECTIONS)
            throw std::runtime_error("Board::parseNodes – invalid " + Keywords::ROTATION + " for node " + std::to_string(id));

        // Build node object and apply the initial rotation
        Node node(id, arms);
        for (int i = 0; i < rotation; i++)
            node.rotate(RotationDir::Clockwise);

        m_graph.addNode(node);
    }
}

// Dynamically creates graph edges by checking the physical distance and angle between nodes.
void Board::buildAdjacency() {
    int n = m_graph.nodeCount();

    // Define a safety margin: neighboring hexes should be roughly one hex width apart
    float minDist = HEX_SPACING_X * 0.85f;
    float maxDist = HEX_SPACING_X * 1.15f;

    for (int i = 0; i < n; i++) {
        auto [xi, yi] = m_nodePositions[i];
        for (int j = i + 1; j < n; j++) {
            auto [xj, yj] = m_nodePositions[j];
            float dx = xj - xi;
            float dy = yj - yi;
            float dist = std::hypot(dx, dy);

            // If nodes are close enough, they are neighbors. Connect them bi-directionally.
            if (dist >= minDist && dist <= maxDist) {
                float angleDeg = std::atan2(-dy, dx) * 180.f / PI;
                if (angleDeg < 0.f) angleDeg += 360.f;

                int sector = static_cast<int>((angleDeg + 30.f) / 60.f) % NUM_DIRECTIONS;
                static const int sectorToDir[NUM_DIRECTIONS] = { 1, 0, 5, 4, 3, 2 };

                int dirItoJ = sectorToDir[sector];
                int dirJtoI = (dirItoJ + 3) % NUM_DIRECTIONS;

                m_graph.addEdge(i, j, dirItoJ);
                m_graph.addEdge(j, i, dirJtoI);
            }
        }
    }
}

// Recalculates which nodes are powered, starting a BFS from the center node.
void Board::updateLighting() {
    BFS::run(m_graph, m_centerNodeId);
}

// Rotates a specific node in the given direction and updates lighting.
void Board::rotateNode(int nodeId, RotationDir dir) {
    m_graph.getNode(nodeId).rotate(dir);
    updateLighting(); 
}

// Returns true if every node on the board is lit.
bool Board::isLevelComplete() const {
    if (m_graph.nodeCount() == 0) return false;
    const auto& nodes = m_graph.getNodes();
    return std::all_of(nodes.begin(), nodes.end(), [](const Node& n) { return n.isLit(); });
}


// Getters
const Graph& Board::getGraph()        const { return m_graph; }
int          Board::getCenterNodeId() const { return m_centerNodeId; }
int          Board::nodeCount()       const { return m_graph.nodeCount(); }
const std::vector<int>& Board::getRowSizes() const { return m_rowSizes; }

std::pair<float, float> Board::getNodePosition(int id) const {
    if (id < 0 || id >= static_cast<int>(m_nodePositions.size()))
        throw std::out_of_range("Board::getNodePosition – invalid id");
    return m_nodePositions[id];
}

// Skips empty lines and lines starting with '#' to ignore comments in the level file.
void Board::skipComments(std::ifstream& file) {
    std::string line;
    while (file.good()) {
        int peeked = file.peek();
        if (peeked == EOF) break; 
        char c = static_cast<char>(peeked);
        if (c == '\n' || c == '\r' || c == ' ' || c == '\t') 
            file.get();
        else if (c == '#') 
            std::getline(file, line);
        else 
            break;
    }
}