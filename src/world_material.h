#pragma once
#include "basics.h"
#include "id.h"

class WorldMaterial
{
    struct SkyBox {
        
        static ModelID model;
        static MaterialID material;
        static void initialize();

        Texture texture = ID::invalid_id;
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