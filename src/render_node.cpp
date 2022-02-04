#include "render_node.h"
#include "scene.h"
#include "material.h"

MeshID RenderNode::screenQuad = -1;
RenderNode::RenderNode(CameraID _camera) : camera(_camera) { }
RenderNode::RenderNode(CameraID _camera,FrameBuffer* _framebuffer) : camera(_camera), framebuffer(_framebuffer) { }
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
}
