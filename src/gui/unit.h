#pragma once
#include <memory>
namespace GUI
{
    struct GuiUnitObject
    {
        bool doRender = true;
        virtual bool core() { return false; };
        virtual bool render() { return false; };
    };

    template <typename T>
    struct GuiUnitFunctionLambda : public GuiUnitObject
    {
        T t;
        GuiUnitFunctionLambda(T t) : t(std::move(t)) { }
        virtual bool render() override { return t(); }
        virtual bool core() override { return t(); }
    };

    template <class T>
    auto makeSimpleGuiUnit(T &&t) {
        return std::shared_ptr<GuiUnitObject>(new GuiUnitFunctionLambda<std::decay_t<T>>{std::forward<T>(t)});
    }

    struct GuiUnit
    {
        std::shared_ptr<GuiUnitObject> func;
        float targetTime;
        bool temporal;
    };
}