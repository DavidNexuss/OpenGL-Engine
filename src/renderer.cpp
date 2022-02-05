#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "viewport.h"
#include "light.h"
#include "world_material.h"
#include <mesh/primitiveMesh.h>
#include <iostream>
#include "render_node.h"

RenderConfiguration Renderer::currentConfiguration;

namespace Renderer
{
    size_t currentFrame = 1;
    size_t currentMaterial = Standard::engineInvalid;
    size_t currentMesh = Standard::engineInvalid;

    ModelID skyModel = -1;
    WorldMaterial worldMaterial;

    RenderNode* renderPipeline = nullptr;

    void useMaterial(MaterialID materialID)
    {
        if(currentMaterial != materialID) {
            currentMaterial = materialID;
            Loader::materials[currentMaterial].bind(worldMaterial);
            REGISTER_MATERIAL_SWAP();
        }

        if (Loader::materials.updateForFrame(materialID,currentFrame)) {
            Scene::flush();
            Light::flush();
        }
    }

    void useMesh(MeshID meshID)
    {
        if (meshID != currentMesh)
        {
            currentMesh = meshID;
            glBindVertexArray(Loader::meshes[Renderer::currentMesh].vao);
            REGISTER_MESH_SWAP();
        }
    }
    
    void useMaterialInstance(MaterialInstanceID instanceID)
    {
        Loader::materials[currentMaterial].useInstance(instanceID);
    }
    
    void setRenderPipeline(RenderNode* rootNode){
        renderPipeline = rootNode;
        if(RenderNode::screenQuad == Standard::engineInvalid)
        {
            RenderNode::screenQuad = Loader::meshes.add(PrimitiveMesh::generateFromBuffers({
                {Standard::aPosition,2,{
                    -1.0,1.0,
                     1.0,1.0,
                    -1.0,-1.0,

                     1.0,1.0,
                     1.0,-1.0,
                     -1.0,-1.0,

                }}
            }));
        }
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
        glCullFace(GL_BACK);
        Model& sky = Loader::models[skyModel];
        if(sky.valid() && !currentConfiguration.skipSkybox)
            sky.draw();

        auto models = Loader::models.getSortedView();

        //Render world
        for(size_t i = 0; i < models.size(); i++) {
            if(!models[i].enabled) continue;

            models[i].process();
            models[i].draw();
        }
    }
    void render()
    {
        currentFrame++;

        glClearColor(currentConfiguration.clearColor.x, currentConfiguration.clearColor.y, currentConfiguration.clearColor.z,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Scene::time += 0.1;
        Scene::update();
        
        if(renderPipeline != nullptr) {
            renderPipeline->render(Viewport::screenWidth,Viewport::screenHeight);
        } else renderPass();

        Light::flushUniforms = false;
    }
};
