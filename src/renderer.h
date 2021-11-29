#pragma once
#include <cstddef>
#include <glm/glm.hpp>
#include "standard.h"
#include "mesh.h"
#include "basics.h"
#include "world_material.h"
#include "framebuffer.h"


struct RenderNode : public FrameBuffer{
    static MeshID screenQuad;

    std::vector<RenderNode*> children;
    MaterialID material;

    RenderNode(MaterialID _material,int attachmentCount);
    RenderNode(int attachmentCount);

    void render(int screenWidth, int screenHeight);
    RenderNode* setChildren(std::vector<RenderNode*>&& children);
};

struct RenderConfiguration
{
    int mssaLevel;
    bool wireRendering;
    bool skipSkybox;
    glm::vec3 clearColor;
};

namespace Renderer
{
    extern size_t currentFrame;
    extern WorldMaterial worldMaterial;;
    extern RenderConfiguration currentConfiguration;

    void useMaterial(MaterialID id);
    void useMaterialInstance(MaterialInstanceID id);
    void useMesh(MeshID id);
    
    void configureRenderer(const RenderConfiguration& config);
    void render();
    void renderPass();

    inline void drawMesh(bool indexed) {
        if(indexed) 
            glDrawElements(GL_TRIANGLES,MeshLoader::meshes[MeshLoader::currentMesh].vertexCount,Standard::meshIndexGL,(void*)0);
        else 
            glDrawArrays(GL_TRIANGLES,0,MeshLoader::meshes[MeshLoader::currentMesh].vertexCount);
    }

    void setRenderPipeline(RenderNode* renderRoot);
}
