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
    MeshID meshID;
    MaterialID materialID;
    MaterialInstanceID materialInstanceID;

    glm::mat4 transformMatrix = glm::mat4(1.0);
    glm::mat3 normalMatrix;

    bool enabled = true;
    bool depthMask = false;
    bool cullFrontFace = false;

    static bool lastcullFrontFace;

    Model() { }
    Model(MeshID _meshID,MaterialID _materialID = 0) : meshID(_meshID), materialID(_materialID) { }
    Model(MeshID _meshID,MaterialID _materialID, MaterialInstanceID _materialInstanceID) : meshID(_meshID), materialID(_materialID), materialInstanceID(_materialInstanceID) { }

    void draw();
    inline void process() { }

    inline bool operator<(const Model& model) const {
        return materialID < model.materialID || 
            (materialID == model.materialID && (materialInstanceID < model.materialInstanceID || 
                (materialInstanceID == model.materialInstanceID && meshID < model.meshID)));
    }

    inline bool ready() const {
        return meshID.valid() && materialID.valid();
    }
};


namespace Loader {
    extern sorted_storage<Model> models;
}

using ModelID = storage_pointer<sorted_storage<Model>,Loader::models>;
