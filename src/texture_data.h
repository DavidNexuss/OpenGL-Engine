#pragma once
#include "core.h"
#include <vector>
#include <string>

struct TextureData
{
    int width, height, nrChannels;
    unsigned char* data;

    TextureData(const std::string& path);
    
    inline GLint format() const { return nrChannels == 3 ? GL_RGB : GL_RGBA; }

    inline static std::vector<TextureData> load(const std::vector<std::string>& paths)
    {
        std::vector<TextureData> textureData;
        for (size_t i = 0; i < paths.size(); i++)
        {
            textureData.emplace_back(paths[i]);
        }
        return textureData;
    }
};
