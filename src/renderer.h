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
#include "renderContext.h"


struct RenderConfiguration
{
    int mssaLevel;
    bool wireRendering;
    bool skipSkybox;
    glm::vec3 clearColor;
};

namespace Renderer
{   
    extern RenderCameraID mainRenderCamera;
    extern ModelID skyModel;
    extern RenderConfiguration currentConfiguration;

    void configureRenderer(const RenderConfiguration& config);
    void render();
    void renderPass();

    inline void drawMesh() {
        if(renderContext.currentMesh->indexed) 
            glDrawElements(GL_TRIANGLES,renderContext.currentMesh->vertexCount,Standard::meshIndexGL,(void*)0);
        else 
            glDrawArrays(GL_TRIANGLES,0,renderContext.currentMesh->vertexCount);
    }
}

#define UNIFORMS(x) Renderer::renderContext.currentMaterial->uniforms[x]
