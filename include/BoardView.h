




void Board::rotateNode(int nodeId, RotationDir dir) {
    m_graph.getNode(nodeId).rotate(dir);
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

