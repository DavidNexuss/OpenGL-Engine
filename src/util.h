#pragma once
#include <glm/glm.hpp>
struct VRP
{
    glm::vec4 min,max;
    inline glm::vec4 center() const { return glm::vec4(glm::vec3(min + max) * 0.5f,1); }
};

namespace Util
{
    VRP getSceneVRP();
}