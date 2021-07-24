#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "viewport.h"
#include "light.h"

namespace Renderer
{
    size_t currentFrame = 1;

    void useMaterial(MaterialID materialID)
    {
        if(MaterialLoader::currentMaterial != materialID)
        {
            MaterialLoader::currentMaterial = materialID;
            MaterialLoader::materials[MaterialLoader::currentMaterial].bind();
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
    

    void configureRenderer()
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);  
        glEnable(GL_DEPTH_TEST);    
    }

    void render()
    {
        auto& models = ModelLoader::models.native();
        currentFrame++;

        glClearColor(0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Scene::time += 0.1;
        Scene::update();
//        skyBox.draw();

        for(size_t i = 0; i < models.size(); i++)
        {
            models[i].process();
            models[i].draw();
        }

        Light::flushUniforms = false;
    }
};
