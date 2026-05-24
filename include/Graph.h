#pragma once
#include "Node.h"
#include <vector>

// Holds all Nodes and their adjacency information.
class Graph {
public:

    // Represents a connection to a neighbour: target ID and the direction towards it.
    struct Edge {
        int toNodeId;   // ID of the neighbouring node
        int direction;  // direction from current node toward that neighbour
    };


    // Adds a node.
    void addNode(const Node& node);

    // Returns a mutable reference to the node. 
    Node& getNode(int id);

    // Returns a read-only reference to the node. 
    const Node& getNode(int id) const;

    // Returns the total number of nodes in the graph.
    int nodeCount() const;


    // Adds ONE directed edge: nodeId → toNodeId in `direction`.
    void addEdge(int nodeId, int toNodeId, int direction);

    // Returns all edges leaving nodeId. Returns empty vector if no edges exist.
    const std::vector<Edge>& getEdges(int nodeId) const;

    // Sets every node's isLit flag to false.
    void resetLighting();

private:
    std::vector<Node> m_nodes;

    // m_adjacency[id] == list of edges leaving node `id`.
    std::vector<std::vector<Edge>> m_adjacency;
};