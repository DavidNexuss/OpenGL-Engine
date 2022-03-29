#pragma once
#include <glm/glm.hpp>
#include <world_material.h>
#include <activeMaterial.h>

/** 
 * @class Camera
 * @brief Engine Camera abstraction
 */

class Camera : public WorldMaterial
{
    glm::vec3 origin;
    glm::vec3 target;

    glm::mat4 viewMatrix;
    glm::mat4 invViewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 combinedMatrix;

    public:

    float fov;
    float zoomDamping = 0.6;
    float zoomFactor;
    float zoomSpeed;
    float zNear;
    float zFar;
    bool useZoom;
    bool useOrthographic;

    float l,r,b,t,zmin,zmax;

    glm::mat4 createProjectionMatrix();
    glm::mat4 createOrthoMatrix();
    

    void updateMatrices();

    inline void setViewMatrix(const glm::mat4& _viewMatrix) { viewMatrix = _viewMatrix; }
    inline void setProjectionMatrix(const glm::mat4& _projectionMatrix) { projectionMatrix = _projectionMatrix; }
    
    void defaultProjectionMatrix();
    void defaultViewMatrix();
    
    void lookAt(const glm::vec3& position,const glm::vec3& target);

    /**
     * @brief updates camera matrices using the currnet camera configuration
     */
    virtual void update();

    /**
     * @brief sends camera uniforms to the shaders
     */
    virtual void bind(ActiveMaterial material);

    Camera();
};

using CameraID = Camera*;