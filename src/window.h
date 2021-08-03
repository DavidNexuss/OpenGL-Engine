#pragma once
#include "glfw.h"
#include <string>

using Window = GLFWwindow;
Window* createWindow(const std::string& titleName,int width,int height,int openglMajorVersion,int openglMinorVersion,int mssaLevel);