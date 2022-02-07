#pragma once
#include "scene.h"
#include "mesh.h"
#include "material.h"
#include "camera.h"
/*
class RenderCamera
{
    
    std::vector<RenderNode*> children;
    CameraID camera = -1;                    //renderPassCamera
    MaterialID material = -1;                //postProcessMaterial
    FrameBufferID framebuffer;

    int flushDepthBuffer = -1;
    int flushStencilBuffer = -1;
    
    public:
    static MeshID screenQuad;

    RenderNode(CameraID _camera);                                                  //Render Node
    RenderNode(CameraID _camera,FrameBufferID _framebuffer);                        //Render Node
    RenderNode(CameraID _camera,MaterialID _material);                             //Post effects node

    void render(int screenWidth, int screenHeight);
    RenderNode* setChildren(std::vector<RenderNode*>&& children);

    inline RenderNode* mapDepthBuffer(int id) {
        flushDepthBuffer = id;
        return this;
    }
};*/

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