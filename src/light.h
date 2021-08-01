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

namespace Light
{
    const static size_t maxLights = 6;
    extern bool flushUniforms;

    extern std::vector<glm::vec3> lightsPositions;
    extern std::vector<glm::vec3> lightsColor;
    extern std::vector<LightComponent> lightsComponent;

    LightID load(glm::vec3 pos,glm::vec3 color);

    void flush();
};
