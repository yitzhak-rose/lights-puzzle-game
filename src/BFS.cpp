#include "BFS.h"
#include "Constants.h"
#include <queue>

// Runs BFS from the center node and updates the lighting state of all nodes.
void BFS::run(Graph& graph, int centerNodeId) {
    // 1. Turn off all lights on the board first
    graph.resetLighting();

    // 2. The power source turns on and goes in first
    std::queue<int> q;
    graph.getNode(centerNodeId).setLit(true);
    q.push(centerNodeId);

    // 3. Keep processing as long as there are lit nodes to check
    while (!q.empty()) {
        int currentId = q.front();
        q.pop();

        const Node& current = graph.getNode(currentId);

        // Check all neighbors of the current node
        for (const Graph::Edge& edge : graph.getEdges(currentId)) {
            Node& neighbour = graph.getNode(edge.toNodeId);

            // If the neighbor is already lit, no need to light it again
            if (neighbour.isLit()) continue;

            int dirToNeighbour = edge.direction;
            int dirBack = (dirToNeighbour + 3) % NUM_DIRECTIONS;

            // Power flows only if both sides have arms pointing toward each other
            if (current.hasArm(dirToNeighbour) && neighbour.hasArm(dirBack)) {
                neighbour.setLit(true);
                q.push(edge.toNodeId);
            }
        }
    }
}