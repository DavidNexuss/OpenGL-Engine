#pragma once
#include <ext/camera/camera.h>
class FlyCamera : public Camera
{
    glm::vec3 position;
    glm::vec3 velocity;
    
    float velocityDamping;
    int advanceKey; 

    public:
    virtual void update();

    FlyCamera();
};