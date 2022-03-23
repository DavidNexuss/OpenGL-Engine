#pragma once
#include "camera.h"
#include <glm/glm.hpp>
#include "structures/storage.h"
#include "material.h"

enum DummyCameraType
{
    THIRDPERSON = 0,
    THIRDPERSON_MANUAL,
    ORTHOGONAL,
    FLY,
    CUSTOM
};

/** 
 * @class DummyCamera
 * @brief Engine Camera abstraction
 */

class DummyCamera : public Camera
{
    public:
    DummyCameraType type;
    
    glm::vec3 focusOrigin;
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 invViewMatrix;

    float zoomDamping = 0.6;
    float zoomFactor;
    float zoomSpeed;
    float fov;
    float phi,zheta,gamma;       
    float d;
    float l,r,b,t,zmin,zmax;

    /**
     * @brief updates camera matrices using the currnet camera configuration
     */
    virtual void update();

    /**
     * @brief sends camera uniforms to the shaders
     */
    virtual void bind(MaterialID material);

    DummyCamera();

    inline static CameraID create() { return new DummyCamera(); }
};