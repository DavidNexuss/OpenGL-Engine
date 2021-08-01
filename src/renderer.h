#pragma once
#include <cstddef>
#include "window.h"
#include "mesh.h"
#include "basics.h"
#include "world_material.h"

namespace Renderer
{
    extern size_t currentFrame;
    extern WorldMaterial worldMaterial;

    void useMaterial(MaterialID id);
    void useMaterialInstance(MaterialInstanceID id);
    void useMesh(MeshID id);
    
    void configureRenderer();
    void render();

    inline void drawMesh() {
        glDrawArrays(GL_TRIANGLES,0,MeshLoader::meshes[MeshLoader::currentMesh].vertexCount);
    }
}