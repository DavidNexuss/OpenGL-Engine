#pragma once
#include <light.h>
#include <ext/camera/camera.h>
#include <framebuffer.h>
#include <render_camera.h>
#include <material.h>

class PointLight
{
    MaterialID shadowMaterial;
    CameraID camera;
    FrameBufferID framebuffer;
    RenderCameraID renderCamera;
    LightID light;

    public:
    PointLight(glm::vec3 position, glm::vec3 color);

    bool update();

    inline FrameBufferID getFramebuffer() const { return framebuffer; }
    inline RenderCameraID getRenderCamera() const { return renderCamera; }
    inline LightID getLight() const { return light; }
};