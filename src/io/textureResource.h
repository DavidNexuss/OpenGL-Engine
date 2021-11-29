#include "resource.h"
#include <stb_image/stb_image.h>
#include <core.h>
struct TextureResourceBuffer :  public ResourceBuffer
{
    uint8_t* data;
    int width, height;
    int nrChannels;

    virtual Buffer getBuffer() const override  { 
        return {size_t(width * height * nrChannels),data}; 
    }

    TextureResourceBuffer(const ResourceHeader& header) : ResourceBuffer(header) { 
        data = stbi_load(header.path.c_str(),&width,&height,&nrChannels,0);
    }
};

struct TextureResource : public Resource
{
    TextureResource(ResourceHeader header) : Resource(header) { }

    virtual std::shared_ptr<ResourceBuffer> createResource(const ResourceHeader& header) {
        return std::make_shared<TextureResourceBuffer>(header);
    }

    inline GLuint format() const { 
        const static GLuint formats[] = {GL_R,GL_RG,GL_RGB,GL_RGBA};
        return formats[us]
    } 
};