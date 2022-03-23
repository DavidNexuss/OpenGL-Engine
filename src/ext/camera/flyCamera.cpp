#include "flyCamera.h"
#include <glm/ext.hpp>
#include <viewport.h>
using namespace glm;

FlyCamera::FlyCamera() {
    position = glm::vec3(0,0,0);
    fov = 90.0f;

    zoomSpeed = 0.0;
    zoomFactor = 1.0;
    velocityDamping = 0.2;
    phi = 0.0;
    setFrameUpdate(true);
}
void FlyCamera::createProjectionMatrix() {
    projectionMatrix = glm::perspective(glm::radians(fov * zoomFactor), float(Viewport::screenWidth) / float(Viewport::screenHeight), 0.1f, 500.0f);
}

void FlyCamera::createCombinedMatrix() {
    combinedMatrix = projectionMatrix * viewMatrix;   
}

void FlyCamera::update() {
    createProjectionMatrix();
    float a = ((Viewport::xpos / Viewport::screenWidth) - 0.5) * M_PI * 2;
    float b = ((Viewport::ypos / Viewport::screenHeight) - 0.5) * M_PI * 2;
    const auto deltaTime = 0.1f;

    glm::vec3 viewDir = glm::vec3(0,0,-1);

    viewDir.x = cos(a) * cos(b);
    viewDir.y = sin(b);
    viewDir.z = sin(a) * cos(b);

    glm::vec3 acceleration = viewDir * deltaTime;
    velocity += acceleration * deltaTime;
    position += (acceleration)  * deltaTime;
    
    viewMatrix = glm::lookAt(position,position + viewDir,glm::vec3(0,1,0));
    
    zoomSpeed += Viewport::scrollY * deltaTime;
    zoomFactor -= zoomSpeed * deltaTime;
    zoomSpeed -= zoomSpeed * deltaTime * zoomDamping;
    
    zoomFactor = std::max(zoomFactor,0.0f);
    Viewport::scrollY = 0.0;

    invViewMatrix = glm::inverse(viewMatrix);
}

void FlyCamera::bind(MaterialID current) {

    glUniformMatrix4fv(current->uniforms[Standard::uProjectionMatrix],1,false,&projectionMatrix[0][0]);
    glUniform3fv(current->uniforms[Standard::uViewPos],1,&invViewMatrix[3][0]);
    
    if(current->isSkyboxMaterial) {
        mat4 skyView = mat4(mat3(viewMatrix));
        glUniformMatrix4fv(current->uniforms[Standard::uViewMatrix],1,false,&skyView[0][0]);
    }else 
        glUniformMatrix4fv(current->uniforms[Standard::uViewMatrix],1,false,&viewMatrix[0][0]);
}
