#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <mesh.h>
#include <vector>
#include "standard.h"

struct PrimitiveMesh
{
    struct MeshAttribute{
        std::vector<glm::vec3> buffer;
        int size;
        int attributeid;

        MeshAttribute(int n = 0) : buffer(n) { }
    };

    std::vector<MeshAttribute> attributes;
    std::vector<Standard::meshIndex> indices;

    std::vector<float> interleavedData(int& vertexSize) const;
    
    inline bool generated() const { return attributes.size(); }   
    Mesh generateMesh();
    virtual void create() = 0;

    static Mesh generateFromBuffer(const std::vector<float>& buffer,
                        const std::vector<std::pair<int /*attribute ptr*/ ,int /* attribute size*/ >>& attributes);

    struct MeshGenerationBuffer
    {
        int attributeid;
        int size;
        std::vector<float> buffer;
    };
    
    static std::vector<float> interleavedData(const std::vector<MeshGenerationBuffer>& buffers);
    static Mesh generateFromBuffers(const std::vector<MeshGenerationBuffer>& buffers,int genrationFlags = 0);
    
    static std::vector<Standard::meshIndex> triangulateMesh(const std::vector<std::pair<int,int>>& edges);
};