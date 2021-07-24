#pragma once
#include <vector>
#include <functional>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "window.h"

using GuiUnit = std::function<void(void)>;

namespace GUI {

    extern std::vector<GuiUnit> guiUnits;
    
    void initialize(GLFWwindow* window,const char* glsl_version);
    void addUnit(const GuiUnit& unit);
    void dispose();
    void render();
};