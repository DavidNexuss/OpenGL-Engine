#pragma once
#include "basics.h"
#include "texture.h"
#include "standard.h"

class WorldMaterial
{
    struct SkyBox {
        
        static ModelID model;
        static MaterialID material;
        static void initialize();

        Texture texture = Standard::invalidId;
    };


    public:
    SkyBox skyBox;

    inline Texture getSkyBoxTexture() const {
        return skyBox.texture;
    }

    inline void setSkyBox(Texture skyTexture) {
        SkyBox::initialize();
        skyBox.texture = skyTexture;
    }
};