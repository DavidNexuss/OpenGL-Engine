#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <engine_component.h>

using LightID = size_t;
using LightComponent = EngineComponent;

/**
 * @brief Manages pointlights.
 * Lights are stored in continous memory using a vector of positions and a vector of colors, this allows us to send all data directly to shaders using
 * a uniform vector. In consequence Lights are managed using an ECS aproach.
 */

class LightLoader
{
    constexpr static glm::vec3 GLOBAL_LIGHT = glm::vec3(std::numeric_limits<float>::infinity());

    bool flushUniforms = true;
    public:

    std::vector<glm::vec3> lightsPositions;
    std::vector<glm::vec3> lightsColor;
    std::vector<LightComponent> lightsComponent;

    LightID add(glm::vec3 pos,glm::vec3 color);

    void flush();
    void flushClean();
};

namespace Loader {
    extern LightLoader lights;
}