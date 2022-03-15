#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <mesh.h>
#include <vector>
#include "standard.h"

struct PrimitiveMesh
{
    struct MeshAttribute{
        int size;
        int attributeid;
        std::vector<glm::vec3> buffer;

        MeshAttribute(int n = 0) : buffer(n) { }
        MeshAttribute(size_t capacity,int _size,int _attributeid) : size(_size), attributeid(_attributeid) { buffer.reserve(capacity); }
    };

    std::vector<MeshAttribute> attributes;
    std::vector<Standard::meshIndex> indices;

    std::vector<float> interleavedData(int& vertexSize) const;
    
    inline bool generated() const { return attributes.size(); }   
    inline void clear() {
        attributes.clear();
        indices.clear();
    }
    int initiliazeAttribute(size_t count,int size,int attributeid);
    virtual void create() = 0;
    Mesh generateMesh();

    
    
    static Mesh generateFromBuffer(const std::vector<float>& buffer,
                        const std::vector<std::pair<int /*attribute ptr*/ ,int /* attribute size*/ >>& attributes);

    struct MeshGenerationBuffer
    {
        int attributeid;
        int size;
        std::vector<float> buffer;
    };
    
    static std::vector<float> interleavedData(const std::vector<MeshGenerationBuffer>& buffers);
    static Mesh generateFromBuffers(const std::vector<MeshGenerationBuffer>& buffers);
    
    static std::vector<Standard::meshIndex> triangulateMesh(const std::vector<std::pair<int,int>>& edges);
};
