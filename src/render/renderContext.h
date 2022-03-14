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

};

namespace Renderer {
    extern RenderContext renderContext;
}
