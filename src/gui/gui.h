#pragma once
#include <vector>
#include <list>
#include <functional>
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include "window.h"
#include <memory>
namespace GUI {

    struct GuiUnitFunctionObject
    {
        bool doRender = true;
        virtual bool core() = 0;
        virtual bool render() = 0;
    };
    using GuiUnitFunction = std::shared_ptr<GuiUnitFunctionObject>;

    template <typename T>
    struct GuiUnitFunctionLambda : public GuiUnitFunctionObject
    {
        T t;
        GuiUnitFunctionLambda(T t) : t(std::move(t)) { }
        virtual bool render() override { return t(); }
        virtual bool core() override { return t(); }
    };
    template <class T>
    auto makeSimpleGuiUnit(T &&t) {
        return GuiUnitFunction(new GuiUnitFunctionLambda<std::decay_t<T>>{std::forward<T>(t)});
    }


    struct GuiUnit
    {
        GuiUnitFunction func;
        float targetTime;
        bool temporal;
    };

    extern std::vector<GuiUnit> guiUnits;
    extern std::vector<ImFont*> fonts;

    void initialize(GLFWwindow* window,const char* glsl_version);
    void addUnit(const GuiUnitFunction& unit);
    void addUnit(float targetTime,const GuiUnitFunction& unit);
    void dispose();
    void render(float deltaTime);

    size_t loadFont(const std::string& fontname,int size);
    size_t loadFont(const std::string& fontname,int size,size_t id);
    void setFont(int id);

    inline bool isMouseOnGUI() {
        return ImGui::GetIO().WantCaptureMouse;
    }
};