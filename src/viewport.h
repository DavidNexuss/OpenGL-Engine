#pragma once
#include "core.h"

/**
 * Mantiene todos los atributos del viewport y de glfw a procesar,incluido el input
 */
namespace Viewport
{
    extern double screenWidth,screenHeight;
    extern double xpos,ypos;
    extern double scrollX,scrollY;
    extern double deltaTime;

    void cursor_position_callback(Window* window, double x, double y);
    void framebuffer_size_callback(Window* window, int width, int height);
    void scroll_callback(Window* window, double xoffset, double yoffset);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    void init_callbacks(Window* window);

    bool isKeyPressed(int keyCode);
    bool isKeyJustPressed(int keyCode);
}
