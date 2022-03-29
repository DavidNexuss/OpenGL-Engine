#pragma once
#include "core.h"
#include "material.h"

class ActiveMaterial {
    MaterialID material;
    public:
    ActiveMaterial() { }
    void setActiveMaterial(MaterialID material);
    void bindUniform(const std::string& name,Uniform value);
    void bindUniform(const std::string& name,Uniform value,MaterialInstanceID instance);
    void bindInstance(MaterialInstanceID instance);

    inline MaterialID operator->() const { return material; }
    inline bool valid() const { return material.valid(); }
};