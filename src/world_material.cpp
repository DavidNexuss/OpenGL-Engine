
#include "mesh_builder.h"
#include "texture.h"
#include "world_material.h"
#include "material.h"
#include "model.h"

ModelID WorldMaterial::SkyBox::model = ID::invalid_id;
MaterialID WorldMaterial::SkyBox::material = ID::invalid_id;

void WorldMaterial::SkyBox::initialize()
 {
    if(model != ID::invalid_id) return;
            
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