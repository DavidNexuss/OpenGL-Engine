#pragma once
#include <vector>
#include <list>
#include <functional>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <memory>
#include "window.h"
#include "unit.h"

/**
 * Engine GUI system
 */

namespace GUI {

    extern std::vector<GuiUnit> guiUnits;

    void initialize(GLFWwindow* window,const char* glsl_version);
    void addUnit(const std::shared_ptr<GuiUnitObject>& unit);
    void addUnit(float targetTime,const std::shared_ptr<GuiUnitObject>& unit);
    void dispose();
    void render(float deltaTime);

    inline bool isMouseOnGUI() {
        return ImGui::GetIO().WantCaptureMouse;
    }
};