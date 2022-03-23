#include "viewport.h"
#include <gui/gui.h>

namespace Viewport
{
    double screenWidth = 800,screenHeight = 600;
    double xpos,ypos;
    double scrollX,scrollY;
    double deltaTime = 0.1;
}

void Viewport::cursor_position_callback(Window* window, double x, double y)
{
    if(GUI::isMouseOnGUI()) return;

    xpos = x;
    ypos = y;
}

void Viewport::framebuffer_size_callback(Window* window, int width, int height)
{
    screenWidth = width;
    screenHeight = height;
    glViewport(0, 0, width, height);
}

void Viewport::scroll_callback(Window* window, double xoffset, double yoffset)
{
    if(GUI::isMouseOnGUI()) return;

    scrollX = xoffset;
    scrollY = yoffset;
}

void Viewport::init_callbacks(Window* window)
{
    glfwSetCursorPosCallback(window, Viewport::cursor_position_callback);
    glfwSetFramebufferSizeCallback(window, Viewport::framebuffer_size_callback);
    glfwSetScrollCallback(window, Viewport::scroll_callback);
}