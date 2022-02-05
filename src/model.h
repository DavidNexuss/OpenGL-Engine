#pragma once
#include "mesh.h"
#include "material.h"
#include "material_instance.h"
#include "structures/storage.h"
#include "engine_component.h"
#include "standard.h"
#include <string>

struct Model : public EngineComponent
{
    MeshID meshID = -1;
    MaterialID materialID = -1;
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

    void draw();
    inline void process() { }

    inline bool operator<(const Model& model) const {
        return materialID < model.materialID || 
            (materialID == model.materialID && (materialInstanceID < model.materialInstanceID || 
                (materialInstanceID == model.materialInstanceID && meshID < model.meshID)));
    }

    inline bool valid() const {
        return meshID != -1 && materialID != -1;
    }
};


namespace Loader {
    extern sorted_storage<Model> models;
}

using ModelID = storage_pointer<sorted_storage<Model>,Loader::models>;
