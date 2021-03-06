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
#include "ext/globalWorldMaterial.h"
#include "activeMaterial.h"
#include <vector>

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
    extern ActiveMaterial currentMaterial;
    extern MeshID currentMesh;
    extern bool materialOverride;

    extern std::vector<WorldMaterial*> worldMaterials;
    extern GlobalWorldMaterial* globalWorldMaterial;
    
    void useMaterial(MaterialID materialID);
    void useMesh(MeshID meshID);
    void useMaterialInstance(MaterialInstanceID materialInstance);

    void addWorldMaterial(WorldMaterial* worldMaterial);
    void useWorldMaterial(Standard::WorldMaterialAspect aspect, WorldMaterial* worldMaterial);
    
    void overrideMaterial(MaterialID overrideMaterial);
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
