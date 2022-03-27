#include <ext/camera/camera.h>
#include "viewport.h"
#include "material.h"
#include "renderer.h"
#include <glm/ext.hpp>
#include <iostream>
#include <standard.h>

using namespace glm;
using namespace std;

Camera::Camera()
{
    fov = 90.0f;
    zNear = 0.1f;
    zFar = 500.0f;

    useZoom = false;
    l = b = -1.0f;
    r = t = 1.0f;
    zmin = -2.0;
    zmax = 2.0;
    zoomSpeed = 0.0;
    zoomFactor = 1.0;
    
    useOrthographic = false;
}
glm::mat4 Camera::createProjectionMatrix() {

    float zoom = useZoom ? zoomFactor : 1.0f;

    if(useZoom) {
        const auto deltaTime = 0.1f;
        zoomSpeed += Viewport::scrollY * deltaTime;
        zoomFactor -= zoomSpeed * deltaTime;
        zoomSpeed -= zoomSpeed * deltaTime * zoomDamping;
        zoomFactor = std::max(zoomFactor,0.0f);
    }
    return glm::perspective(glm::radians(fov * zoom), float(Viewport::screenWidth) / float(Viewport::screenHeight), zNear, zFar);
}
glm::mat4 Camera::createOrthoMatrix() {
    return glm::ortho(l,r,b,t,zmin,zmax);
}
    
void Camera::defaultViewMatrix() {
    glm::mat4 viewMatrix = glm::lookAt(origin,target,glm::vec3(0,1,0));
    setViewMatrix(viewMatrix);
}
void Camera::defaultProjectionMatrix() {
    setProjectionMatrix(useOrthographic ? createOrthoMatrix() : createProjectionMatrix());
}

void Camera::updateMatrices() {
    combinedMatrix = projectionMatrix * viewMatrix;
    invViewMatrix = glm::inverse(viewMatrix);
}

void Camera::lookAt(const glm::vec3& origin,const glm::vec3& target) {
    this->origin = origin;
    this->target = target;
}

void Camera::update()
{
    defaultProjectionMatrix();
    defaultViewMatrix();
    updateMatrices();
}

void Camera::bind(MaterialID current)
{
    current->bindUniform(Standard::uProjectionMatrix,Uniform(&projectionMatrix));
    current->bindUniform(Standard::uViewPos,Uniform(&invViewMatrix[3]));
    
    if(current->isSkyboxMaterial) {
        mat4 skyView = mat4(mat3(viewMatrix));
        current->bindUniform(Standard::uViewMatrix,Uniform(&skyView));
    }else 
        current->bindUniform(Standard::uViewMatrix,Uniform(&viewMatrix));
}
