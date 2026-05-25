#pragma once
#include "Graph.h"
#include <string>
#include <vector>
#include <utility>

class Board {
public:
    // Parses the level file to build the graph and set logical node positions.
    void loadFromFile(const std::string& filename);

    // Rotates a specific node in the given direction.
    //void rotateNode(int nodeId, RotationDir dir);

    // Runs BFS from the center node to update the lighting state of all nodes.
    void updateLighting();

    // Returns true if all nodes in the graph are currently lit.
    //bool isLevelComplete() const;

    // Returns the ID of the clicked node based on coordinates, or -1 if missed.
    //int getClickedNodeId(float mouseX, float mouseY, float hitRadius) const;

    // Returns a read-only reference to the underlying logic graph.
    //const Graph& getGraph() const;

    // Returns the ID of the power source (center) node.
    //int getCenterNodeId() const;

    // Returns the logical X and Y position of a specific node.
    //std::pair<float, float> getNodePosition(int id) const;

    // Returns the total number of nodes currently on the board.
    //int nodeCount() const;

private:
    // Returns the row sizes read from the file; also sets m_centerNodeId.
    std::vector<int> readLevelInfo(std::ifstream& file);

    // Fills m_nodePositions from the row sizes.
    void computeNodePositions(const std::vector<int>& rowSizes);

    // Reads NODE lines and populates m_graph.
    void parseNodes(std::ifstream& file);

    // Builds edges between neighbours at runtime (bonus).
    void buildAdjacency();

    static void skipComments(std::ifstream& file);

    Graph m_graph;
    int   m_centerNodeId = 0;
    std::vector<std::pair<float, float>> m_nodePositions;
};