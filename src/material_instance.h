#pragma once
#include "glfw.h"
#include "core.h"
#include "uniform.h"
#include "texture.h"
#include "engine_component.h"
#include "structures/storage.h"
#include <unordered_map>

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
    using uniformCollection = std::unordered_map<std::string, Uniform>;
    uniformCollection uniforms;

    inline MaterialInstance(const uniformCollection& _uniformValues) : uniforms(_uniformValues) { }

    #define UNIFORMS_FUNC_DECLARATION(v,T) \
    inline void set(const std::string& name,const T& a) { \
        Uniform val; \
        val.v = a; \
        val.dirty = true; \
        val.type = UniformType::v;\
        uniforms[name] = val; \
    }
        
    UNIFORMS_LIST(UNIFORMS_FUNC_DECLARATION)
    #undef UNIFORMS_FUNC_DECLARATION

    inline uniformCollection& getUniformCollection() { return uniforms; }
    Uniform& getUniform(const std::string& name) { return uniforms[name]; }
};

namespace Loader {
    extern storage<MaterialInstance> materialInstances;
}

using MaterialInstanceID = STORAGE_POINTER_TYPE(Loader::materialInstances);
