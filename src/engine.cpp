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
}

int Engine::renderLoop()
{
    return Renderer::renderLoop(window);
}