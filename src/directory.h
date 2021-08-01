#pragma once 
#include <string>

namespace Directory
{
    extern const char* texturePrefix;
    extern const char* materialPrefix;
    extern const char* objectPrefix;
    extern const char* fontPaths;

    std::string getDefaultVertexShaderPath();
    std::string getDefaultFragemntShaderPath();
}
