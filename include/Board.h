#pragma once
#include "Graph.h"
#include <string>
#include <vector>
#include <utility>
#include <fstream> 

class Board {
public:
    // Parses the level file to build the graph and set logical node positions.
    void loadFromFile(const std::string& filename);

    // Rotates a node and automatically updates lighting.
    void rotateNode(int nodeId, RotationDir dir);

    // Runs BFS from center to update node power states.
    void updateLighting();

    // Checks if all nodes in the graph are powered.
    bool isLevelComplete() const;

    // Finds the closest node to mouse click radius.
    int getClickedNodeId(float mouseX, float mouseY, float hitRadius) const;

    // Returns a read-only reference to the graph.
    const Graph& getGraph() const;

    // Returns the ID of the central power source.
    int getCenterNodeId() const;

    // Returns (x, y) coordinates for a specific node.
    std::pair<float, float> getNodePosition(int id) const;

    // Returns the total number of nodes in the board.
    int nodeCount() const;

private:
    // Reads rows and center ID from file.
    std::vector<int> readLevelInfo(std::ifstream& file);

    // Fetches an integer value for a given keyword.
    int getData(std::ifstream& file, const std::string& expectedKeyword);

    // Reads the number of nodes in each row.
    std::vector<int> getRowSizes(std::ifstream& file, int totalRows);

    // Calculates screen positions for all nodes.
    void computeNodePositions(const std::vector<int>& rowSizes);

    // Reads arm configurations for every node from file.
    void parseNodes(std::ifstream& file);

    // Connects neighboring nodes based on distance and angle.
    void buildAdjacency();

    // Skips hashtags, empty lines, and trailing spaces.
    void skipComments(std::ifstream& file);

    Graph m_graph;
    int m_centerNodeId = 0;
    // List of (x, y) positions for rendering and clicks.
    std::vector<std::pair<float, float>> m_nodePositions;
};