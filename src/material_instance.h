#pragma once
#include "glfw.h"
#include "core.h"
#include "uniform.h"
#include "texture.h"
#include <vector>

class MaterialInstance
{
    public:
    std::vector<Uniform> uniformValues;
    std::vector<Texture> assignedTextureUnits;

    inline MaterialInstance(size_t size = 0) : uniformValues(size), 
    assignedTextureUnits(TextureLoader::maxTextureUnits,-1) { }

    inline MaterialInstance(const std::vector<Uniform>& _uniformValues) : uniformValues(_uniformValues), 
    assignedTextureUnits(TextureLoader::maxTextureUnits,-1)
    {}

    void useUniform(UniformID id,GLuint glUniformID);

    #define UNIFORMS_FUNC_DECLARATION(v,T) \
    inline void set(UniformID id,const T& a) { uniformValues[id].v = a; uniformValues[id].forward = true; uniformValues[id].type = UniformType::v; }

    UNIFORMS_LIST(UNIFORMS_FUNC_DECLARATION)
    #undef UNIFORMS_FUNC_DECLARATION

    inline void setTexture(Texture textureID,int unitID) {
        assignedTextureUnits[unitID] = textureID;
    }

};

using MaterialInstanceID = size_t;

namespace MaterialInstanceLoader
{
    extern std::vector<MaterialInstance> materialInstances;

    inline static MaterialInstanceID loadMaterialInstance(const MaterialInstance& materialInstance)
    {
        materialInstances.push_back(materialInstance);
        return materialInstances.size() - 1;
    }
    inline static MaterialInstance& get(size_t index)
    {
        return materialInstances[index];
    }
};
