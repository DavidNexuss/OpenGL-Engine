#pragma once
#include "mesh.h"
#include "material.h"
#include "camera.h"

class RenderCamera
{
    
    int flushDepthBuffer = -1;
    int flushStencilBuffer = -1;
    static MeshID screenQuad;

    static void createScreenQuad();
    
    public:
    
    std::vector<size_t> renderCameraChildren;

    CameraID camera;
    MaterialID postProcessEffect;
    FrameBufferID renderBuffer;

    RenderCamera(CameraID camera);

    void render(int screenWidth, int screenHeight);
};
namespace Loader
{
    extern storage<RenderCamera> renderCameras;
}

using RenderCameraID = STORAGE_POINTER_TYPE(Loader::renderCameras);
