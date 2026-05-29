#include "logic/Node.h"

// Constructor: Initializes the node with its ID and base arms. Rotation starts at 0.
Node::Node(int id, const std::vector<bool>& baseArms)
    : m_id(id)
    , m_rotation(0)
    , m_baseArms(baseArms)
    , m_isLit(false)
{
}


// Returns the node's unique ID.
int Node::getId() const {
    return m_id;
}

// Returns current rotation state.
int Node::getRotation() const {
    return m_rotation;
}

// Returns true if the node is lit.
bool Node::isLit() const {
    return m_isLit;
}

// Checks if the rotated node has an arm in `globalDirection`.
// The math finds where this arm was in the original unrotated array.
bool Node::hasArm(int globalDirection) const {
    int baseIndex = ((globalDirection - m_rotation) % NUM_DIRECTIONS + NUM_DIRECTIONS) % NUM_DIRECTIONS;
    return m_baseArms[baseIndex];
}


// Rotates the node one step (60°) CW or CCW, keeping it within 0-5.
void Node::rotate(RotationDir dir) {
    if (dir == RotationDir::Clockwise) {
        m_rotation = (m_rotation + 1) % NUM_DIRECTIONS;
    }
    else {
        m_rotation = (m_rotation + 5) % NUM_DIRECTIONS;  // +5 mod 6 is equivalent to -1 mod 6
    }
}

// Updates the lit state of the node.
void Node::setLit(bool lit) {
    m_isLit = lit;
}
