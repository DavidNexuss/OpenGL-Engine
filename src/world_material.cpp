
#include "mesh_builder.h"
#include "texture.h"
#include "world_material.h"
#include "material.h"
#include "model.h"

ModelID WorldMaterial::SkyBox::model = Standard::invalidId;
MaterialID WorldMaterial::SkyBox::material = Standard::invalidId;

void WorldMaterial::SkyBox::initialize()
 {
    if(model != Standard::invalidId) return;
            
    Material cubeMap_material("cubemap",{});
    cubeMap_material.isSkyboxMaterial = true;
    material = MaterialLoader::loadMaterial(cubeMap_material);
    MeshID cubeMap_mesh = MeshLoader::loadMesh(MeshBuilder::createPrimitiveMesh(MeshBuilder::SkyBox,true));

    Model cubeMap_model(cubeMap_mesh,material); 
    cubeMap_model.depthMask = true;
    cubeMap_model.cullFrontFace = true;
    cubeMap_model.enabled = false;
    model = ModelLoader::loadModel(cubeMap_model);
}