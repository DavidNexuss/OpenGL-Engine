#pragma once
#include <vector>
#include <list>
#include <functional>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "window.h"

using GuiUnit = std::function<void(void)>;

namespace GUI {

    struct TemporalUnit
    {
        GuiUnit unit;
        float targetTime;
    };

    extern std::vector<GuiUnit> guiUnits;
    extern std::list<TemporalUnit> temporalUnits;

    extern std::vector<ImFont*> fonts;

    void initialize(GLFWwindow* window,const char* glsl_version);
    void addUnit(const GuiUnit& unit);
    void addTemporalUnit(float targetTime,const GuiUnit& unit);
    void dispose();
    void render(float deltaTime);

    size_t loadFont(const std::string& fontname,int size);
    size_t loadFont(const std::string& fontname,int size,size_t id);
    void setFont(int id);

    inline bool isMouseOnGUI() {
        return ImGui::GetIO().WantCaptureMouse;
    }
};