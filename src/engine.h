#pragma once
#pragma once
#include <string>
#include <material.h>
#include <texture.h>
#include <renderer.h>
#include <gui/gui.h>
#include <io/resource.h>
#include <mesh.h>
#include <mesh_builder.h>
#include <model.h>

namespace Engine
{
    /**
     * @brief Initializes all engine systems and creates window
     */ 
    void createEngine(const std::string& windowTitle);
    
    /**
     * @brief dispose all engine resources
     */
    void disposeEngine();

    /**
     * @brief enters in renderLoop
     * @returns 0 if exited successfully
     */
    int renderLoop();
};