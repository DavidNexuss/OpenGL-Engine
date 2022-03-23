#pragma once
#include "mesh.h"
#include "material.h"
#include "ext/camera/camera.h"

class RenderCamera
{
    
    int flushDepthBuffer = -1;
    int flushStencilBuffer = -1;
    
    static MeshID screenQuad;
    static void createScreenQuad();
    
    bool renderAlways = true;
    bool renderCurrentFrame;
    
    public:
    
    std::vector<size_t> renderCameraChildren;

    CameraID camera;
    MaterialID postProcessEffect;
    FrameBufferID renderBuffer;
    MaterialID overrideMaterial;
    
    RenderCamera();
    RenderCamera(CameraID camera);
    void render(int screenWidth, int screenHeight);
    void setForRendering(bool enable);
    void setForAlwaysRendering(bool enable);
};

namespace Loader {
    extern storage<RenderCamera> renderCameras;
}

using RenderCameraID = STORAGE_POINTER_TYPE(Loader::renderCameras);
