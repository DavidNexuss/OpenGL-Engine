#include "model.h"
#include "mesh_builder.h"
#include <list>
using namespace std;

sorted_vector<Model> ModelLoader::models;

bool Model::lastcullFrontFace = false;

Model ModelLoader::createSkyBox()
{
    Material cubeMap_material("cubemap",vector<string>());
    cubeMap_material.isSkyboxMaterial = true;

    MaterialID cubeMap_material_id = MaterialLoader::loadMaterial(cubeMap_material);
    
    MeshID cubeMap_mesh = MeshLoader::loadMesh(MeshBuilder::createPrimitiveMesh(MeshBuilder::SkyBox,true));
    
    Model cubeMap_model(cubeMap_mesh,cubeMap_material_id); 
    cubeMap_model.depthMask = true;
    cubeMap_model.cullFrontFace = true;

    return cubeMap_model; 

}