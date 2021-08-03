#include "engine.h"
#include "core.h"
#include "renderer.h"
#include "camera.h"
#include "viewport.h"
#include <gui/gui.h>
#include "debug.h"
#include "window.h"
#include <iostream>

std::string EngineConfiguration::glslVersion() const {
    return "#version " + std::to_string(openglMajorVersion) + std::to_string(openglMinorVersion) + "0 ";
}

namespace Engine
{
    EngineConfiguration configuration;
    Window *window;
    float currentFrameTime = 0.0f;
    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;

    inline void computeDeltaTime()
    {
        lastFrameTime = currentFrameTime;
        currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
    }
}

void Engine::createEngine(const std::string& titleName,const EngineConfiguration& configuration)
{
    Engine::configuration = configuration;
    window = createWindow(titleName,
    configuration.windowWidth,
    configuration.windowHeight,
    configuration.openglMajorVersion,
    configuration.openglMinorVersion,
    configuration.mssaLevel);
    
    glfwSetCursorPosCallback(window, Viewport::cursor_position_callback);
    glfwSetFramebufferSizeCallback(window, Viewport::framebuffer_size_callback);
    glfwSetScrollCallback(window, Viewport::scroll_callback);

    #ifdef DEBUG
        glDebugMessageCallback(&Debug::glError,NULL);
        glEnable(GL_DEBUG_OUTPUT);
    #endif
    CameraLoader::loadCamera(Camera());
    GUI::initialize(window,configuration.glslVersion().c_str());
}

int Engine::renderLoop()
{
    RenderConfiguration config;
    config.useMssa = Engine::configuration.mssaLevel > 0;
    Renderer::configureRenderer(config);
    do
    {
        computeDeltaTime();
        REGISTER_FRAME(currentFrameTime);
        
        glViewport(0,0,Viewport::screenWidth,Viewport::screenHeight);

        Renderer::render();
        GUI::render(deltaTime);
        
        LOG_FRAME();

        glfwSwapBuffers(window);
        glfwPollEvents();
        

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
               glfwWindowShouldClose(window) == 0);
    
    return 0;
}

void Engine::disposeEngine()
{
    GUI::dispose();
}