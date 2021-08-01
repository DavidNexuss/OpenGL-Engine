#pragma once
#include <vector>
#include "core.h"
#include "mesh_region.h"

/**
 * @brief class for programmatic mesh generation
 * 
 */

class MeshBuffer
{

    size_t vertexCount;
    size_t vertexStride;

    std::vector<GLfloat> meshBuffer;
    std::vector<MeshRegion> regions;

    size_t allocateRegion();

    public:
    MeshBuffer(const GLfloat* raw_meshBuffer,int _vertexCount,int _vertexStride);
    
    void generateNormals();
    void generateTangents();

    void bindRegions() const;
    
    inline void bufferData() const {
        glBufferData(GL_ARRAY_BUFFER, meshBuffer.size() * sizeof(GLfloat), (const GLfloat*)&meshBuffer[0], GL_STATIC_DRAW);
    }
    inline const GLfloat* raw() const {
        return (const GLfloat*)&meshBuffer[0];
    }
    inline size_t getVertexStride() const {
        return vertexStride;
    }
};
