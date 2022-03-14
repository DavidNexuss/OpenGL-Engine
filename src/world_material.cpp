#include "world_material.h"

void WorldMaterial::bind(MaterialID material) {

    //Flush sky texture
    if (skyTexture && material->isSkyBoxSensitive()) {
        TextureLoader::useTexture(skyTexture,Standard::tSkyBox,GL_TEXTURE_CUBE_MAP);
        glUniform1i(material->uniforms[Standard::uSkyBox],Standard::tSkyBox);
    }
}
