#include <iostream>
#include "engine.h"
#include "model.h"
#include "mesh.h"
#include "material.h"
#include "mesh_builder.h"
#include "light.h"
#include "obj.h"

#include <glm/ext.hpp>

using namespace std;
using namespace glm;

//Funciones especificas de testeo
void loadSpecificMaterials()
{

    MaterialInstance debugMaterialInstance({vec4(1.0,1.0,1.0,1.0)});

    MaterialInstance container({Uniform(15.0f)});
    
    container.setTexture(TextureLoader::loadTexture(TextureData("glass.png")),0);
    container.setTexture(TextureLoader::loadTexture(TextureData("glass_specular.png")),1);
    container.setTexture(TextureLoader::loadTexture(TextureData("glass_normal.png")),2);
    MaterialInstanceLoader::loadMaterialInstance(container);

    MaterialLoader::loadMaterial(Material("primitive",list<string>()));
    MaterialLoader::loadMaterial(Material("emissive",{"emissive","factor"}));
    Material light("light",{"shinness"});

    MaterialLoader::loadMaterial(light);
    MaterialLoader::debugMaterialID = MaterialLoader::loadMaterial(Material("unshaded",{"shadecolor"}));
    MaterialLoader::debugMaterialInstanceID = MaterialInstanceLoader::loadMaterialInstance(MaterialInstance({vec4(1.0,1.0,1.0,1.0)}));

    Material debug("unshaded",list<string>());
    Material textured("textured",list<string>());
    MaterialLoader::loadMaterial(textured);

    Material textured2("textured",list<string>());
    MaterialLoader::loadMaterial(textured2);
}
void loadSpecificWorld()
{
    
    Model cube = Model(MeshLoader::loadMesh(MeshBuilder::createPrimitiveMesh(MeshBuilder::Cube,true)));
    
    /*cube.materialID = 2;
    cube.materialInstanceID = 0;

    int n = 4;
    int n2 = n / 2;
    for (int i = -n2; i < n2; i++)
    {
        for (int j = -n2; j < n2; j++)
        {
            cube.transformMatrix = glm::translate(mat4(1.0f),vec3(2.2 * i,2.2 * j,0.0));
            ModelLoader::loadModel(cube);
        }
    }*/

    
    Model cube2 = cube;
    for (size_t i = 0; i < 4; i++)
    {

        vec3 lightPosition(rand() % 10 - 5,rand() % 10 - 5,rand() % 10 - 2);
        cube2.transformMatrix = glm::scale(glm::translate(mat4(1.0),lightPosition),vec3(0.1));
        cube2.materialID = 3;
        vec4 color((rand() % 255) / 255.0f,(rand() % 255) / 255.0f,(rand() % 255 ) / 255.0f,1.0);
        MaterialInstance unshadedColor(1);
        unshadedColor.set(0,color);
        cube2.materialInstanceID = MaterialInstanceLoader::loadMaterialInstance(unshadedColor);
        
        ModelLoader::loadModel(cube2);
        Light::load(cube2.transformMatrix[3],vec3(color) * 2.0f);
    }


    ModelGroup baby = loadMeshFromFile("baby.obj",2,true);
    baby[0].transformMatrix = glm::scale(mat4(1.0f),vec3(3.0f));
    ModelLoader::loadModel(baby);

    ModelLoader::loadModel(loadMeshFromFile("backpack.obj",2)) /* .apply([](const glm::mat4& matrix){
        return glm::rotate<float>(matrix,M_PI,vec3(0,1,0));
    }) */ ;
    
}


int main(int argc, char** argv)
{
    Engine::createEngine("Test Engine");
    loadSpecificMaterials();
    loadSpecificWorld();
    return Engine::renderLoop();
}
