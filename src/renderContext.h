#pragma once
#include <glm/glm.hpp>
#include "camera.h"
#include "model.h"
#include "material.h"
#include "standard.h"
#include "material_instance.h"
#include "world_material.h"
#include "structures/pack.h"
#include <stack>
#include <vector>

struct RenderContext {
    size_t currentFrame = 1;

    CameraID currentCamera = 0;
    MaterialID currentMaterial;
    MeshID currentMesh;

    WorldMaterial* currentWorldMaterial = nullptr;

    void useMaterial(MaterialID materialID);
    void useMesh(MeshID meshID);
    void useCamera(CameraID cameraID);
    void useMaterialInstance(MaterialInstanceID materialInstance);
    void useWorldMaterial(WorldMaterial* worldMaterial);

    void registerFrame();
};

namespace Renderer {
    extern RenderContext renderContext;
}
