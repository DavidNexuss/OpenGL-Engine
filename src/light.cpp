#include "light.h"
#include "activeMaterial.h"
#include <iostream>


size_t LightLoader::add(glm::vec3 pos,glm::vec3 color)
{
    lightsPositions.push_back(pos);
    lightsColor.push_back(color);
    lightsComponent.emplace_back("");
    flushUniforms = true;
    return lightsPositions.size() - 1;
}

void LightLoader::flush(ActiveMaterial mat)
{
        mat.bindUniform(Standard::uLightPosition,Uniform(lightsPositions.data(),lightsPositions.size()));
        mat.bindUniform(Standard::uLightColor,Uniform(lightsColor.data(),lightsColor.size()));
        mat.bindUniform(Standard::uLightCount,Uniform(int(lightsPositions.size())));
        REGISTER_LIGHT_FLUSH();
}

void LightLoader::flushClean() {
    flushUniforms = false;
}

namespace Loader {
    LightLoader lights;
}
