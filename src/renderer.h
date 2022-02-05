#pragma once
#include <cstddef>
#include <glm/glm.hpp>
#include "standard.h"
#include "mesh.h"
#include "basics.h"
#include "framebuffer.h"
#include "sky.h"


struct RenderConfiguration
{
    int mssaLevel;
    bool wireRendering;
    bool skipSkybox;
    glm::vec3 clearColor;
};

class RenderNode;
namespace Renderer
{
    extern size_t currentFrame;
    extern size_t currentMaterial;
    extern size_t currentMesh;
    
    extern ModelID skyModel;
    extern WorldMaterial worldMaterial;
    
    extern RenderConfiguration currentConfiguration;

    void useMaterial(MaterialID id);
    void useMaterialInstance(MaterialInstanceID id);
    void useMesh(MeshID id);

    void configureRenderer(const RenderConfiguration& config);
    void render();
    void renderPass();

    inline void drawMesh(bool indexed) {
        if(indexed) 
            glDrawElements(GL_TRIANGLES,Loader::meshes[Renderer::currentMesh].vertexCount,Standard::meshIndexGL,(void*)0);
        else 
            glDrawArrays(GL_TRIANGLES,0,Loader::meshes[Renderer::currentMesh].vertexCount);
    }

    void setRenderPipeline(RenderNode* renderRoot);
}

#define UNIFORMS(x) Loader::materials[Renderer::currentMaterial].uniforms[x]
