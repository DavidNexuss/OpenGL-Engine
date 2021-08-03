#include "window.h"
#include "debug.h"
#include <iostream>

using namespace std;

Window* createWindow(const std::string& title,int width,int height,int openglMajorVersion,int openglMinorVersion,int mssaLevel)
{
    glewExperimental = true;
    if(!glfwInit())
    {
        cerr << "[ERROR] Could not initialize GLFW" << endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_SAMPLES, mssaLevel);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, openglMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, openglMinorVersion);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window* window;
    window = glfwCreateWindow( width, height, title.c_str(), NULL, NULL);
    if( window == nullptr)
    {
        cerr << "[ERROR] Could not create window" << endl;
        cerr << "Creating window with: " << title << endl;
        cerr << "Width: " << width << endl;
        cerr << "Height: " << height << endl;
        cerr << "OpenGL: " << openglMajorVersion << endl;
        cerr << "OpenGL Minor Version: " << openglMinorVersion << endl;
        cerr << "MSSA Level: " << mssaLevel << endl;
        
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
