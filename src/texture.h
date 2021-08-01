#pragma once
#include "glfw.h"
#include "core.h"
#include "debug.h"
#include "texture_data.h"
#include <vector>

using Texture = size_t;

/**
 * @brief Manages OpenGL textures and its identifiers
 */

namespace TextureLoader
{
    const static size_t maxTextureUnits = 16;
    const static size_t skyBoxTextureUnit = 15;

    extern std::vector<TextureData> texturesData;                                // textureID -> textureData
    extern std::vector<GLuint> glTexturesIds;                                    // textureID -> GLID
    extern std::vector<Texture> texturesUnits;                                   // slot -> textureID
    extern Texture skyBoxID;
    
    extern Texture loadInternalTexture(GLuint textID);
    extern Texture loadTexture(const TextureData& textureData);
    extern Texture loadCubemap(const std::vector<TextureData> &cubemaps);

    inline static void useTexture(Texture textureID,int textureUnit,GLenum mode)
    {
        GLuint glTextureID = glTexturesIds[textureID];
        if (texturesUnits[textureUnit] != textureID)
        {
            texturesUnits[textureUnit] = textureID;
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(mode,glTextureID);
            REGISTER_TEXTURE_SWAP();
        }
    }

    inline static size_t bindSkyBox()
    {
        useTexture(skyBoxID,skyBoxTextureUnit,GL_TEXTURE_CUBE_MAP);
        return skyBoxTextureUnit;
    }
};