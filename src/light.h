#include <vector>
#include <glm/glm.hpp>
#include <engine_component.h>

using LightID = size_t;
using LightComponent = EngineComponent;
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
