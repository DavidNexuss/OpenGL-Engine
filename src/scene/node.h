#pragma once
#include <glm/glm.hpp>
#include <structures/storage.h>
#include <vector>

class Node
{
    size_t parentNode = 0;
    bool dirty = false;

    glm::mat4 transformMatrix = glm::mat4(1.0f);
    glm::mat4 combinedMatrix;
    
    bool isDirty();

    public:
    void setTransformMatrix(glm::mat4 transform);
    glm::mat4 getTransformMatrix() const;
    glm::mat4 getCombinedMatrix();
};

namespace Loader {
    extern storage<Node> nodes;
};

using NodeID = STORAGE_POINTER_TYPE(Loader::nodes);

struct NodeMember : public Node {
    NodeID parentNode = 0;
    glm::mat4 localTransform = glm::mat4(1.0f);

    inline glm::mat4 getCombinedMatrix() const {
        return parentNode->getCombinedMatrix() * localTransform;
    }
};
