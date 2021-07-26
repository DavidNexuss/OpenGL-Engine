#pragma once
#include <string>

struct EngineComponent
{
    std::string name;
    bool editMenu;

    EngineComponent();
    EngineComponent(const std::string& name);
};