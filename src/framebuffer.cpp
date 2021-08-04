#include "framebuffer.h"
#include <iostream>
#include <vector>
#include <thread>
#include <cassert>

using namespace std;

FrameBuffer::FrameBuffer() : FrameBuffer(1.0) { }
FrameBuffer::FrameBuffer(float resizeFactor)
{
    resize_factor = resizeFactor;
}
FrameBuffer::FrameBuffer(int screenWidth, int screenHeight,float resizeFactor,GLuint shaderRes)
{
    resize_factor = resizeFactor;
    resize(screenWidth, screenHeight, shaderRes);
}
FrameBuffer::~FrameBuffer()
{
    dispose_framebuffer();
}

float FrameBuffer::get_render_width() const
{
    return screen_width * resize_factor;
}
float FrameBuffer::get_render_height() const
{
    return screen_height * resize_factor;
}
void FrameBuffer::set_resize_factor(float factor)
{
    old_factor = resize_factor;
    resize_factor = factor;
}
void FrameBuffer::resize(int screenWidth, int screenHeight,GLuint shaderRes)
{
    dispose_framebuffer();
    screen_width = screenWidth;
    screen_height = screenHeight;
    float resolution[2] = {get_render_width(), get_render_height()};
    glUniform2fv(shaderRes,1,resolution);
    initialize_framebuffer();
    old_factor = resize_factor;
}

void FrameBuffer::flush(GLuint shaderRes)
{
    resize(screen_width,screen_height,shaderRes);
}
float FrameBuffer::get_resize_factor() const
{
    return resize_factor;
}

void FrameBuffer::initialize_framebuffer()
{
    assert(!framebuffer_initialized);
    glGenFramebuffers(1, &fb);
    glGenTextures(1, &color);
    glGenRenderbuffers(1, &depth);

    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    fbo_width = get_render_width();
    fbo_height = get_render_height();

    glBindTexture(GL_TEXTURE_2D, color);
    glTexImage2D(   GL_TEXTURE_2D, 
            0, 
            GL_RGB, 
            fbo_width, fbo_height,
            0, 
            GL_RGB, 
            GL_UNSIGNED_BYTE, 
            NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, get_render_width(), get_render_height());
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
    framebuffer_initialized = true;
}

void FrameBuffer::dispose_framebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDeleteFramebuffers(1,&fb);
    glDeleteTextures(1, &color);
    glDeleteRenderbuffers(1, &depth);
    framebuffer_initialized = false;
}

void FrameBuffer::begin(GLuint shaderRes) {
    begin(screen_width,screen_height,shaderRes);
}

void FrameBuffer::begin(int screenWidth, int screenHeight,GLuint shaderRes)
{
    if (old_factor != resize_factor || fb == 0 || screenWidth != screen_width || screenHeight != screen_height) resize(screenWidth, screenHeight,shaderRes);

    glBindTexture(GL_TEXTURE_2D, 0);
    glEnable(GL_TEXTURE_2D);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    glViewport(0,0, get_render_width(), get_render_height());

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void FrameBuffer::end()
{   
    //Draw framebuffer to screen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0,0, screen_width, screen_height);

    glBindFramebuffer(GL_READ_FRAMEBUFFER, fb);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, color);

    glBlitFramebuffer(0, 0, get_render_width(), get_render_height(), 0, 0, screen_width, screen_height,
                  GL_COLOR_BUFFER_BIT, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER,0);

}