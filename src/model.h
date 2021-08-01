#pragma once
#include "mesh.h"

#include "material.h"
#include "material_instance.h"
#include "renderer.h"
#include "sorted_vector.h"
#include "id.h"
#include "engine_component.h"
#include <string>

struct Model : public EngineComponent
{
    MeshID meshID;
    MaterialID materialID;
    MaterialInstanceID materialInstanceID = -1;

    glm::mat4 transformMatrix;
    glm::mat3 normalMatrix;

    bool enabled = true;
    bool depthMask = false;
    bool cullFrontFace = false;

    static bool lastcullFrontFace;


    Model(MeshID _meshID,MaterialID _materialID = 0) : meshID(_meshID), materialID(_materialID), transformMatrix(1.0f) { }

    inline void draw()
    {
        if (cullFrontFace != lastcullFrontFace)
        {
            glCullFace(GL_FRONT + cullFrontFace);
            lastcullFrontFace = cullFrontFace;
        }  

        if (depthMask) glDepthMask(GL_FALSE);

        Renderer::useMaterial(materialID);
        if (materialInstanceID != ID::invalid_id) Renderer::useMaterialInstance(materialInstanceID);

        Renderer::useMesh(meshID);
        
        if(UNIFORMS(UNIFORM_TRANSFORM_MATRIX) != GL_INVALID_INDEX)
        {
            glUniformMatrix4fv(UNIFORMS(UNIFORM_TRANSFORM_MATRIX),1,false,&transformMatrix[0][0]);
        }

        if (UNIFORMS(UNIFORM_NORMAL_MATRIX) != GL_INVALID_INDEX)
        {
            normalMatrix = glm::transpose(glm::inverse(transformMatrix));
            glUniformMatrix3fv(UNIFORMS(UNIFORM_NORMAL_MATRIX),1,false,&normalMatrix[0][0]);
        }

        Renderer::drawMesh();

        if(depthMask) glDepthMask(GL_TRUE);

    }
    inline void process() 
    {

    }

    bool operator<(const Model& model)
    {
        return materialID < model.materialID || 
            (materialID == model.materialID && (materialInstanceID < model.materialInstanceID || 
                (materialInstanceID == model.materialInstanceID && meshID < model.meshID)));

    }
};
namespace ModelLoader
{
    extern sorted_vector<Model> models;
    static inline ModelID loadModel(const Model& model)
    {
        Model m = model;
        m.name = std::to_string(models.native().size());
        return models.push_back(m);
    }

    static inline Model& get(ModelID modelID) { return models[modelID]; }
    static inline const std::vector<Model>& native() { return models.native(); }
    
    ModelID createSkyBox(const std::vector<TextureData>& paths);
};
