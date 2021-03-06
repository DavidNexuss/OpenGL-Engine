#include "viewport.h"
#include <gui/gui.h>
#include <unordered_map>

namespace Viewport
{
    Window* currentWindow;
    double screenWidth = 800,screenHeight = 600;
    double xpos,ypos;
    double scrollX,scrollY;
    double deltaTime = 0.1;

    std::unordered_map<int,bool> pressed;
    std::unordered_map<int,bool> justPressed;

    void cursor_position_callback(Window* window, double x, double y);
    void framebuffer_size_callback(Window* window, int width, int height);
    void scroll_callback(Window* window, double xoffset, double yoffset);
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
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

void Viewport::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    bool pressed = action == GLFW_PRESS || action == GLFW_REPEAT;
    Viewport::justPressed[key] = !Viewport::pressed[key] && pressed;
    Viewport::pressed[key] = pressed;
}

void Viewport::initCallbacks(Window* window) {
    glfwSetCursorPosCallback(window, Viewport::cursor_position_callback);
    glfwSetFramebufferSizeCallback(window, Viewport::framebuffer_size_callback);
    glfwSetScrollCallback(window, Viewport::scroll_callback);
    glfwSetKeyCallback(window,Viewport::key_callback);
    currentWindow = window;
}

void Viewport::hideMouse(bool hide) {
    glfwSetInputMode(currentWindow, GLFW_CURSOR, hide ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);  
}

bool Viewport::isKeyPressed(int keyCode){
    return Viewport::pressed[keyCode];
}

bool Viewport::isKeyJustPressed(int keyCode) {
    return Viewport::justPressed[keyCode];
}