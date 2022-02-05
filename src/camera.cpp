#include "camera.h"
#include "viewport.h"
#include "material.h"
#include "renderer.h"
#include <glm/ext.hpp>
#include <iostream>

using namespace glm;
using namespace std;

Camera::Camera()
{
    fov = 90.0f;
    focusOrigin = vec3(0,0,0);
    l = b = -1.0f;
    r = t = 1.0f;
    zmin = -2.0;
    zmax = 2.0;
    zoomSpeed = 0.0;
    zoomFactor = 1.0;
    d = 5.0;
    type = THIRDPERSON;
    gamma = 0.0;
    zheta = 0.0;
    phi = 0.0;
}


void Camera::update()
{
    if(type == THIRDPERSON || type == THIRDPERSON_MANUAL)
    {
        projectionMatrix = glm::perspective(glm::radians(fov * zoomFactor), float(Viewport::screenWidth) / float(Viewport::screenHeight), 0.1f, 500.0f);
        float a = gamma,b = zheta;
        if(type == THIRDPERSON)
        {
            a = ((Viewport::xpos / Viewport::screenWidth) - 0.5) * M_PI * 3;
            b = ((Viewport::ypos / Viewport::screenHeight) - 0.5) * M_PI * 3;
        }

        const auto deltaTime = 0.1f;
        zoomSpeed += Viewport::scrollY * deltaTime;
        zoomFactor -= zoomSpeed * deltaTime;
        zoomSpeed -= zoomSpeed * deltaTime * zoomDamping;
        
        zoomFactor = std::max(zoomFactor,0.0f);

        Viewport::scrollY = 0.0;

        viewMatrix = mat4(1.0);
        viewMatrix = translate(viewMatrix,vec3(0,0,-d));

        viewMatrix = rotate(viewMatrix,-glm::radians(phi),vec3(0,0,1));
        viewMatrix = rotate(viewMatrix,b,vec3(1,0,0));          //  b
        viewMatrix = rotate(viewMatrix,a,vec3(0,1,0));          // -a
        viewMatrix = translate(viewMatrix,-focusOrigin);
    }

    if  (type == ORTHOGONAL)
    {
        projectionMatrix = glm::ortho(l,r,b,t,zmin,zmax);
    }
    invViewMatrix = glm::inverse(viewMatrix);

}

void Camera::flush()
{
        Material& current = Loader::materials[Renderer::currentMaterial];
        glUniformMatrix4fv(current.uniforms[Standard::uProjectionMatrix],1,false,&projectionMatrix[0][0]);
        glUniform3fv(current.uniforms[Standard::uViewPos],1,&invViewMatrix[3][0]);
        
        if(current.isSkyboxMaterial)
        {
            mat4 skyView = mat4(mat3(viewMatrix));
            glUniformMatrix4fv(current.uniforms[Standard::uViewMatrix],1,false,&skyView[0][0]);
        }else glUniformMatrix4fv(current.uniforms[Standard::uViewMatrix],1,false,&viewMatrix[0][0]);
}

namespace Loader
{
    storage<Camera> cameras;
}