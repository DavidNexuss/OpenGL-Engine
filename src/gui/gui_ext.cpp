#include "gui_ext.h"

namespace GUI
{
    namespace Extra
    {
		EngineTextEditor::EngineTextEditor() : editor(std::make_shared<TextEditor>()) { }
        bool EngineTextEditor::render()
        {
            auto cpos = editor->GetCursorPosition();
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
		    		bool ro = editor->IsReadOnly();
		    		if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
		    			editor->SetReadOnly(ro);
		    		ImGui::Separator();

		    		if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor->CanUndo()))
		    			editor->Undo();
		    		if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor->CanRedo()))
		    			editor->Redo();

		    		ImGui::Separator();

		    		if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor->HasSelection()))
		    			editor->Copy();
		    		if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor->HasSelection()))
		    			editor->Cut();
		    		if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor->HasSelection()))
		    			editor->Delete();
		    		if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr, !ro && ImGui::GetClipboardText() != nullptr))
		    			editor->Paste();

		    		ImGui::Separator();

		    		if (ImGui::MenuItem("Select all", nullptr, nullptr))
		    			editor->SetSelection(TextEditor::Coordinates(), TextEditor::Coordinates(editor->GetTotalLines(), 0));

		    		ImGui::EndMenu();
		    	}

		    	if (ImGui::BeginMenu("View"))
		    	{
		    		if (ImGui::MenuItem("Dark palette"))
		    			editor->SetPalette(TextEditor::GetDarkPalette());
		    		if (ImGui::MenuItem("Light palette"))
		    			editor->SetPalette(TextEditor::GetLightPalette());
		    		if (ImGui::MenuItem("Retro blue palette"))
		    			editor->SetPalette(TextEditor::GetRetroBluePalette());
		    		ImGui::EndMenu();
		    	}
		    	ImGui::EndMenuBar();
		    }

		    ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1, editor->GetTotalLines(),
		    	editor->IsOverwrite() ? "Ovr" : "Ins",
		    	editor->CanUndo() ? "*" : " ",
		    	editor->GetLanguageDefinition().mName.c_str(), "test");

		    editor->Render("TextEditor");
		    ImGui::End();
			return true;
        }

		void EngineTextEditor::openResource(Resource& resource)
		{
			editor->SetText(resource.asString());
		}

		ImGuiWindowFlags overlayFlags(int corner,float x, float y)
		{
        	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        	const float PAD = 10.0f;
        	const ImGuiViewport* viewport = ImGui::GetMainViewport();
        	ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
        	ImVec2 work_size = viewport->WorkSize;
        	ImVec2 window_pos, window_pos_pivot;
        	window_pos.x = (corner & 1) ? (work_pos.x + work_size.x + x - PAD) : (work_pos.x + x + PAD);
        	window_pos.y = (corner & 2) ? (work_pos.y + work_size.y + y - PAD) : (work_pos.y + y + PAD);
        	window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
        	window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
        	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        	window_flags |= ImGuiWindowFlags_NoMove;
			return window_flags;
    	}
	}
}