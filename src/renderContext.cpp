#include "renderContext.h"
#include "light.h"

void RenderContext::useMaterial(MaterialID materialID)
{
    if(currentMaterial != materialID) {
        currentMaterial = materialID;
        currentMaterial->bind();
        REGISTER_MATERIAL_SWAP();
    }

    if (Loader::materials.updateForFrame(materialID,currentFrame)) {
        Loader::lights.flush(currentMaterial);

        if(currentCamera.valid()) 
            currentCamera->bind(currentMaterial);        
        
        if(currentWorldMaterial != nullptr)
            currentWorldMaterial->bind(currentMaterial);
    }

}

void RenderContext::useMesh(MeshID meshID)
{
    if (meshID != currentMesh) {
        currentMesh = meshID;
        glBindVertexArray(currentMesh->vao);
        REGISTER_MESH_SWAP();
    }
}

void RenderContext::useCamera(CameraID cameraID) {
    if(cameraID != currentCamera) {
        currentCamera = cameraID;
        currentCamera->bind(currentMaterial);
    }
}   

void RenderContext::useMaterialInstance(MaterialInstanceID instanceID) {
    currentMaterial->useInstance(instanceID);
}

void RenderContext::useWorldMaterial(WorldMaterial* worldMaterial) {
    currentWorldMaterial = worldMaterial;
}

void RenderContext::registerFrame() {
    currentFrame++;
    if(currentCamera.valid()) currentCamera->update();
}

namespace Renderer {
    RenderContext renderContext;
}
