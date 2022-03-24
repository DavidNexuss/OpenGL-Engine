#include "flyCamera.h"
#include <glm/ext.hpp>
#include <viewport.h>
using namespace glm;

FlyCamera::FlyCamera() {
    position = glm::vec3(0,0,0);
    velocityDamping = 0.2;
    advanceKey = GLFW_KEY_W;
    setFrameUpdate(true);
}

void FlyCamera::update() {

    //Viewport::hideMouse(true);
    float a = ((Viewport::xpos / Viewport::screenWidth) - 0.5) * M_PI * 2;
    float b = ((Viewport::ypos / Viewport::screenHeight) - 0.5) * M_PI * 2;
    const auto deltaTime = 0.1f;

    glm::vec3 viewDir = glm::vec3(0,0,-1);

    viewDir.x = cos(a) * cos(b);
    viewDir.y = sin(b);
    viewDir.z = sin(a) * cos(b);

    int ad = Viewport::isKeyPressed(GLFW_KEY_D) - Viewport::isKeyPressed(GLFW_KEY_A);
    int ws = Viewport::isKeyPressed(GLFW_KEY_W) - Viewport::isKeyPressed(GLFW_KEY_S);

    glm::vec3 input = glm::vec3(ad,0,ws);

    glm::vec3 accZ = viewDir * deltaTime * float(ws);
    glm::vec3 accX = glm::cross(viewDir,glm::vec3(0,1,0)) * deltaTime * float(ad);

    glm::vec3 acceleration = (accX + accZ) * 4.0f;
    velocity += acceleration * deltaTime;
    position += (acceleration)  * deltaTime;

    lookAt(position, position + viewDir);
    Camera::update();
}
