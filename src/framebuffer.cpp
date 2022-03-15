#include "framebuffer.h"
#include <iostream>

FrameBuffer::FrameBuffer(int attachmentCount) : FrameBuffer(attachmentCount, USE_DEPTH | USE_STENCIL) { }

FrameBuffer::FrameBuffer(int attachmentCount, FrameBufferDescriptorFlags flags) {
    colorAttachments.resize(attachmentCount);
    configuration = flags;

}

GLuint FrameBuffer::createDepthTexture() {
    GLuint depthTexture;
    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
             bufferWidth, bufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    return depthTexture;  
}

GLuint FrameBuffer::createStencilTexture() {
    GLuint stencilTexture;
    glGenTextures(1, &stencilTexture);
    glBindTexture(GL_TEXTURE_2D, stencilTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
             bufferWidth, bufferHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    return stencilTexture;

}
GLuint FrameBuffer::createDepthStencilBuffer() {

    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight);
    return rbo;
}

GLuint FrameBuffer::createDepthStencilTexture()
{
    GLuint rbot;
    glGenTextures(1,&rbot);
    glBindTexture(GL_TEXTURE_2D, rbot);

    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight, 0, 
      GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
    );
    return rbot;
}

void FrameBuffer::initialize() {
    glGenFramebuffers(1,&framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
	glGenTextures(colorAttachments.size(),&colorAttachments[0]);
	
    //Generate color attachments
    for (size_t i = 0; i < colorAttachments.size(); i++) {
        GLuint texture = colorAttachments[i];
        glBindTexture(GL_TEXTURE_2D,texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bufferWidth, bufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture, 0);  
    }

    if(combinedDepthStencil()) {
        if(configuration & USE_RENDER_BUFFER) {
            stencilDepthBuffer = createDepthStencilBuffer(); 
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, stencilDepthBuffer);
        }
        else {
            stencilDepthBuffer = createDepthStencilTexture();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencilDepthBuffer, 0);  
        }
    }
    else {
        if(configuration & USE_DEPTH) {
            depthBuffer = createDepthTexture();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthBuffer,0);
        }

        if(configuration & USE_STENCIL) {
            stencilBuffer = createStencilTexture();
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencilBuffer, 0);
        }
    }

    if(colorAttachments.size() == 0) {
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    std::vector<unsigned int> buffers;
    buffers.reserve(16);
    for (size_t i = 0; i < colorAttachments.size(); i++) {
        buffers.push_back(GL_COLOR_ATTACHMENT0 + i);
    }
    glDrawBuffers(colorAttachments.size(), buffers.data());

    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void FrameBuffer::dispose() {
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glDeleteTextures(colorAttachments.size(),&colorAttachments[0]);
    
    if(combinedDepthStencil()) {
        if(configuration & USE_RENDER_BUFFER) glDeleteRenderbuffers(1,&stencilDepthBuffer);
        else glDeleteTextures(1,&stencilDepthBuffer);
    }
    else {
        if(configuration & USE_DEPTH) {
            glDeleteTextures(1,&depthBuffer);
        }
        if(configuration & USE_STENCIL) {
            glDeleteTextures(1,&stencilBuffer);
        }
    }    

    glDeleteFramebuffers(1,&framebuffer);
}

void FrameBuffer::resize(int screenWidth, int screenHeight) {
    if(bufferWidth == screenWidth && bufferHeight == screenHeight) return;
    if(framebufferInitialized) dispose();

    bufferWidth = screenWidth;
    bufferHeight = screenHeight;
	
    initialize();
	check();
}

void FrameBuffer::check() { 
    bool error = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;
    if(error) {
    
	}
}

void FrameBuffer::begin(int screenWidth, int screenHeight) {
    resize(screenWidth, screenHeight);
    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::end() { 
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBuffer::getDepthBuffer() const
{
    if(combinedDepthStencil()) return stencilDepthBuffer;
    return depthBuffer;
}
GLuint FrameBuffer::getStencilBuffer() const
{
    if(combinedDepthStencil()) return stencilDepthBuffer;
    return stencilBuffer;
}

namespace Loader
{
    storage<FrameBuffer> framebuffers;
};
