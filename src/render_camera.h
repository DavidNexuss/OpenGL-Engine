#pragma once
#include "mesh.h"
#include "material.h"
#include "ext/camera/camera.h"

enum class BindingMode {
    COLOR,
    DEPTH,
    STENCIL
};

struct BindingDescriptor {
    int renderCameraIndex;
    int colorAttributeIndex;
    int textureUnit;
    BindingMode mode;
    std::string uniformname;
};

class RenderCamera
{
    
    int flushDepthBuffer = -1;
    int flushStencilBuffer = -1;
    
    static MeshID screenQuad;
    static void createScreenQuad();
    
    bool renderAlways = true;
    bool renderCurrentFrame;
    
    Uniform getRenderResult(const BindingDescriptor& desc);
    public:
    
    std::vector<size_t> renderCameraChildren;
    std::vector<BindingDescriptor> bindingsDescriptors;

    CameraID camera;
    MaterialID postProcessEffect;
    FrameBufferID renderBuffer;
    MaterialID overrideMaterial;
    
    RenderCamera();
    RenderCamera(CameraID camera);

    void render(int screenWidth, int screenHeight);
    void setForRendering(bool enable);
    void setForAlwaysRendering(bool enable);

    void addRenderCamera(size_t renderCameraID);
    void addBinding(BindingDescriptor binding);
};

namespace Loader {
    extern storage<RenderCamera> renderCameras;
}

using RenderCameraID = STORAGE_POINTER_TYPE(Loader::renderCameras);
