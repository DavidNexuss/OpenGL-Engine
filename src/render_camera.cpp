#include "render_camera.h"
#include "material.h"
#include "mesh/primitiveMesh.h"
#include "renderer.h"

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

	//Render each child
    for (size_t i = 0; i < renderCameraChildren.size(); i++) {
        RenderCameraID child = RenderCameraID(renderCameraChildren[i]);
        child->render(screenWidth, screenHeight);
    }
	
	// Render post effect shader if valid
    if(postProcessEffect.valid()) {

        Renderer::useMaterial(postProcessEffect);
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

        Renderer::useMesh(screenQuad);
        Renderer::drawMesh();
    }
	//If not do a render pass for the renderCamera buffer and camera
    else if (renderBuffer.valid()) {
        renderBuffer->begin(screenWidth,screenHeight);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);  
        glEnable(GL_DEPTH_TEST); 
        
        Renderer::useCamera(camera);
        Renderer::renderPass();
        renderBuffer->end();
    }
}


namespace Loader
{
    storage<RenderCamera> renderCameras;
}
