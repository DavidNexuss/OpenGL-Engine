#include "gui.h"
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <ImGuiColorTextEdit/TextEditor.h>

using namespace std;
namespace GUI
{
    vector<GuiUnit> guiUnits;
    
    void initialize(GLFWwindow* window,const char* glsl_version)
    {
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

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
    void addUnit(const GuiUnit& unit)
    {
        guiUnits.push_back(unit);
    }
    void render()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (size_t i = 0; i < guiUnits.size(); i++) {
            guiUnits[i]();
        }
        
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    namespace Util
    {
        void displayEditor(TextEditor& editor)
        {
            auto cpos = editor.GetCursorPosition();
            ImGui::Begin("Text Editor Demo", nullptr, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
		    ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
		    if (ImGui::BeginMenuBar())
		    {
		    	if (ImGui::BeginMenu("File"))
		    	{
		    		if (ImGui::MenuItem("Save"))
		    		{
		    			//
		    		}
		    		if (ImGui::MenuItem("Quit", "Alt-F4")) { } // Handle quit
		    		ImGui::EndMenu();
		    	}

		    	if (ImGui::BeginMenu("Edit"))
		    	{
		    		bool ro = editor.IsReadOnly();
		    		if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
		    			editor.SetReadOnly(ro);
		    		ImGui::Separator();

		    		if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
		    			editor.Undo();
		    		if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
		    			editor.Redo();

		    		ImGui::Separator();

		    		if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
		    			editor.Copy();
		    		if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
		    			editor.Cut();
		    		if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
		    			editor.Delete();
		    		if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
		    			editor.Paste();

		    		ImGui::Separator();

		    		if (ImGui::MenuItem("Select all", nullptr, nullptr))
		    			editor.SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor.GetTotalLines(), 0));

		    		ImGui::EndMenu();
		    	}

		    	if (ImGui::BeginMenu("View"))
		    	{
		    		if (ImGui::MenuItem("Dark palette"))
		    			editor.SetPalette(TextEditor::GetDarkPalette());
		    		if (ImGui::MenuItem("Light palette"))
		    			editor.SetPalette(TextEditor::GetLightPalette());
		    		if (ImGui::MenuItem("Retro blue palette"))
		    			editor.SetPalette(TextEditor::GetRetroBluePalette());
		    		ImGui::EndMenu();
		    	}
		    	ImGui::EndMenuBar();
		    }

		    ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor.GetTotalLines(),
		    	editor.IsOverwrite() ? "Ovr" : "Ins",
		    	editor.CanUndo() ? "*" : " ",
		    	editor.GetLanguageDefinition().mName.c_str(), "test");

		    editor.Render("TextEditor");
		    ImGui::End();
        }
    }
}