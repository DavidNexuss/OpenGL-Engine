#include "framebuffer.h"
#include <iostream>
#include <vector>
#include <thread>
#include <cassert>

using namespace std;

FrameBuffer::FrameBuffer(int attachmentCount) {
    setAttachmentCount(attachmentCount);
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
void FrameBuffer::resize(int screenWidth, int screenHeight)
{
    if(screen_width == screenWidth && screen_height == screenHeight) return;
    dispose_framebuffer();
    screen_width = screenWidth;
    screen_height = screenHeight;
    initialize_framebuffer();
    old_factor = resize_factor;
}

float FrameBuffer::get_resize_factor() const
{
    return resize_factor;
}

void FrameBuffer::setAttachmentCount(int attachmentCount)
{
    textureAttachments.resize(attachmentCount);
}
Texture FrameBuffer::createFrameBufferTexture(int width, int height)
{
    Texture tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexImage2D(GL_TEXTURE_2D, 
            0, 
            GL_RGB, 
            width, height,
            0, 
            GL_RGB, 
            GL_UNSIGNED_BYTE, 
            NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    return tex;
        
}
void FrameBuffer::initialize_framebuffer()
{
    assert(!framebuffer_initialized);
    glGenFramebuffers(1, &fb);
    glGenRenderbuffers(1, &depth);

    glBindFramebuffer(GL_FRAMEBUFFER, fb);

    fbo_width = get_render_width();
    fbo_height = get_render_height();

    for (size_t i = 0; i < textureAttachments.size(); i++)
    {
        textureAttachments[i] = createFrameBufferTexture(fbo_width, fbo_height);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, textureAttachments[i], 0);
    }

    glBindRenderbuffer(GL_RENDERBUFFER, depth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, get_render_width(), get_render_height());
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depth);
    
    assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
    framebuffer_initialized = true;

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void FrameBuffer::dispose_framebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDeleteFramebuffers(1,&fb);
    glDeleteRenderbuffers(1, &depth);
    glDeleteTextures(textureAttachments.size(),textureAttachments.data());
    
    framebuffer_initialized = false;
}

void FrameBuffer::begin(int screenWidth,int screenHeight)
{
    resize(screenWidth,screenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER,fb);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::end(bool flushToScreen)
{   
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    if(flushToScreen)
    {
        glViewport(0,0, screen_width, screen_height);

        glBindFramebuffer(GL_READ_FRAMEBUFFER, fb);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        glEnable(GL_TEXTURE_2D);
       // glBindTexture(GL_TEXTURE_2D, color); TODO : FIX

        glBlitFramebuffer(0, 0, get_render_width(), get_render_height(), 0, 0, screen_width, screen_height,
                      GL_COLOR_BUFFER_BIT, GL_NEAREST);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}