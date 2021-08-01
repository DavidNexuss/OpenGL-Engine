#include "texture_data.h"
#include "directory.h"
#include <stb_image/stb_image.h>
#include <iostream>
using namespace std;

TextureData::TextureData(const string& path)
{
    data = stbi_load((string(Directory::texturePrefix) + string(path)).c_str(),&width,&height,&nrChannels,0);
    if (! data)
    {
        cerr << "Error loading texture " << path << " from disk" << endl;
        throw std::runtime_error("Error loading texture");
    }
}
