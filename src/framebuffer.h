#pragma once
#include "core.h"
#include "macros.h"
#include "structures/storage.h"

enum FrameBufferDescriptorFlags {
    USE_RENDER_BUFFER =         1 << 0,
    USE_DEPTH =                 1 << 1,
    USE_STENCIL =               1 << 2,
    SEPARATE_DEPTH_STENCIL =    1 << 3,
    ONLY_WRITE_DEPTH =          1 << 4,
    ONLY_READ_DEPTH =           1 << 5,
};

ENUM_OPERATORS(FrameBufferDescriptorFlags)

class FrameBuffer
{
    FrameBufferDescriptorFlags configuration;
    GLuint framebuffer;
    
    bool framebufferInitialized = false;
    int bufferWidth = 0, bufferHeight = 0;


    void initialize();
    void dispose();
    void resize(int screenWidth, int screenHeight);
    void check();
    
    GLuint createDepthStencilTexture();
    GLuint createDepthStencilBuffer();

    GLuint createDepthTexture();
    GLuint createStencilTexture();

    inline bool combinedDepthStencil() const {
        return configuration & USE_DEPTH && configuration & USE_STENCIL && !(configuration & SEPARATE_DEPTH_STENCIL);
    }
    public:

    GLuint stencilDepthBuffer;
    
    GLuint depthBuffer;
    GLuint stencilBuffer;

    std::vector<GLuint> colorAttachments;

    FrameBuffer(int attachmentCount);
    FrameBuffer(int attachmentCount, FrameBufferDescriptorFlags flags);

    void begin(int screenWidth, int screenHeight);
    void end();

    inline int getWidth() const { return bufferWidth; }
    inline int getHeight() const { return bufferHeight; }

    GLuint getDepthBuffer() const;
    GLuint getStencilBuffer() const;
};

namespace Loader
{
    extern storage<FrameBuffer> framebuffers;
};

using FrameBufferID = STORAGE_POINTER_TYPE(Loader::framebuffers);
