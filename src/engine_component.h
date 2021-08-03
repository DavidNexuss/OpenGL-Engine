#pragma once
#include <string>

/**
 * @class This class serves as a superclass for all entities inside the engine, 
 * for the purpose of labeling and listing them in the GUI.
 */
struct EngineComponent
{
    std::string name;
    bool editMenu;

    EngineComponent();
    EngineComponent(const std::string& name);
};