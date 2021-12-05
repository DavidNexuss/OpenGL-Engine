#pragma once
#include "mesh.h"
#include "material.h"
#include "material_instance.h"
#include "renderer.h"
#include "structures/storage.h"
#include "engine_component.h"
#include "standard.h"
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

    Model() { }
    Model(MeshID _meshID,MaterialID _materialID = 0) : meshID(_meshID), materialID(_materialID), transformMatrix(1.0f) { }
    Model(MeshID _meshID,MaterialID _materialID, MaterialInstanceID _materialInstanceID) : meshID(_meshID), materialID(_materialID), materialInstanceID(_materialInstanceID), transformMatrix(1.0f) { }

    inline void draw()
    {
        if (cullFrontFace != lastcullFrontFace)
        {
            glCullFace(GL_FRONT + cullFrontFace);
            lastcullFrontFace = cullFrontFace;
        }  

        if (depthMask) glDepthMask(GL_FALSE);

        Renderer::useMaterial(materialID);
        if (!Standard::is_invalid(materialInstanceID)) Renderer::useMaterialInstance(materialInstanceID);

        Renderer::useMesh(meshID);
        
        if(UNIFORMS(Standard::uTransformMatrix) != GL_INVALID_INDEX)
        {
            glUniformMatrix4fv(UNIFORMS(Standard::uTransformMatrix),1,false,&transformMatrix[0][0]);
        }

        if (UNIFORMS(Standard::uNormalMatrix) != GL_INVALID_INDEX)
        {
            normalMatrix = glm::transpose(glm::inverse(transformMatrix));
            glUniformMatrix3fv(UNIFORMS(Standard::uNormalMatrix),1,false,&normalMatrix[0][0]);
        }

        Renderer::drawMesh(MeshLoader::meshes[MeshLoader::currentMesh].indexed);

        if(depthMask) glDepthMask(GL_TRUE);

    }
    inline void process() 
    {

    }

    bool operator<(const Model& model) const
    {
        return materialID < model.materialID || 
            (materialID == model.materialID && (materialInstanceID < model.materialInstanceID || 
                (materialInstanceID == model.materialInstanceID && meshID < model.meshID)));

    }
};

using ModelID = size_t;
namespace ModelLoader
{
    extern sorted_storage<Model> models;
    static inline ModelID loadModel(Model&& model)
    {
        model.name = std::to_string(models.internal().size());
        return models.push_back(std::move(model));
    }
    static inline ModelID loadModel(const Model& model) {
        return models.push_back(Model(model));
    }

    static inline std::vector<Model>& native() { return models.sorted_internal(); }
};
