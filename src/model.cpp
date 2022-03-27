#include "model.h"
#include "mesh_builder.h"
#include "texture.h"
#include "renderer.h"

using namespace std;

bool Model::lastcullFrontFace = false;

void Model::draw()
{
    if (cullFrontFace != lastcullFrontFace) {
        glCullFace(GL_FRONT + cullFrontFace);
        lastcullFrontFace = cullFrontFace;
    }  

    if (depthMask) glDepthMask(GL_FALSE);

    Renderer::useMaterial(materialID);
    
    if (materialInstanceID.valid()) 
        Renderer::useMaterialInstance(materialInstanceID);
    


    materialID->bindUniform(Standard::uTransformMatrix,Uniform(transformMatrix));
    materialID->bindUniform(Standard::uNormalMatrix,Uniform(normalMatrix));

    Renderer::useMesh(meshID);
    Renderer::drawMesh();
    
    if(depthMask) glDepthMask(GL_TRUE);

}

namespace Loader {
    sorted_storage<Model> models;
}
