#pragma once

#include <vector>
#include <glm/glm.hpp>

enum CameraType
{
    THIRDPERSON = 0,
    THIRDPERSON_MANUAL,
    ORTHOGONAL,
    FLY,
    CUSTOM
};

const float zoomDaping = 0.6;

class Camera
{
    public:
    CameraType type;
    
    glm::vec3 focusOrigin;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 invViewMatrix;

    float zoomFactor;
    float zoomSpeed;
    float fov;
    float phi,zheta,gamma;       //For manual controlling
    float d;
    float l,r,b,t,zmin,zmax;


    void update();
    void flush();

    Camera();
};

using CameraID = size_t;

namespace CameraLoader
{
    extern std::vector<Camera> cameras;

    inline static CameraID loadCamera(const Camera& camera)
    {
        cameras.push_back(camera);
        return cameras.size() - 1;
    }
};