#include "renderer.h"
#include "scene.h"

class RenderNode
{
    
    std::vector<RenderNode*> children;
    CameraID camera = -1;                    //renderPassCamera
    MaterialID material = -1;                //postProcessMaterial
    FrameBuffer* framebuffer = nullptr;

    int flushDepthBuffer = -1;
    int flushStencilBuffer = -1;
    
    public:
    static MeshID screenQuad;

    RenderNode(CameraID _camera);                                                  //Render Node
    RenderNode(CameraID _camera,FrameBuffer* _framebuffer);                        //Render Node
    RenderNode(CameraID _camera,MaterialID _material);                             //Post effects node

    void render(int screenWidth, int screenHeight);
    RenderNode* setChildren(std::vector<RenderNode*>&& children);

    inline RenderNode* mapDepthBuffer(int id) {
        flushDepthBuffer = id;
        return this;
    }
};