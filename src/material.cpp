#include "material.h"
#include "directory.h"
#include "debug.h"
#include "load_shader.h"

using namespace std;

Material::Material(const std::string& fragmentShaderPath,const std::string& vertexShaderPath)
{
    programID = compileShader(vertexShaderPath.c_str(),fragmentShaderPath.c_str());
    
    if (programID == Standard::glInvalid)  {
		std::cerr << "Error loading shaders: " << programID << std::endl;
        throw std::runtime_error("Error compiling shader");
    }

    //Read all uniforms
    int count;
    glGetProgramiv(programID, GL_ACTIVE_UNIFORMS, &count);
    for (int i = 0; i < count; i++)
    {
        int bufSize = 256;
        char nameBuffer[bufSize];
        GLint length;
        GLenum type;
        int size;
        glGetActiveUniform(programID, (GLuint)i, bufSize, &length, &size, &type, nameBuffer);
        string uniformname(nameBuffer,length);
        uniformsMap[uniformname] = glGetUniformLocation(programID, nameBuffer);
    }
}

Material::Material(const string& materialName) : Material(
            Directory::materialPrefix + materialName + "_fragment.glsl",
            Directory::materialPrefix + materialName + "_vertex.glsl") { }

bool Material::bindUniform(const std::string& uniformName,Uniform& uniformValue,MaterialInstanceID materialInstanceID) {
    decltype(uniformsMap)::const_iterator it;
    if((it = uniformsMap.find(uniformName)) == uniformsMap.end()) return false; //TODO: generateWarning
    
    if(!materialInstanceID.valid() || (usedInstances[it->first] != materialInstanceID) || uniformValue.dirty) {
        usedInstances[it->first] = materialInstanceID;
        return uniformValue.bind(it->second);
    }
    return false;
}
void Material::useInstance(MaterialInstanceID matInstance)
{
    MaterialInstance::uniformCollection& matUniforms = matInstance->getUniformCollection();
    for(auto& uniform : matUniforms) {
        bindUniform(uniform.first, uniform.second,matInstance);
    }
}

Material Material::createDefaultMaterial()
{
    return Material(Directory::getDefaultFragemntShaderPath(),Directory::getDefaultVertexShaderPath());
}

namespace Loader
{
    MaterialLoader materials;
};
