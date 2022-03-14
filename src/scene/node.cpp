#include "node.h"

glm::mat4 Node::getCombinedMatrix() {
    if(isDirty()) {
        dirty = false;
        NodeID parentNodeID = NodeID(parentNode);
        return combinedMatrix = parentNodeID->getTransformMatrix() * transformMatrix;
    }
    return combinedMatrix;
}

bool Node::isDirty() {
    return dirty = dirty || Loader::nodes[parentNode].isDirty();
}

glm::mat4 Node::getTransformMatrix() const {
    return transformMatrix;
}

void Node::setTransformMatrix(glm::mat4 transform) {
    transformMatrix = transform;
    dirty = true;
}
