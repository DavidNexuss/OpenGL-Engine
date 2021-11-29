#pragma once
#include "core.h"
#include <vector>

class FrameBuffer
{
    GLuint framebuffer, stencilDepthBuffer;
    
    bool framebufferInitialized = false;
    int bufferWidth = 0, bufferHeight = 0;


    void initialize();
    void dispose();
    void resize(int screenWidth, int screenHeight);
    void check();

    public:

    std::vector<GLuint> textureAttachments;
    FrameBuffer(int attachmentCount);

    void begin(int screenWidth, int screenHeight);
    void end();
};