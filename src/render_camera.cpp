#include "render_camera.h"
#include "material.h"
#include "mesh/primitiveMesh.h"
#include "renderer.h"

MeshID RenderCamera::screenQuad;

RenderCamera::RenderCamera(CameraID _camera) : camera(_camera) { }
RenderCamera::RenderCamera() { }
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

Uniform RenderCamera::getRenderResult(const BindingDescriptor& desc) {
    RenderCameraID child = (renderCameraChildren[desc.renderCameraIndex]);
    GLuint texture;
    switch (desc.mode) {
        case BindingMode::COLOR:
            texture = child->renderBuffer->colorAttachments[desc.colorAttributeIndex]; break;
        case BindingMode::DEPTH:
            texture = child->renderBuffer->getDepthBuffer(); break;
        case BindingMode::STENCIL:
            texture = child->renderBuffer->getStencilBuffer(); break;
    }
    return Uniform(UTexture(texture,desc.textureUnit));
}
void RenderCamera::render(int screenWidth,int screenHeight) { 

    if(!(renderAlways || renderCurrentFrame)) return;
    renderCurrentFrame = false;
	
    //Render each child
    for (size_t i = 0; i < renderCameraChildren.size(); i++) {
        RenderCameraID child = RenderCameraID(renderCameraChildren[i]);
        child->render(screenWidth, screenHeight);
    }

	
	// Render post effect shader if valid
    if(postProcessEffect.valid()) {

        Renderer::useMaterial(postProcessEffect);
        for(BindingDescriptor& desc : bindingsDescriptors) {
            Renderer::currentMaterial.bindUniform(desc.uniformname, getRenderResult(desc));
        }
        /*
        int last = 0;
        for (size_t i = 0; i < renderCameraChildren.size(); i++) {
            RenderCameraID child = renderCameraChildren[i];
            const auto& textures = child->renderBuffer->colorAttachments;
            for (size_t j = 0; j < textures.size(); j++) {
                postProcessEffect->bindScreenTexture(textures[j],last++);
            }
        }*/

        glCullFace(GL_FRONT);
        if(!screenQuad.valid()) createScreenQuad();

        Renderer::useMesh(screenQuad);
        Renderer::drawMesh();
    }
	//If not do a render pass for the renderCamera buffer and camera
    else if (renderBuffer.valid()) {
        renderBuffer->begin(screenWidth,screenHeight);
        
        if(overrideMaterial.valid()) { 
            Renderer::overrideMaterial(overrideMaterial);
        }
        Renderer::useWorldMaterial(Standard::wCamera,camera);
        Renderer::renderPass();
        if(overrideMaterial.valid()) Renderer::overrideMaterial(MaterialID::invalidInstance());
        
        renderBuffer->end();
    }
    else {
        for(BindingDescriptor& desc : bindingsDescriptors) {
            Renderer::globalWorldMaterial->setUniform(desc.uniformname,getRenderResult(desc));
        }

        Renderer::useWorldMaterial(Standard::wCamera,camera);
        Renderer::renderPass();
    }
}

void RenderCamera::setForRendering(bool enable) {
    renderCurrentFrame = enable;
}
void RenderCamera::setForAlwaysRendering(bool enable) {
    renderAlways = enable;
}
namespace Loader
{
    storage<RenderCamera> renderCameras;
}
