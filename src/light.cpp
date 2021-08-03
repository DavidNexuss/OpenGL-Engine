#include "light.h"
#include "material.h"
namespace Light
{
    bool flushUniforms = true;

    std::vector<glm::vec3> lightsPositions;
    std::vector<glm::vec3> lightsColor;
    std::vector<LightComponent> lightsComponent;

    std::string defaultLightName() {
        return "Light " + lightsComponent.size();
    }

    LightID load(glm::vec3 pos,glm::vec3 color)
    {
        lightsPositions.push_back(pos);
        lightsColor.push_back(color);
        lightsComponent.push_back({defaultLightName()});
        flushUniforms = true;
        return lightsPositions.size() - 1;
    }

    void flush()
    {
        Material& mat = MaterialLoader::materials[MaterialLoader::currentMaterial];
        if (flushUniforms && mat.isLightSensitive())
        {
            glUniform3fv(mat.uniforms[Standard::uLightPosition],lightsPositions.size(),(GLfloat*)&lightsPositions[0]);
            glUniform3fv(mat.uniforms[Standard::uLightColor],lightsColor.size(),(GLfloat*)&lightsColor[0]);
            glUniform1i(mat.uniforms[Standard::uLightCount],lightsColor.size());
            
            REGISTER_LIGHT_FLUSH();
        }
    }
};
