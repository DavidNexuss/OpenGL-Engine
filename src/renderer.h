#pragma once
#include <cstddef>
#include "mesh.h"
#include "basics.h"
#include "world_material.h"

struct RenderConfiguration
{
    bool useMssa;
};

namespace Renderer
{
    extern size_t currentFrame;
    extern WorldMaterial worldMaterial;

    void useMaterial(MaterialID id);
    void useMaterialInstance(MaterialInstanceID id);
    void useMesh(MeshID id);
    
    void configureRenderer(const RenderConfiguration& config);
    void render();

    inline void drawMesh(bool indexed) {
        if(indexed) glDrawElements(GL_TRIANGLES,MeshLoader::meshes[MeshLoader::currentMesh].vertexCount,GL_UNSIGNED_INT,(void*)0);
        else glDrawArrays(GL_TRIANGLES,0,MeshLoader::meshes[MeshLoader::currentMesh].vertexCount);
    }
}