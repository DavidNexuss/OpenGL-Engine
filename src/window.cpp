#include "window.h"
#include "debug.h"
#include <iostream>

using namespace std;

Window* createWindow(const std::string& title)
{
    glewExperimental = true;
    if(!glfwInit())
    {
        cerr << "[ERROR] Could not initialize GLFW" << endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    
    Window* window;
    window = glfwCreateWindow( 1024, 768, title.c_str(), NULL, NULL);
    if( window == nullptr)
    {
        cerr << "[ERROR] Could not create window" << endl;
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glewExperimental=true;

    if (glewInit() != GLEW_OK) {
        cerr << "[ERROR] Could not initialize glew" << endl;
        return nullptr;
    }

    return window;
}
