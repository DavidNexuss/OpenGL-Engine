#include "render_camera.h"
#include "material.h"
#include "mesh/primitiveMesh.h"
#include "renderer.h"
#include "renderContext.h"

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
        RenderCameraID child = RenderCameraID(renderCameraChildren[i]);
        child->render(screenWidth, screenHeight);
    }
    if(postProcessEffect.valid()) {

        int last = 0;
        for (size_t i = 0; i < renderCameraChildren.size(); i++) {
            RenderCameraID child = renderCameraChildren[i];
            const auto& textures = child->renderBuffer->colorAttachments;
            for (size_t j = 0; j < textures.size(); j++) {
                postProcessEffect->bindScreenTexture(textures[j],last++);
            }
        }

        glCullFace(GL_FRONT);
        if(!screenQuad.valid()) createScreenQuad();

        Renderer::renderContext.useMesh(screenQuad);
        Renderer::renderContext.useMaterial(postProcessEffect);
        Renderer::drawMesh();
    }
    else if (renderBuffer.valid()) {
        renderBuffer->begin(screenWidth,screenHeight);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);  
        glEnable(GL_DEPTH_TEST); 
        
        Renderer::renderContext.useCamera(camera);
        Renderer::renderPass();
        renderBuffer->end();
    }
}


namespace Loader
{
    storage<RenderCamera> renderCameras;
}
