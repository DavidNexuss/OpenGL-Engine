#include "model.h"
#include "mesh_builder.h"
#include "texture.h"
#include "renderer.h"
#include "renderContext.h"
using namespace std;
using namespace Renderer;

bool Model::lastcullFrontFace = false;

void Model::draw()
{
    if (cullFrontFace != lastcullFrontFace)
    {
        glCullFace(GL_FRONT + cullFrontFace);
        lastcullFrontFace = cullFrontFace;
    }  

    if (depthMask) glDepthMask(GL_FALSE);

    renderContext.useMaterial(materialID);
    
    if (materialInstanceID.valid()) renderContext.useMaterialInstance(materialInstanceID);
    

    renderContext.useMesh(meshID);
    
    if(UNIFORMS(Standard::uTransformMatrix) != GL_INVALID_INDEX)
    {
        glUniformMatrix4fv(UNIFORMS(Standard::uTransformMatrix),1,false,&transformMatrix[0][0]);
    }
    
    if (UNIFORMS(Standard::uNormalMatrix) != GL_INVALID_INDEX) {
        normalMatrix = glm::transpose(glm::inverse(transformMatrix));
        glUniformMatrix3fv(UNIFORMS(Standard::uNormalMatrix),1,false,&normalMatrix[0][0]);
    }
    
    Renderer::drawMesh();
    
    if(depthMask) glDepthMask(GL_TRUE);

}

namespace Loader {
    sorted_storage<Model> models;
}
