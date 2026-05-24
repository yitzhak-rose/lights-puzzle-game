#pragma once
#include "Constants.h"
#include <vector>


// Represents a single bulb on the hexagonal board.
class Node {

    public:
        // Initializes the node with its unique ID and original arm layout.
        Node(int id, const std::vector<bool>& baseArms);


        // Returns the unique identifier of the node.
        int getId() const;

        // Returns the current rotation step (0–5), where each step represents 60°.
        int getRotation() const;

        // Returns whether the node is currently receiving power and lit.
        bool isLit() const;

        // True if this node currently has an arm in globalDirection (after rotation).
        bool hasArm(int globalDirection) const;


        // Updates the node's rotation based on the given direction (CW/CCW).
        void rotate(RotationDir dir);

        // Sets the lighting status of the node.
        void setLit(bool lit);

    private:
        int m_id;         // Unique ID
        int m_rotation;   // Current rotation [0, 5]
        std::vector<bool> m_baseArms;   // Original arm configuration (immutable)
        bool m_isLit;      // Current state (on/off)
};