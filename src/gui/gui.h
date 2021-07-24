#pragma once
#include <vector>
#include <functional>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "window.h"

using GuiUnit = std::function<void(void)>;

namespace GUI {

    extern std::vector<GuiUnit> guiUnits;
    extern std::vector<ImFont*> fonts;

    void initialize(GLFWwindow* window,const char* glsl_version);
    void addUnit(const GuiUnit& unit);
    void dispose();
    void render();

    size_t loadFont(const std::string& fontname,int size);
    size_t loadFont(const std::string& fontname,int size,size_t id);
    void setFont(int id);

    inline bool isMouseOnGUI() {
        return ImGui::GetIO().WantCaptureMouse;
    }
};