#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "structures/storage.h"

enum CameraType
{
    THIRDPERSON = 0,
    THIRDPERSON_MANUAL,
    ORTHOGONAL,
    FLY,
    CUSTOM
};

/** 
 * @class Camera
 * @brief Engine Camera abstraction
 */

class Camera
{
    public:
    CameraType type;
    
    glm::vec3 focusOrigin;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 invViewMatrix;

    float zoomDamping = 0.6;
    float zoomFactor;
    float zoomSpeed;
    float fov;
    float phi,zheta,gamma;       //For manual controlling
    float d;
    float l,r,b,t,zmin,zmax;

    /**
     * @brief updates camera matrices using the currnet camera configuration
     */
    void update();

    /**
     * @brief sends camera uniforms to the shaders
     */
    void flush();

    Camera();
};

namespace Loader
{
    extern storage<Camera> cameras;
}

using CameraID = STORAGE_POINTER_TYPE(Loader::cameras);