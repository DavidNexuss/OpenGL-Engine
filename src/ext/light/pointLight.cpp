#include "pointLight.h"

PointLight::PointLight(glm::vec3 position, glm::vec3 color) {
    renderCamera = Loader::renderCameras.add(RenderCamera());
    light = Loader::lights.add(position, color);
    framebuffer = Loader::framebuffers.add(FrameBuffer(0,USE_DEPTH));
    shadowMaterial = Loader::materials.add(Material("depth"));
    camera = Loader::cameras.add(Camera());

    renderCamera->camera = camera;
    renderCamera->overrideMaterial = shadowMaterial;
    renderCamera->renderBuffer = framebuffer;
}