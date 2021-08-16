#include "renderer.h"
#include "scene.h"
#include "model.h"
#include "viewport.h"
#include "light.h"
#include "world_material.h"
#include <mesh/primitiveMesh.h>

struct Posprocess
{
    FrameBuffer screenBuffer;
    MaterialID material = -1;
    static MeshID screenQuad;
};

MeshID Posprocess::screenQuad = -1;
namespace Renderer
{
    size_t currentFrame = 1;
    WorldMaterial worldMaterial;
    Posprocess postprocess;

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
    
    void addPostProcess(MaterialID mat)
    {
        if(Posprocess::screenQuad == Standard::invalidId)
        {
            Posprocess::screenQuad = MeshLoader::loadMesh(PrimitiveMesh::generateFromBuffers({
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
        postprocess.screenBuffer.setAttachmentCount(1);
        postprocess.material = mat;
    }

    void configureRenderer(const RenderConfiguration& config)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);  
        glEnable(GL_DEPTH_TEST); 
        if(config.useMssa)glEnable(GL_MULTISAMPLE);
    }

    void render()
    {
        auto& models = ModelLoader::models.native();
        currentFrame++;

        glClearColor(0.0,0.0,0.0,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Scene::time += 0.1;
        Scene::update();
        
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
        }
        Light::flushUniforms = false;
    }
};
