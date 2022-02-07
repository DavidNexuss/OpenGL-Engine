#include "render_node.h"
#include "scene.h"
#include "material.h"
#include "mesh/primitiveMesh.h"
#include "renderer.h"
/*
MeshID RenderNode::screenQuad = -1;
RenderNode::RenderNode(CameraID _camera) : camera(_camera) { }
RenderNode::RenderNode(CameraID _camera,FrameBufferID _framebuffer) : camera(_camera), framebuffer(_framebuffer) { }
RenderNode::RenderNode(CameraID _camera,MaterialID _material) : camera(_camera), material(_material) { }

void RenderNode::render(int screenWidth,int screenHeight) { 
    if(children.size() == 0 && framebuffer != nullptr) {
        framebuffer->begin(screenWidth,screenHeight);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);  
        glEnable(GL_DEPTH_TEST); 
        
        Scene::camera = camera;
        Renderer::renderPass();
        framebuffer->end();
    }
    else {

        for (size_t i = 0; i < children.size(); i++) {
            children[i]->render(screenWidth,screenHeight);
        }

        Renderer::useMaterial(material);
        
        int last = 0;
        for (size_t i = 0; i < children.size(); i++) {
            const auto& textures = children[i]->framebuffer->colorAttachments;
            for (size_t j = 0; j < textures.size(); j++) {
                Loader::materials[material].bind(Renderer::worldMaterial);
                Loader::materials[material].bindScreenTexture(textures[j],last++);
            }
        }

        glCullFace(GL_FRONT);
        Renderer::useMesh(screenQuad);
        Renderer::drawMesh(false);
    }
}
RenderNode* RenderNode::setChildren(std::vector<RenderNode*>&& _children) {
    children = std::move(_children);
    return this;
}*/

MeshID RenderCamera::screenQuad;

RenderCamera::RenderCamera(CameraID _camera) : camera(_camera) { }

void RenderCamera::createScreenQuad()
{
    screenQuad = Loader::meshes.add(PrimitiveMesh::generateFromBuffers({
                {Standard::aPosition,2,{
                    -1.0,1.0,
                     1.0,1.0,
                    -1.0,-1.0,

                     1.0,1.0,
                     1.0,-1.0,
                     -1.0,-1.0,

                }}
            }));
}
void RenderCamera::render(int screenWidth,int screenHeight) { 

    for (size_t i = 0; i < renderCameraChildren.size(); i++) {
        Loader::renderCameras[renderCameraChildren[i]].render(screenWidth, screenHeight);
    }
    if(postProcessEffect.valid()) {
        Renderer::useMaterial(postProcessEffect);
        
        int last = 0;
        for (size_t i = 0; i < renderCameraChildren.size(); i++) {
            RenderCameraID child = renderCameraChildren[i];
            const auto& textures = child->renderBuffer->colorAttachments;
            for (size_t j = 0; j < textures.size(); j++) {
                postProcessEffect->bind(Renderer::worldMaterial);
                postProcessEffect->bindScreenTexture(textures[j],last++);
            }
        }

        glCullFace(GL_FRONT);
        if(!screenQuad.valid()) createScreenQuad();

        Renderer::useMesh(screenQuad);
        Renderer::drawMesh(false);
    }
    else if (renderBuffer.valid()) {
        renderBuffer->begin(screenWidth,screenHeight);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);  
        glEnable(GL_DEPTH_TEST); 
        
        Scene::camera = camera;
        Renderer::renderPass();
        renderBuffer->end();
    }
}

namespace Loader
{
    storage<RenderCamera> renderCameras;
}