#pragma once
#include "glfw.h"
#include "core.h"
#include "debug.h"
#include "texture_data.h"
#include "standard.h"
#include <vector>
#include <iostream>
/**
 * @brief Manages OpenGL textures and its identifiers
 */
using Texture = GLuint;
namespace TextureLoader
{
    const static size_t maxTextureUnits = 16;
    extern int currentActiveUnit;
    extern std::vector<Texture> texturesUnits;                                   // slot -> textureID
    
    extern Texture loadInternalTexture(GLuint textID);
    extern Texture loadTexture(const TextureData& textureData,bool filter = true);
    extern Texture loadCubemap(const std::vector<TextureData> &cubemaps);

    inline static void useTexture(Texture textureID,size_t textureUnit,GLenum mode)
    {
        if (texturesUnits[textureUnit] != textureID) {
            texturesUnits[textureUnit] = textureID;
            int nextActiveUnit = GL_TEXTURE0 + textureUnit;
            if(currentActiveUnit != nextActiveUnit) {
                currentActiveUnit = nextActiveUnit;
                glActiveTexture(currentActiveUnit);
            }
            glBindTexture(mode,textureID);
            REGISTER_TEXTURE_SWAP();
        }
    }
    inline static void bindTexture(Texture textureID,GLenum mode) {
        useTexture(textureID,Standard::tCreation,mode);
    }
    inline static void invalidateCaching(){
        for(size_t i = 0; i < texturesUnits.size(); i++) { texturesUnits[i] = -1; }
    }
};
