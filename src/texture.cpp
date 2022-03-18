#include "texture.h"
using namespace std;

std::vector<Texture> TextureLoader::texturesUnits(TextureLoader::maxTextureUnits,-1);             // slot -> textureID
int TextureLoader::currentActiveUnit = 0;

int getInternalFormat(int c)
{
    const static int channels[] = {GL_RED,GL_RG8,GL_RGB8,GL_RGBA8};
    return channels[c - 1];
}
int getExternalFormat(int c)
{
    const static int channels[] = {GL_RED,GL_RG,GL_RGB,GL_RGBA};
    return channels[c - 1];
}

Texture TextureLoader::loadTexture(const TextureData& textureData,bool filter)
{
    if (!textureData.data ) return -1;
    GLuint texId;
    glGenTextures(1,&texId);
    TextureLoader::bindTexture(texId,GL_TEXTURE_2D);
    //TextureLoader filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if(filter)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    }
    else{
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);    
    }
    glTexImage2D(
        GL_TEXTURE_2D, 0, 
        GL_RGB, textureData.width, textureData.height, 0,
        getExternalFormat(textureData.nrChannels), GL_UNSIGNED_BYTE, textureData.data);

    glGenerateMipmap(GL_TEXTURE_2D);
    return texId;
}

Texture TextureLoader::loadCubemap(const vector<TextureData> &cubemaps)
{
    for (size_t i = 0; i < cubemaps.size(); i++)
        if (!cubemaps[i].data)
            return -1;
    GLuint texId;
    glGenTextures(1, &texId);
    TextureLoader::bindTexture(texId, GL_TEXTURE_CUBE_MAP);
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
    return texId;
}
