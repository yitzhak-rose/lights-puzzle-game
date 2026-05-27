#pragma once
#include "Graph.h"

// Responsible for propagating power through the board using BFS.
// Starting from the center node, it lights up every node that is
// connected via matching arms (both sides must point toward each other).
class BFS {
public:
    // Runs the BFS algorithm on the given graph, starting from centerNodeId.
    // Updates the isLit state of every node in the graph.
    static void run(Graph& graph, int centerNodeId);
};