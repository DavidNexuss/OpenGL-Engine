#include <iostream>
#include <engine.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gui/gui_ext.h>
#include <gui/debug/gui_debug.h>

using namespace std;
using namespace glm;


float randf()
{
    return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}
void loadSpecificWorld()
{
    
    Model cube = Model(MeshLoader::loadMesh(MeshBuilder::createPrimitiveMesh(MeshBuilder::Cube,true)));
    cube.materialID = MaterialLoader::loadMaterial(Material("base",{"surface"}));
    cube.materialInstanceID = MaterialInstanceLoader::loadMaterialInstance(MaterialInstance({glm::vec3(0.4)}));
    ModelLoader::loadModel(cube);
}


int main(int argc, char** argv)
{
    Engine::createEngine("Test Engine");
    loadSpecificWorld();

    GUI::addUnit(GUI::makeSimpleGuiUnit([](){
        bool window = true;
        GUI::Debug::renderModelTree(&window);
        GUI::Debug::renderMaterialInstanceMenu(0,0,nullptr);
        return true;
    }));


    GUI::addUnit(2.0,GUI::makeSimpleGuiUnit([&](){
        
        static int corner = 0;
        static bool p_open = true;
        if(p_open)
        {
            ImGui::Begin("FrameRate",&p_open, GUI::Util::overlayFlags(0,100,0));
            ImGui::Text((string("Framerate:  ") + to_string(Debug::getFrameRate())).c_str());
            ImGui::End();
        }
        return true;
    }));
/*
    TextEditor editor;
    Resource text = Resource(ResourceHeader::fromFile("res/materials/base_vertex.glsl").makeWatch(),[&](const vector<uint8_t>& data){
        editor.SetText((const char*)data.data());
    });
    
    GUI::Util::openResource(editor,text);
    GUI::addUnit(GUI::makeSimpleGuiUnit([&](){
        GUI::Util::displayEditor(editor);
        return true;
    }));
*/
  //  GUI::setFont(GUI::loadFont("mono.ttf",16));
    return Engine::renderLoop();
}