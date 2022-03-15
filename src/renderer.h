#pragma once
#include <cstddef>
#include <glm/glm.hpp>
#include "standard.h"
#include "mesh.h"
#include "framebuffer.h"
#include "render_camera.h"
#include "mesh.h"
#include "material.h"
#include "world_material.h"
#include "model.h"

struct RenderConfiguration
{
    int mssaLevel;
    bool wireRendering;
    glm::vec3 clearColor;
};

namespace Renderer
{   
    extern RenderCameraID mainRenderCamera;
    extern RenderConfiguration currentConfiguration;

    extern int currentFrame;
    extern CameraID currentCamera;
    extern MaterialID currentMaterial;
    extern MeshID currentMesh;

    extern WorldMaterial* currentWorldMaterial;

    void useMaterial(MaterialID materialID);
    void useMesh(MeshID meshID);
    void useCamera(CameraID cameraID);
    void useMaterialInstance(MaterialInstanceID materialInstance);
    void useWorldMaterial(WorldMaterial* worldMaterial);

    void registerFrame();

    void configureRenderer(const RenderConfiguration& config);
    void render();
    void renderPass();

    inline void drawMesh() {
        if(currentMesh->indexed) 
            glDrawElements(GL_TRIANGLES,currentMesh->vertexCount,Standard::meshIndexGL,(void*)0);
        else 
            glDrawArrays(GL_TRIANGLES,0,currentMesh->vertexCount);
    }
}

#define UNIFORMS(x) Renderer::currentMaterial->uniforms[x]
