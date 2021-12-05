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
    WorldMaterial worldMaterial;
    RenderNode* renderPipeline = nullptr;

    void useMaterial(MaterialID materialID)
    {
        if(MaterialLoader::currentMaterial != materialID)
        {
            MaterialLoader::currentMaterial = materialID;
            MaterialLoader::materials[MaterialLoader::currentMaterial].bind(worldMaterial);
            REGISTER_MATERIAL_SWAP();
        }

        if (MaterialLoader::usedMaterials[MaterialLoader::currentMaterial] != currentFrame)
        {
            MaterialLoader::usedMaterials[MaterialLoader::currentMaterial] = currentFrame;
            Scene::flush();
            Light::flush();
        }
    }

    void useMesh(MeshID meshID)
    {
        if (meshID != MeshLoader::currentMesh)
        {
            MeshLoader::currentMesh = meshID;
            glBindVertexArray(MeshLoader::meshes[MeshLoader::currentMesh].vao);
            REGISTER_MESH_SWAP();
        }
    }
    
    void useMaterialInstance(MaterialInstanceID instanceID)
    {
        MaterialLoader::materials[MaterialLoader::currentMaterial].useInstance(instanceID);
    }
    
    void setRenderPipeline(RenderNode* rootNode){
        renderPipeline = rootNode;
        if(RenderNode::screenQuad == -1)
        {
            RenderNode::screenQuad = MeshLoader::loadMesh(PrimitiveMesh::generateFromBuffers({
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
        auto& models = ModelLoader::native();
        if(!Standard::is_invalid(worldMaterial.skyBox.model) && !currentConfiguration.skipSkybox)
                ModelLoader::models[worldMaterial.skyBox.model].draw();

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
        /*
        if(postprocess.material != Standard::invalidId)
        {
            postprocess.screenBuffer.begin(Viewport::screenWidth,Viewport::screenHeight);
            
            glCullFace(GL_BACK);
            //Render skybox first
            if(worldMaterial.skyBox.model != Standard::invalidId)
                ModelLoader::models[worldMaterial.skyBox.model].draw();

            //Render world
            for(size_t i = 0; i < models.size(); i++) {
                if(!models[i].enabled) continue;

                models[i].process();
                models[i].draw();
            }
            postprocess.screenBuffer.end();

            glCullFace(GL_FRONT);
            useMaterial(postprocess.material);
            useMesh(Posprocess::screenQuad);
            MaterialLoader::materials[postprocess.material].useScreenAttachments(postprocess.screenBuffer);
            drawMesh(false);

        }
        else{
            //Render skybox first
            if(worldMaterial.skyBox.model != Standard::invalidId)
                ModelLoader::models[worldMaterial.skyBox.model].draw();

            //Render world
            for(size_t i = 0; i < models.size(); i++) {
                if(!models[i].enabled) continue;

                models[i].process();
                models[i].draw();
            }
        }*/
        Light::flushUniforms = false;
    }
};
