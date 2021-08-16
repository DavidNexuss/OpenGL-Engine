#pragma once
#include <glfw.h>
#include <texture.h>
#include <vector>

/**
 * @class OpenGL frameBuffer abstraction to draw screen to a buffer using native or scaled resolution
 */

class FrameBuffer
{
    static Texture createFrameBufferTexture(int width,int height);

    GLuint fb = 0,depth = 0;

    bool framebuffer_initialized = false;
    
    int screen_width = 0;
    int screen_height = 0;

    float resize_factor = 1.0;
    float old_factor;

    int fbo_width;
    int fbo_height;
    
    void initialize_framebuffer();
    void initialize_textureAttach();
    void dispose_framebuffer();
    void resize(int screenWidth, int screenHeight);


    public:
    std::vector<Texture> textureAttachments;
    
    FrameBuffer(int attachmentCount = 0);
    ~FrameBuffer();

    float get_render_width() const;
    float get_render_height() const;
    
    void set_resize_factor(float factor);
    float get_resize_factor() const;

    void begin(int screenWidth,int screenHeight);
    void end(bool flushToScreen = false);

    void setAttachmentCount(int attachmentCount);
};
