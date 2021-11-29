#include "framebuffer.h"


FrameBuffer::FrameBuffer(int attachmentCount) { 
    textureAttachments.resize(attachmentCount); 
}

void FrameBuffer::initialize() {
    glGenFramebuffers(1,&framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);

    //Generate color attachments
    for (size_t i = 0; i < textureAttachments.size(); i++) {
        GLuint texture;
        glGenTextures(1,&texture);
        glBindTexture(GL_TEXTURE_2D,texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bufferWidth, bufferHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture, 0);  
        textureAttachments[i] = texture; 
    }

    //Generate Stencil and depth buffer
    glGenTextures(1,&stencilDepthBuffer);
    glBindTexture(GL_TEXTURE_2D, stencilDepthBuffer);

    glTexImage2D(
      GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, bufferWidth, bufferHeight, 0, 
      GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
    );

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, stencilDepthBuffer, 0);  
    glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void FrameBuffer::dispose() {
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    glDeleteTextures(textureAttachments.size(),&textureAttachments[0]);
    glDeleteFramebuffers(1,&framebuffer);
}

void FrameBuffer::resize(int screenWidth, int screenHeight) {
    if(bufferWidth == screenWidth && bufferHeight == screenHeight) return;
    if(framebufferInitialized) dispose();

    bufferWidth = screenWidth;
    bufferHeight = screenHeight;

    initialize();
}

void FrameBuffer::check() { 
    bool error = glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE;
    if(error) {
        //Do error handling here
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