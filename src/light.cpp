#include "light.h"
#include "material.h"
#include <iostream>


size_t LightLoader::add(glm::vec3 pos,glm::vec3 color)
{
    lightsPositions.push_back(pos);
    lightsColor.push_back(color);
    lightsComponent.emplace_back("");
    flushUniforms = true;
    return lightsPositions.size() - 1;
}

void LightLoader::flush(MaterialID mat)
{
    if (flushUniforms && mat->isLightSensitive()) {
        glUniform3fv(mat->uniforms[Standard::uLightPosition],lightsPositions.size(),(GLfloat*)&lightsPositions[0]);
        glUniform3fv(mat->uniforms[Standard::uLightColor],lightsColor.size(),(GLfloat*)&lightsColor[0]);
        glUniform1i(mat->uniforms[Standard::uLightCount],lightsColor.size());
        
        REGISTER_LIGHT_FLUSH();
    }
}

void LightLoader::flushClean() {
    flushUniforms = false;
}

namespace Loader {
    LightLoader lights;
}
