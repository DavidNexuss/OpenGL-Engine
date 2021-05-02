#include "light.h"
#include "material.h"
namespace Light
{
    bool flushUniforms = true;

    std::vector<glm::vec3> lightsPositions;
    std::vector<glm::vec3> lightsColor;

    LightID load(glm::vec3 pos,glm::vec3 color)
    {
        lightsPositions.push_back(pos);
        lightsColor.push_back(color);
        flushUniforms = true;
        return lightsPositions.size() - 1;
    }

    void flush()
    {
        Material& mat = MaterialLoader::materials[MaterialLoader::currentMaterial];
        if (flushUniforms && mat.isLightSensitive())
        {
            glUniform3fv(mat.uniforms[UNIFORM_LIGHTPOSITION],lightsPositions.size(),(GLfloat*)&lightsPositions[0]);
            glUniform3fv(mat.uniforms[UNIFORM_LIGHTCOLOR],lightsColor.size(),(GLfloat*)&lightsColor[0]);
            glUniform1i(mat.uniforms[UNIFORM_LIGHTCOUNT],lightsColor.size());
            
            REGISTER_LIGHT_FLUSH();
        }
    }
};
