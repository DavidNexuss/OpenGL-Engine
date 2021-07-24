#include "engine.h"
#include "core.h"
#include "window.h"
#include "renderer.h"
#include "camera.h"
#include "viewport.h"

namespace Engine
{
    Window *window;
}

void Engine::createEngine(const std::string& titleName)
{
    window = createWindow(titleName);
    
    glfwSetCursorPosCallback(window, Viewport::cursor_position_callback);
    glfwSetFramebufferSizeCallback(window, Viewport::framebuffer_size_callback);
    glfwSetScrollCallback(window, Viewport::scroll_callback);

    glDebugMessageCallback(&Debug::glError,NULL);
    glEnable(GL_DEBUG_OUTPUT);
    
    CameraLoader::loadCamera(Camera());
    GUI::initialize(window,"#version 100");
}

int Engine::renderLoop()
{
    Renderer::configureRenderer();
    do
    {
        REGISTER_FRAME();
        glViewport(0,0,Viewport::screenWidth,Viewport::screenHeight);

        Renderer::render();
        GUI::render();
        
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