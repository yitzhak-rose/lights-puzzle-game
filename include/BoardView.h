


/*// --- Gameplay and Electricity Logic ---

void Board::rotateNode(int nodeId, RotationDir dir) {
    m_graph.getNode(nodeId).rotate(dir);
}

void Board::updateLighting() {
    // 1. Turn off all lights on the board first
    m_graph.resetLighting();

    // 2. Create a queue for BFS. The power source turns on and goes in first.
    std::queue<int> q;
    m_graph.getNode(m_centerNodeId).setLit(true);
    q.push(m_centerNodeId);

    // 3. Keep processing as long as there are lit nodes to check
    while (!q.empty()) {
        int currentId = q.front();
        q.pop();

        const Node& current = m_graph.getNode(currentId);

        // Check all neighbors of the current node
        for (const Graph::Edge& edge : m_graph.getEdges(currentId)) {
            Node& neighbour = m_graph.getNode(edge.toNodeId);

            // If the neighbor is already lit, no need to light it again
            if (neighbour.isLit()) continue;

            int dirToNeighbour = edge.direction;
            int dirBack = (dirToNeighbour + 3) % NUM_DIRECTIONS;

            // Power flows *only* if I have an arm pointing to them, and they have an arm pointing back
            if (current.hasArm(dirToNeighbour) && neighbour.hasArm(dirBack)) {
                neighbour.setLit(true);
                q.push(edge.toNodeId); // Push to queue so power continues flowing from it
            }
        }
    }
}

bool Board::isLevelComplete() const {
    // Scan the entire graph - a single unlit node means the game is not over
    for (int id = 0; id < m_graph.nodeCount(); ++id)
        if (!m_graph.getNode(id).isLit()) return false;
    return true;
}


// --- Input Helpers ---

int Board::getClickedNodeId(float mouseX, float mouseY, float hitRadius) const {
    float radiusSq = hitRadius * hitRadius; // Squared comparison avoids expensive sqrt()
    for (int id = 0; id < static_cast<int>(m_nodePositions.size()); ++id) {
        auto [nx, ny] = m_nodePositions[id];
        if (distSq(mouseX, mouseY, nx, ny) <= radiusSq)
            return id;
    }
    return -1; // No hexagon was clicked
}


// --- Read-only Accessors ---

const Graph& Board::getGraph()        const { return m_graph; }
int          Board::getCenterNodeId() const { return m_centerNodeId; }
int          Board::nodeCount()       const { return m_graph.nodeCount(); }

std::pair<float, float> Board::getNodePosition(int id) const {
    if (id < 0 || id >= static_cast<int>(m_nodePositions.size()))
        throw std::out_of_range("Board::getNodePosition – invalid id");
    return m_nodePositions[id];
}


// --- Private Under-the-Hood Helpers ---

void Board::skipComments(std::ifstream& file) {
    // Skips any line that starts with a hash or is completely empty
    while (file.peek() == '#' || file.peek() == '\n' || file.peek() == '\r') {
        std::string line;
        std::getline(file, line);
    }
}

}*/