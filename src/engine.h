#pragma once
#include <string>
#include "standard.h"

struct EngineConfiguration {
    int mssaLevel = Standard::defaultMssaLevel;
    int openglMajorVersion = Standard::defaultOpenglMajorVersion;
    int openglMinorVersion = Standard::defaultOpenglMinorVersion;
    int windowWidth = Standard::defaultWindowWidth;
    int windowHeight = Standard::defaultlWindowHeight;

    std::string glslVersion() const;
};

namespace Engine
{   
    /**
     * @brief Initializes all engine systems and creates window
     */ 
    void createEngine(const std::string& windowTitle,const EngineConfiguration& configuration);
    
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