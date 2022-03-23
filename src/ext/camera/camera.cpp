#include <ext/camera/camera.h>
#include "viewport.h"
#include "material.h"
#include "renderer.h"
#include <glm/ext.hpp>
#include <iostream>
#include <standard.h>
using namespace glm;
using namespace std;

DummyCamera::DummyCamera()
{
    fov = Standard::Configuration::defaultFov;
    zNear = Standard::Configuration::defaultZnear;
    zFar = Standard::Configuration::defaultZfar;

    useZoom = false;
    l = b = -1.0f;
    r = t = 1.0f;
    zmin = -2.0;
    zmax = 2.0;
    zoomSpeed = 0.0;
    zoomFactor = 1.0;
    
    useOrthographic = false;
}
glm::mat4 DummyCamera::createProjectionMatrix() {

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
glm::mat4 DummyCamera::createOrthoMatrix() {
    return glm::ortho(l,r,b,t,zmin,zmax);
}
    
void DummyCamera::defaultViewMatrix() {
    glm::mat4 viewMatrix = glm::lookAt(origin,target,glm::vec3(0,1,0));
    setViewMatrix(viewMatrix);
}
void DummyCamera::defaultProjectionMatrix() {
    setProjectionMatrix(useOrthographic ? createOrthoMatrix() : createProjectionMatrix());
}

void DummyCamera::updateMatrices() {
    combinedMatrix = projectionMatrix * viewMatrix;
    invViewMatrix = glm::inverse(viewMatrix);
}

void DummyCamera::lookAt(const glm::vec3& origin,const glm::vec3& target) {
    this->origin = origin;
    this->target = target;
}

void DummyCamera::update()
{
    defaultProjectionMatrix();
    defaultViewMatrix();
    updateMatrices();
}

void DummyCamera::bind(MaterialID current)
{
    glUniformMatrix4fv(current->uniforms[Standard::uProjectionMatrix],1,false,&projectionMatrix[0][0]);
    glUniform3fv(current->uniforms[Standard::uViewPos],1,&invViewMatrix[3][0]);
    
    if(current->isSkyboxMaterial) {
        mat4 skyView = mat4(mat3(viewMatrix));
        glUniformMatrix4fv(current->uniforms[Standard::uViewMatrix],1,false,&skyView[0][0]);
    }else 
        glUniformMatrix4fv(current->uniforms[Standard::uViewMatrix],1,false,&viewMatrix[0][0]);
}