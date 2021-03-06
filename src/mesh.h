#pragma once
#include "core.h"
#include "mesh_buffer.h"
#include "structures/storage.h"
#include <memory>
#include <vector>

struct Mesh {
    GLuint vao,vbo,ebo;
    size_t vertexCount;
    bool indexed = false;

    std::shared_ptr<MeshBuffer> meshBuffer;

    Mesh(const GLfloat* raw_meshBuffer,size_t _vertexCount,size_t _vertexStride) : vertexCount(_vertexCount),
    meshBuffer(new MeshBuffer(raw_meshBuffer,_vertexCount,_vertexStride)) { }

    Mesh(GLuint _vao,GLuint _vbo,GLuint _ebo,size_t _vertexCount):
    vao(_vao),vbo(_vbo),ebo(_ebo),vertexCount(_vertexCount) { }

    Mesh() { }
    inline const std::shared_ptr<MeshBuffer> getMeshBuffer() const { return meshBuffer; }

};

namespace Loader {
    extern storage<Mesh> meshes;
}

using MeshID = STORAGE_POINTER_TYPE(Loader::meshes);
