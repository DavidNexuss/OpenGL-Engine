#include "model.h"
#include "mesh_builder.h"
#include "texture.h"
#include "renderer.h"
using namespace std;

sorted_storage<Model> Loader::models;

bool Model::lastcullFrontFace = false;

void Model::draw()
{
    if (cullFrontFace != lastcullFrontFace)
    {
        glCullFace(GL_FRONT + cullFrontFace);
        lastcullFrontFace = cullFrontFace;
    }  

    if (depthMask) glDepthMask(GL_FALSE);

    Renderer::useMaterial(materialID);
    
    if (!Standard::is_invalid(materialInstanceID)) Renderer::useMaterialInstance(materialInstanceID);
    
    Renderer::useMesh(meshID);
    
    if(UNIFORMS(Standard::uTransformMatrix) != GL_INVALID_INDEX)
    {
        glUniformMatrix4fv(UNIFORMS(Standard::uTransformMatrix),1,false,&transformMatrix[0][0]);
    }
    
    if (UNIFORMS(Standard::uNormalMatrix) != GL_INVALID_INDEX)
    {
        normalMatrix = glm::transpose(glm::inverse(transformMatrix));
        glUniformMatrix3fv(UNIFORMS(Standard::uNormalMatrix),1,false,&normalMatrix[0][0]);
    }
    
    Renderer::drawMesh(Loader::meshes[Renderer::currentMesh].indexed);
    
    if(depthMask) glDepthMask(GL_TRUE);

}
