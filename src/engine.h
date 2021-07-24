#pragma once
#pragma once
#include <string>
#include <material.h>
#include <texture.h>
#include <renderer.h>
#include <gui.h>
#include <io/resource.h>
#include <mesh.h>
#include <mesh_builder.h>
#include <model.h>

namespace Engine
{
    void createEngine(const std::string& windowTitle);
    void disposeEngine();
    int renderLoop();
};