#pragma once
#include "texture.h"
#include "material.h"
#include "camera.h"

struct WorldMaterial {
    Texture skyTexture = -1;
    void bind(MaterialID currentMaterial);
};
