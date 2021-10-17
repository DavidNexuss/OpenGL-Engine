#pragma once
#include "model.h"
#include "material.h"
#include <glm/glm.hpp>
#include <iostream>

using ModelGroup = std::vector<Model>;
ModelGroup loadMeshFromFile(const std::string& path,MaterialID materialID,bool flipUv = false);

struct ModelGroupID
{
    size_t first,last;

    inline void apply(glm::mat4 (func)(const glm::mat4& transform))
    {
        std::cerr << first << " " << last << std::endl;
        for(size_t i = first; i <= last; i++)
        {
            ModelLoader::models[i].transformMatrix = func(ModelLoader::models[i].transformMatrix);
        }
    }
};

namespace ModelLoader
{
    static inline ModelGroupID loadModel(ModelGroup& modelGroup)
    {
        ModelGroupID id;
        for(size_t i = 0; i < modelGroup.size(); i++)
        {
            id.last = ModelLoader::loadModel(std::move(modelGroup[i]));
            if (i == 0) id.first = id.last;
        }
        return id;
    }
}
