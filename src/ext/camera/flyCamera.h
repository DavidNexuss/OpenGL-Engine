#pragma once
#include <ext/camera/camera.h>
class FlyCamera : public Camera
{
    glm::vec3 position;
    glm::vec3 velocity;
    float velocityDamping;
    
    glm::mat4 viewMatrix;
    glm::mat4 invViewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 combinedMatrix;

    float phi;
    float fov;
    float zoomFactor;
    float zoomDamping;
    float zoomSpeed;

    void createProjectionMatrix();
    void createCombinedMatrix();

    public:
    virtual void update();
    virtual void bind(MaterialID material);
    inline static CameraID create() { return new FlyCamera(); }

    FlyCamera();
};