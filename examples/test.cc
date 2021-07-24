#include <iostream>
#include <engine.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <gui/gui_ext.h>

using namespace std;
using namespace glm;

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

    GUI::addUnit([&](){
        
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
    });

  //  GUI::setFont(GUI::loadFont("mono.ttf",16));
    return Engine::renderLoop();
}