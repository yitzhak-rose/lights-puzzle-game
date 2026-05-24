#include "Graph.h"
#include <stdexcept>
#include <string>


// Adds a node and ensures its ID matches its exact index in the vector
void Graph::addNode(const Node& node) {
    if (node.getId() != static_cast<int>(m_nodes.size())) {
        throw std::runtime_error(
            "Graph::addNode - node id " + std::to_string(node.getId()) +
            " does not match expected index " + std::to_string(m_nodes.size()) +
            ". Nodes must be added in order ");
    }

    m_nodes.push_back(node);
    m_adjacency.emplace_back();
}

// Retrieves a mutable node. Throws out_of_range if ID is invalid
Node& Graph::getNode(int id) {
    if (id < 0 || id >= static_cast<int>(m_nodes.size())) {
        throw std::out_of_range(
            "Graph::getNode - invalid id: " + std::to_string(id) +
            " (nodeCount = " + std::to_string(m_nodes.size()) + ")");
    }
    return m_nodes[id];
}

// Retrieves a read-only node. Throws out_of_range if ID is invalid
const Node& Graph::getNode(int id) const {
    if (id < 0 || id >= static_cast<int>(m_nodes.size())) {
        throw std::out_of_range(
            "Graph::getNode - invalid id: " + std::to_string(id) +
            " (nodeCount = " + std::to_string(m_nodes.size()) + ")");
    }
    return m_nodes[id];
}

// Returns current amount of nodes
int Graph::nodeCount() const {
    return static_cast<int>(m_nodes.size());
}


// מוסיף קשת בין נורה לשכנה שלה, כולל הכיוון
// Adds an edge connecting a node to its neighbour, including direction
void Graph::addEdge(int nodeId, int toNodeId, int direction) {
    if (nodeId < 0 || nodeId >= static_cast<int>(m_adjacency.size())) {
        throw std::out_of_range(
            "Graph::addEdge - source nodeId " + std::to_string(nodeId) + " is out of range");
    }
    m_adjacency[nodeId].push_back({ toNodeId, direction });
}

// Returns neighbor list. Throws out_of_range if ID is invalid (replaced s_empty fallback)
const std::vector<Graph::Edge>& Graph::getEdges(int nodeId) const {
    if (nodeId < 0 || nodeId >= static_cast<int>(m_adjacency.size())) {
        throw std::out_of_range(
            "Graph::getEdges - source nodeId " + std::to_string(nodeId) + " is out of range");
    }
    return m_adjacency[nodeId];
}


// Iterates through all nodes and turns off their lit state
void Graph::resetLighting() {
    for (Node& node : m_nodes) {
        node.setLit(false);
    }
}