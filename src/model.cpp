#include "model.h"
#include "mesh_builder.h"
#include "texture.h"
#include <list>
using namespace std;

sorted_storage<Model> ModelLoader::models;

bool Model::lastcullFrontFace = false;

ModelID ModelLoader::createSkyBox(const std::vector<TextureData>& paths)
{
    Material cubeMap_material("cubemap",vector<string>());
    cubeMap_material.isSkyboxMaterial = true;

    MaterialID cubeMap_material_id = MaterialLoader::loadMaterial(cubeMap_material);
    MeshID cubeMap_mesh = MeshLoader::loadMesh(MeshBuilder::createPrimitiveMesh(MeshBuilder::SkyBox,true));
    
    Model cubeMap_model(cubeMap_mesh,cubeMap_material_id); 
    cubeMap_model.depthMask = true;
    cubeMap_model.cullFrontFace = true;
    cubeMap_model.enabled = false;

    MaterialInstance mat;
    mat.setTexture(TextureLoader::loadCubemap(paths),0);
    MaterialInstanceID cubemap_material_instance = MaterialInstanceLoader::loadMaterialInstance(mat);
    cubeMap_model.materialInstanceID = cubemap_material_instance;
    return loadModel(std::move(cubeMap_model)); 

}