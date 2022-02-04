
#include "mesh_builder.h"
#include "texture.h"
#include "world_material.h"
#include "material.h"
#include "model.h"

ModelID WorldMaterial::SkyBox::model = Standard::engineInvalid;
MaterialID WorldMaterial::SkyBox::material = Standard::engineInvalid;

void WorldMaterial::SkyBox::initialize()
 {
    if(!Standard::is_invalid(model)) return;
            
    Material cubeMap_material("cubemap",{});
    cubeMap_material.isSkyboxMaterial = true;
    material = Loader::materials.add(cubeMap_material);
    MeshID cubeMap_mesh = Loader::meshes.add(MeshBuilder::createPrimitiveMesh(MeshBuilder::SkyBox,true));

    Model cubeMap_model(cubeMap_mesh,material); 
    cubeMap_model.depthMask = true;
    cubeMap_model.cullFrontFace = true;
    cubeMap_model.enabled = false;
    model = Loader::models.add(cubeMap_model);
}