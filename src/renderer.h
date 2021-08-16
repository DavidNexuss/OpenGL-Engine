#pragma once
#include <cstddef>
#include "standard.h"
#include "mesh.h"
#include "basics.h"
#include "world_material.h"
#include "framebuffer.h"

struct RenderConfiguration
{
    bool useMssa;
};

namespace Renderer
{
    extern size_t currentFrame;
    extern WorldMaterial worldMaterial;;

    void useMaterial(MaterialID id);
    void useMaterialInstance(MaterialInstanceID id);
    void useMesh(MeshID id);
    
    void configureRenderer(const RenderConfiguration& config);
    void render();

    inline void drawMesh(bool indexed) {
        if(indexed) 
            glDrawElements(GL_TRIANGLES,MeshLoader::meshes[MeshLoader::currentMesh].vertexCount,Standard::meshIndexGL,(void*)0);
        else 
            glDrawArrays(GL_TRIANGLES,0,MeshLoader::meshes[MeshLoader::currentMesh].vertexCount);
    }

    void addPostProcess(MaterialID mat);
}