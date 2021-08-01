#pragma once
#include "glfw.h"
#include "core.h"
#include "uniform.h"
#include "texture.h"
#include "engine_component.h"
#include <vector>

/**
 * @class MaterialInstance
 * @brief Holds Material Instance specific values
 *        - Uniform values
 *        - Assigned textures 
 * 
 *  Uniform values keep a dirty value to keep track of its modifications and flush their value to the current GLSL shader 
 *  when used only if diry bit is on. Assigned textures don't have dirty bits as OpenGL texture slots (16) are shared amongst all
 *  shaders, so no diry bit management can be done. 
 */

class MaterialInstance : public EngineComponent
{
    public:
    std::vector<Uniform> uniformValues;
    std::vector<Texture> assignedTextureUnits;

    inline MaterialInstance(size_t size = 0) : uniformValues(size), 
    assignedTextureUnits(TextureLoader::maxTextureUnits,-1) { }

    inline MaterialInstance(const std::vector<Uniform>& _uniformValues) : uniformValues(_uniformValues), 
    assignedTextureUnits(TextureLoader::maxTextureUnits,-1)
    {}
    
    /**
     * @brief flush uniform value of slot id to shader if necessary
     */
    void useUniform(UniformID id,GLuint glUniformID);
    
    /** @brief sets textureID to assignedTextureUnits
     */
    inline void setTexture(Texture textureID,int unitID) {
        assignedTextureUnits[unitID] = textureID;
    }

    /**
     * @brief Returns uniform value given a Material Value index, assumes uniform value will be modified
     * and marks uniform as dirty.
     * @returns Uniform value
     */
    inline Uniform& getUniform(int index) {
        uniformValues[index].setDirty();
        return uniformValues[index];
    }
    /**
     * @brief Returns uniform value given a Material Value index.
     * @returns Uniform value
     */
    inline const Uniform& getUniform(int index) const {
        return uniformValues[index];
    }

    #define UNIFORMS_FUNC_DECLARATION(v,T) \
    inline void set(UniformID id,const T& a) { uniformValues[id].v = a; uniformValues[id].dirty = true; uniformValues[id].type = UniformType::v; }

    UNIFORMS_LIST(UNIFORMS_FUNC_DECLARATION)
    #undef UNIFORMS_FUNC_DECLARATION
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
