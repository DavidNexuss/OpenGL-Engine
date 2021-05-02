#include "texture.h"
using namespace std;

std::vector<TextureData> TextureLoader::texturesData;                                // textureID -> textureData
std::vector<GLuint> TextureLoader::glTexturesIds;                                    // textureID -> GLID
std::vector<Texture> TextureLoader::texturesUnits(TextureLoader::maxTextureUnits,-1);             // slot -> textureID
Texture TextureLoader::skyBoxID;

Texture TextureLoader::loadTexture(const TextureData& textureData)
{
    if (!textureData.data ) return -1;
    GLuint texId;
    glGenTextures(1,&texId);
    glBindTexture(GL_TEXTURE_2D,texId);
    //TextureLoader filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLint format = textureData.nrChannels == 3 ? GL_RGB : GL_RGBA; 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureData.width, textureData.height, 0, format, GL_UNSIGNED_BYTE, textureData.data);
    glGenerateMipmap(GL_TEXTURE_2D);
    texturesData.push_back(textureData);
    glTexturesIds.push_back(texId);
    return texturesData.size() - 1;
}

Texture TextureLoader::loadCubemap(const vector<TextureData> &cubemaps)
{
    for (size_t i = 0; i < cubemaps.size(); i++)
        if (!cubemaps[i].data)
            return -1;
    GLuint texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texId);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    for (size_t i = 0; i < cubemaps.size(); i++)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                     0, GL_RGB, cubemaps[i].width, cubemaps[i].height, 0, cubemaps[i].format(), GL_UNSIGNED_BYTE, cubemaps[i].data);
    }
    glTexturesIds.push_back(texId);
    return glTexturesIds.size() - 1;
}
