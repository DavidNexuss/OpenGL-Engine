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

    for (int i = 0; i < 100; i++)
    {
        Model cube2 = cube;
        cube2.transformMatrix = glm::translate(glm::mat4(1.0),glm::vec3(randf(),randf(),randf()) * 30.0f);
        ModelLoader::loadModel(cube2);
    }
    
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
        ImGuiIO& io = ImGui::GetIO();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        if (corner != -1)
        {
            const float PAD = 10.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            window_flags |= ImGuiWindowFlags_NoMove;
        }
        bool p_open = true;
        if (ImGui::Begin("FrameRate",&p_open, window_flags))
        {
            ImGui::Text((string("Framerate:  ") + to_string(Debug::getFrameRate())).c_str());
        }
        ImGui::End();
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