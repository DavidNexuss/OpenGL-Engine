#include "vrp.h"
#include "model.h"

using namespace glm;
VRP Util::getSceneVRP()
{
    auto models = Loader::models.getSortedView();
    vec4 min = vec4(vec3(INFINITY),0);
    vec4 max = vec4(vec3(-INFINITY),0);

    for (size_t i = 0; i <models.size(); i++)
    {
        MeshID mesh = ModelID(i)->meshID;
        auto buffer = mesh->getMeshBuffer();
        for (size_t j = 0; j < mesh->vertexCount; j++)
        {
            size_t b = buffer->getVertexStride() * j;
            vec4 v = models[i].transformMatrix * vec4(buffer->raw()[b],buffer->raw()[b + 1],buffer->raw()[b + 2],1.0);
            
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
