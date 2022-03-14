#include "renderer.h"
#include "model.h"
#include "viewport.h"
#include "light.h"
#include "world_material.h"
#include <mesh/primitiveMesh.h>
#include <iostream>
#include "render_camera.h"
#include "renderContext.h"

RenderConfiguration Renderer::currentConfiguration;

namespace Renderer
{
    RenderCameraID mainRenderCamera;
    ModelID skyModel;

    void configureRenderer(const RenderConfiguration& config)
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);  
        glEnable(GL_DEPTH_TEST); 
        if(config.mssaLevel > 0)glEnable(GL_MULTISAMPLE);
        if(config.wireRendering) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }
        else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
        currentConfiguration = config;
    }

    void renderPass()
    {
        glCullFace(GL_BACK);

        if(skyModel.valid() && skyModel->ready() && !currentConfiguration.skipSkybox)
            skyModel->draw();

        auto models = Loader::models.getSortedView();

        //Render world
        for(size_t i = 0; i < models.size(); i++) {
            if(!models[i].enabled) continue;

            models[i].process();
            models[i].draw();
        }
    }
    void render() {
        renderContext.registerFrame();

        glClearColor(currentConfiguration.clearColor.x, currentConfiguration.clearColor.y, currentConfiguration.clearColor.z,1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        if(mainRenderCamera.valid()) {
            mainRenderCamera->render(Viewport::screenWidth,Viewport::screenHeight);
        } else renderPass();

        Loader::lights.flushClean();
    }
};
