#include <vector>
#include <glm/glm.hpp>

using LightID = size_t;
namespace Light
{
    const static size_t maxLights = 6;
    extern bool flushUniforms;

    extern std::vector<glm::vec3> lightsPositions;
    extern std::vector<glm::vec3> lightsColor;

    LightID load(glm::vec3 pos,glm::vec3 color);

    void flush();
};
