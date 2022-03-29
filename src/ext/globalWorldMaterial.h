#include "world_material.h"
#include "activeMaterial.h"
class GlobalWorldMaterial : public WorldMaterial {
    std::unordered_map<std::string,Uniform> uniforms;
    
    public:
    virtual void bind(ActiveMaterial currentMaterial) {
        for(const auto& it : uniforms) {
            currentMaterial.bindUniform(it.first,it.second);
        }
    }

    inline void setUniform(const std::string& name,Uniform value) {
        uniforms[name] = value;
    }
};
