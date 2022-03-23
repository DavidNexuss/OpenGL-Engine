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
    
    void initCallbacks(Window* window);

    bool isKeyPressed(int keyCode);
    bool isKeyJustPressed(int keyCode);
    void hideMouse(bool hide);
}
