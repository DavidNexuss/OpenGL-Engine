#include <iostream>
#include <engine.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gui/gui_ext.h>
#include <light.h>
#include <gui/debug/gui_debug.h>
#include <mesh/sphereMesh.h>
#include <obj.h>
#include <mesh_builder.h>


using namespace std;
using namespace glm;


float randf()
{
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
void loadSpecificWorld()
{
    
    Model cube = Model(MeshLoader::loadMesh(MeshBuilder::createPrimitiveMesh(MeshBuilder::Cube,true)));
    cube.materialID = MaterialLoader::loadMaterial(Material("ushader_untextured",
    {"ka","kd","ks","specularStrength","reflectionStrength"}));

    cube.materialInstanceID = MaterialInstanceLoader::loadMaterialInstance(
        MaterialInstance({
            glm::vec3(0.4),glm::vec3(0.4),glm::vec3(0.4),50.0f,0.2f
        }));
    //ModelLoader::loadModel(cube);
    Light::load(glm::vec3(8.0),glm::vec3(1.0,1.0,1.0));

    Renderer::worldMaterial.setSkyBox(TextureLoader::loadCubemap({
        TextureData("canyon/right.png"),
        TextureData("canyon/left.png"),
        TextureData("canyon/top.png"),
        TextureData("canyon/bottom.png"),
        TextureData("canyon/front.png"),
        TextureData("canyon/back.png")
    }));
/*
    MeshID sphere = MeshLoader::loadMesh(SphereMesh(2.0,64,64).generateMesh());
    Model sphereModel = Model(sphere,cube.materialID);
    sphereModel.materialInstanceID = cube.materialInstanceID;
    ModelLoader::loadModel(sphereModel);
*/
    ModelGroup donut = loadMeshFromFile("donut.obj",cube.materialID);
    donut[0].materialInstanceID = cube.materialInstanceID;
    ModelLoader::loadModel(donut);
}


int main(int argc, char** argv)
{
    Engine::createEngine("Test Engine",{
        .mssaLevel = 4,
        .openglMajorVersion = 4,
        .openglMinorVersion = 5,
        .windowWidth = 1024,
        .windowHeight = 768
    });

    loadSpecificWorld();

    GUI::addUnit(GUI::makeSimpleGuiUnit([](){
        bool window = true;
        GUI::Debug::renderModelTree(&window);
        GUI::Debug::renderMaterialInstanceMenu(0,0,nullptr);
        return true;
    }));


    GUI::addUnit(GUI::makeSimpleGuiUnit([&](){
        
        static int corner = 0;
        static bool p_open = true;
        if(p_open)
        {
            ImGui::Begin("FrameRate",&p_open, GUI::Extra::overlayFlags(0,100,0));
            ImGui::Text((string("Framerate:  ") + to_string(Debug::getFrameRate())).c_str());
            ImGui::End();
        }
        return true;
    }));
/*
    GUI::Extra::EngineTextEditor* editor = new GUI::Extra::EngineTextEditor();
    Resource text = Resource(ResourceHeader::fromFile("res/materials/base_vertex.glsl"));
    editor->openResource(text);
    GUI::addUnit(std::shared_ptr<GUI::GuiUnitObject>((GUI::GuiUnitObject*)editor));
*/
    return Engine::renderLoop();
}