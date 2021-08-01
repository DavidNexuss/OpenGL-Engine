#pragma once
#include <cstddef>
#include "material.h"
#include "material_instance.h"
#include "mesh.h"
#include "window.h"

namespace Renderer
{
    extern size_t currentFrame;

    void useMaterial(MaterialID id);
    void useMaterialInstance(MaterialInstanceID id);
    void useMesh(MeshID id);

    inline void drawMesh()
    {
        glDrawArrays(GL_TRIANGLES,0,MeshLoader::meshes[MeshLoader::currentMesh].vertexCount);
    }
    
    void configureRenderer();
    void render();
}