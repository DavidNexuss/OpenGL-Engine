#include "mesh_builder.h"
#include "texture.h"
#include "world_material.h"
#include "material.h"
#include "model.h"
#include "sky.h"


ModelID SkyCreator::createSkyBox()
{
    Material skyMaterial("cubemap",{});
    skyMaterial.isSkyboxMaterial = true;

    MeshID skyMesh = Loader::meshes.add(MeshBuilder::createPrimitiveMesh(MeshBuilder::SkyBox,true));
    Model skyModel(skyMesh,Loader::materials.add(skyMaterial));
    
    skyModel.depthMask = true;
    skyModel.cullFrontFace = true;
    skyModel.enabled = false;

    return Loader::models.add(skyModel);
}