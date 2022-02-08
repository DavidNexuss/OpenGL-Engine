#pragma once
#include <cstddef>
#include <glm/glm.hpp>
#include "standard.h"
#include "mesh.h"
#include "framebuffer.h"
#include "sky.h"
#include "render_camera.h"
#include "mesh.h"
#include "material.h"


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
    extern MaterialID currentMaterial;
    extern MeshID currentMesh;
    
    extern ModelID skyModel;
    extern WorldMaterial worldMaterial;
    extern RenderCameraID mainRenderCamera;
    
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
}

#define UNIFORMS(x) Loader::materials[Renderer::currentMaterial].uniforms[x]
