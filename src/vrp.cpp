#include "vrp.h"
#include "model.h"

using namespace glm;
VRP Util::getSceneVRP()
{
    auto models = ModelLoader::native();
    vec4 min = vec4(vec3(INFINITY),0);
    vec4 max = vec4(vec3(-INFINITY),0);

    for (size_t i = 0; i <models.size(); i++)
    {
        Mesh& mesh = MeshLoader::meshes[ModelLoader::models[i].meshID];
        for (size_t j = 0; j < mesh.vertexCount; j++)
        {
            size_t b = mesh.vertexStride * j;
            vec4 v = models[i].transformMatrix * vec4(mesh.meshPtr()[b],mesh.meshPtr()[b + 1],mesh.meshPtr()[b + 2],1.0);
            
            min.x = glm::min(v.x,min.x);
            min.y = glm::min(v.y,min.y);
            min.z = glm::min(v.z,min.z);

            max.x = glm::max(v.x,max.x);
            max.y = glm::max(v.y,max.y);
            max.z = glm::max(v.z,max.z);
        }
    }

    return {min,max};
}
