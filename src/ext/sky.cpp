#include "mesh_builder.h"
#include "texture.h"
#include "world_material.h"
#include "activeMaterial.h"
#include "material.h"
#include "model.h"
#include "sky.h"
#include <iostream>

ModelID SkyCreator::createSkyBox()
{
    Material skyMaterial("cubemap");
    skyMaterial.isSkyboxMaterial = true;

    MeshID skyMesh = Loader::meshes.add(MeshBuilder::createPrimitiveMesh(MeshBuilder::SkyBox,true));
    Model skyModel(skyMesh,Loader::materials.add(skyMaterial));
    
    skyModel.depthMask = true;
    skyModel.cullFrontFace = true;
    skyModel.zIndex = -1;
    
    return Loader::models.add(skyModel);
}

void SkyWorldMaterial::bind(ActiveMaterial material) {
    //Flush sky texture
    if (skyTexture) {
        material.bindUniform(Standard::uSkyBox,UTexture(skyTexture,Standard::tSkyBox,GL_TEXTURE_CUBE_MAP));
    }
}