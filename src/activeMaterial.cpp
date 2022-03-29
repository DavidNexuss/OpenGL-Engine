#include "activeMaterial.h"

void ActiveMaterial::setActiveMaterial(MaterialID material) {
    if(this->material != material) {
        this->material = material;
        glUseProgram(material->programID);
    }
}
void ActiveMaterial::bindUniform(const std::string& name,Uniform value) {
    bindUniform(name,value,-1);
}
void ActiveMaterial::bindUniform(const std::string& name,Uniform value,MaterialInstanceID instance) {
    GLuint uniformID = material->getUniform(name);
    if(uniformID != GL_INVALID_INDEX)
        value.bind(material->getUniform(name));
}
void ActiveMaterial::bindInstance(MaterialInstanceID matInstance) {
    MaterialInstance::uniformCollection& matUniforms = matInstance->getUniformCollection();
    for(auto& uniform : matUniforms) {
        bindUniform(uniform.first, uniform.second,matInstance);
    }
}