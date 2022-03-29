#include "renderer.h"
#include "model.h"
#include "viewport.h"
#include "light.h"
#include "world_material.h"
#include <mesh/primitiveMesh.h>
#include <unordered_map>
#include <iostream>
#include "render_camera.h"

RenderConfiguration Renderer::currentConfiguration;

namespace Renderer
{
    RenderCameraID mainRenderCamera;
    ModelID skyModel;

    int currentFrame = 1;
    ActiveMaterial currentMaterial;
    MeshID currentMesh;

    std::vector<WorldMaterial*> worldMaterials;
    std::unordered_map<Standard::WorldMaterialAspect,WorldMaterial*> registeredWorldMaterials;
    bool materialOverride = false;
    GlobalWorldMaterial* globalWorldMaterial = new GlobalWorldMaterial();

    void useMaterial(MaterialID materialID)
    {
        if(materialOverride) return;
        currentMaterial.setActiveMaterial(materialID);

        if (Loader::materials.updateForFrame(materialID,currentFrame)) {
            Loader::lights.flush(currentMaterial);
            for(WorldMaterial* mat : worldMaterials) mat->bind(currentMaterial);
            for(auto it : registeredWorldMaterials) it.second->bind(currentMaterial);
        }
    
    }
    
    void useMesh(MeshID meshID)
    {
        if (meshID != currentMesh) {
            currentMesh = meshID;
            glBindVertexArray(currentMesh->vao);
            REGISTER_MESH_SWAP();
        }
    }
    
    void useMaterialInstance(MaterialInstanceID instanceID) {
        currentMaterial.bindInstance(instanceID);
    }
    
    void addWorldMaterial(WorldMaterial* worldMaterial) {
        worldMaterials.push_back(worldMaterial);
    }

    void useWorldMaterial(Standard::WorldMaterialAspect aspect, WorldMaterial* worldMaterial) {
        registeredWorldMaterials[aspect] = worldMaterial;
        if(currentMaterial.valid()) worldMaterial->bind(currentMaterial);
    }

    void overrideMaterial(MaterialID material) {
        if(material.valid()) {
            useMaterial(material);
            materialOverride = true;
        } else { 
            materialOverride = false;
        }
    }
    
    void registerFrame() {
        currentFrame++;
        for(WorldMaterial* worldMaterial : worldMaterials) 
            if(worldMaterial->needsFrameUpdate())worldMaterial->update();
        
        for(auto it : registeredWorldMaterials) 
            if(it.second->needsFrameUpdate()) it.second->update();

    }

    void configureRenderer(const RenderConfiguration& config)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);  
        glEnable(GL_DEPTH_TEST); 
        if(config.mssaLevel > 0)glEnable(GL_MULTISAMPLE);
        if(config.wireRendering) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }
        else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
        currentConfiguration = config;
    }

    void renderPass()
    {
        registerFrame();
        glCullFace(GL_BACK);
        auto models = Loader::models.getSortedView();
		
        //Render world
        for(size_t i = 0; i < models.size(); i++) {
            if(!models[i].enabled) continue;

            models[i].process();
            models[i].draw();
        }
    }
    void render() {
        glClearColor(currentConfiguration.clearColor.x, currentConfiguration.clearColor.y, currentConfiguration.clearColor.z,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
        if(mainRenderCamera.valid()) {
            mainRenderCamera->render(Viewport::screenWidth,Viewport::screenHeight);
        } else renderPass();

        Loader::lights.flushClean();
    }
};
