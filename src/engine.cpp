#include "engine.h"
#include "core.h"
#include "renderer.h"
#include "camera.h"
#include "viewport.h"
#include <gui/gui.h>
#include <gui/debug/gui_debug.h>
#include "debug.h"
#include "window.h"
#include <iostream>

std::string EngineConfiguration::glslVersion() const {
    return "#version " + std::to_string(openglMajorVersion) + std::to_string(openglMinorVersion) + "0 ";
}

namespace Engine
{
    EngineConfiguration configuration;
    RenderConfiguration renderConfiguration;

    Window *window;
    float currentFrameTime = 0.0f;
    float lastFrameTime = 0.0f;
    float deltaTime = 0.0f;

    inline void computeDeltaTime() {
        lastFrameTime = currentFrameTime;
        currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
    }

    inline void engineGUI() {
        GUI::Debug::renderRenderConfigMenu(nullptr,renderConfiguration);
    }

    inline void configure() {
        Renderer::configureRenderer(renderConfiguration);
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

    //#ifdef DEBUG
        glDebugMessageCallback(&Debug::glError,NULL);
        glEnable(GL_DEBUG_OUTPUT);
    //#endif

    Loader::cameras.add(Camera());
    GUI::initialize(window,configuration.glslVersion().c_str());

    GUI::addUnit(GUI::makeSimpleGuiUnit([&](){
        Engine::engineGUI();
        return true;
    }));
}

int Engine::renderLoop()
{
    do {
        Engine::configure();
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
