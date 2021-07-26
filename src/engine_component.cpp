#include "engine_component.h"

EngineComponent::EngineComponent() : EngineComponent("") { }
EngineComponent::EngineComponent(const std::string& _name) : name(_name)
{
    editMenu = false;
}