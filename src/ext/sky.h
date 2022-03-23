#pragma once
#include "model.h"
#include "world_material.h"

namespace SkyCreator {
    ModelID createSkyBox();
}
struct SkyWorldMaterial : public WorldMaterial {
    Texture skyTexture = -1;
    virtual void bind(MaterialID currentMaterial);
};