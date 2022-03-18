#include "renderer.h"
#include "model.h"
#include "viewport.h"
#include "light.h"
#include "world_material.h"
#include <mesh/primitiveMesh.h>
#include <iostream>
#include "render_camera.h"

RenderConfiguration Renderer::currentConfiguration;

namespace Renderer
{
    RenderCameraID mainRenderCamera;
    ModelID skyModel;

    int currentFrame = 1;
    CameraID currentCamera = 0;
    MaterialID currentMaterial;
    MeshID currentMesh;

    WorldMaterial* currentWorldMaterial = nullptr;

    bool materialOverride = false;

    void useMaterial(MaterialID materialID)
    {
        if(materialOverride) return;

        if(currentMaterial != materialID) {
            currentMaterial = materialID;
            currentMaterial->bind();
            REGISTER_MATERIAL_SWAP();
        }
    
        if (Loader::materials.updateForFrame(materialID,currentFrame)) {
            Loader::lights.flush(currentMaterial);
			
			//TODO: Should refactor this to enable multiple uniforms subsystems
            if(currentCamera.valid()) 
                currentCamera->bind(currentMaterial);        
            
            if(currentWorldMaterial != nullptr)
                currentWorldMaterial->bind(currentMaterial);
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
    
    void useCamera(CameraID cameraID) {
        
		if(cameraID != currentCamera) {
            currentCamera = cameraID;
            currentCamera->bind(currentMaterial);
        }
    }   
    
    void useMaterialInstance(MaterialInstanceID instanceID) {
        currentMaterial->useInstance(instanceID);
    }
    
    void useWorldMaterial(WorldMaterial* worldMaterial) {
        currentWorldMaterial = worldMaterial;
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
        if(currentCamera.valid()) currentCamera->update();
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
