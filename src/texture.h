#pragma once
#include "glfw.h"
#include "core.h"
#include "debug.h"
#include "texture_data.h"
#include "internals.h"
#include <vector>

/**
 * @brief Manages OpenGL textures and its identifiers
 */
using Texture = GLuint;
namespace TextureLoader
{
    const static size_t maxTextureUnits = 16;
    extern std::vector<Texture> texturesUnits;                                   // slot -> textureID
    
    extern Texture loadInternalTexture(GLuint textID);
    extern Texture loadTexture(const TextureData& textureData,bool filter = true);
    extern Texture loadCubemap(const std::vector<TextureData> &cubemaps);

    inline static void useTexture(Texture textureID,int textureUnit,GLenum mode)
    {
        if (texturesUnits[textureUnit] != textureID)
        {
            texturesUnits[textureUnit] = textureID;
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(mode,textureID);
            REGISTER_TEXTURE_SWAP();
        }
    }
};