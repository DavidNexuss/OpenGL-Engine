#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <mesh.h>

struct PrimitiveMesh
{
    const static int attribVertex = 0;
    const static int attribNormal = 3;
    const static int attribUV = 2;

    struct MeshAttribute{
        std::vector<glm::vec3> buffer;
        int size;
        int attributeid;

        MeshAttribute(int n) : buffer(n) { }
    };

    std::vector<MeshAttribute> attributes;
    std::vector<size_t> indices;

    std::vector<float> interleavedData(int& vertexSize) const;
    
    inline bool generated() const { return attributes.size(); }   
    Mesh generateMesh();
    virtual void create() = 0;
};