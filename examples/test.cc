#include <iostream>
#include <engine.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gui_ext.h>

using namespace std;
using namespace glm;

//Funciones especificas de testeo
void loadSpecificMaterials()
{
    MaterialLoader::loadMaterial(Material("base",{"surface"}));
    MaterialInstanceLoader::loadMaterialInstance(MaterialInstance({glm::vec3(0.4)}));
}
void loadSpecificWorld()
{
    
    Model cube = Model(MeshLoader::loadMesh(MeshBuilder::createPrimitiveMesh(MeshBuilder::Cube,true)));
    cube.materialID = 0;
    cube.materialInstanceID = 0;
    ModelLoader::loadModel(cube);
}


int main(int argc, char** argv)
{
    Engine::createEngine("Test Engine");
    loadSpecificMaterials();
    loadSpecificWorld();
    GUI::addUnit([](){
        auto& uniform = MaterialInstanceLoader::get(0).getUniform(0);
        ImGui::Begin("Choose color");
        ImGui::ColorPicker3("Surface color",&uniform.VEC3.x);
        ImGui::End();

    });

    TextEditor editor;
    GUI::addUnit([&](){
        GUI::Util::displayEditor(editor);
    });
    return Engine::renderLoop();
}