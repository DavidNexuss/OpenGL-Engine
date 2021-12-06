#include "gui.h"
#include "gui_ext.h"
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imguiText/TextEditor.h>
#include <directory.h>
#include <limits>

using namespace std;
namespace GUI
{
    vector<GuiUnit> guiUnits;
	vector<ImFont*> fonts;
    
	void initialize(GLFWwindow* window,const char* glsl_version)
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);
    }
    void dispose()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    void addUnit(const std::shared_ptr<GuiUnitObject>& unit)
    {
		guiUnits.push_back({unit,0.0,false});
    }
	void addUnit(float targetTime,const std::shared_ptr<GuiUnitObject>& temporalUnit)
	{
		guiUnits.push_back({temporalUnit,targetTime,true});
	}
    void render(float deltaTime)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		auto it = guiUnits.begin();
		while(it != guiUnits.end())
		{
			if(!it->temporal) it->func->render();
			else{
				if(it->targetTime < 0) {
					it = guiUnits.erase(it);
					continue;
				}
				it->func->render();
				it->targetTime -= deltaTime;
			}
			++it;
		}
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
