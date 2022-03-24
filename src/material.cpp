#include "material.h"
#include "directory.h"
#include "debug.h"
#include "load_shader.h"
#include <iostream>

using namespace std;

Material::Material(const std::string& fragmentShaderPath,const std::string& vertexShaderPath,const vector<string>& uniforms)
{
    programID = compileShader(vertexShaderPath.c_str(),fragmentShaderPath.c_str());
    
    if (programID == Standard::glInvalid)  {
		std::cerr << "Error loading shaders: " << programID << std::endl;
        throw std::runtime_error("Error compiling shader");
    }

    loadShaderUniforms(uniforms);

}

Material::Material(const string& materialName,const vector<string>& uniforms) : Material(
            Directory::materialPrefix + materialName + "_fragment.glsl",
            Directory::materialPrefix + materialName + "_vertex.glsl",uniforms) { }

Material::Material(const string& materialName) : Material(materialName,{}) { }

void Material::loadShaderUniforms(const vector<string>& uniformsList)
{ 
    this->uniformNames = uniformsList;
    uniforms.resize(Standard::uniformCount);
    
    for (size_t i = 0; i < Standard::uniformCount; i++) uniforms[i] = glGetUniformLocation(programID,Standard::UniformsNames[i]);
    
    //User uniform lookup
    for(const auto& uniform : uniformsList)
    {
        GLuint uniform_location = glGetUniformLocation(programID,uniform.c_str());
        if(Standard::is_invalid(uniform_location)) {
            throw std::runtime_error("Asked uniform " + uniform + " not found");
        }
        uniforms.emplace_back(uniform_location);
    }
    
    const auto uniformListLookup = [](GLuint programID,vector<GLuint>& uniforms,const string& pattern){
        GLuint location = 0;
        for (size_t i = 0; i < TextureLoader::maxTextureUnits and !Standard::is_invalid(location); i++)
        {
            string uniformName = pattern + to_string(i);
            location = glGetUniformLocation(programID,uniformName.c_str());
            if(!Standard::is_invalid(location)) {
                uniforms.push_back(location);
            }
        }
    };

    uniformListLookup(programID,textureUniforms,"texture");
    uniformListLookup(programID,screenTextureUniforms,"screenTexture");

    // Read all uniforms
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
        
        uniformsMap[string(nameBuffer,length)] = glGetUniformLocation(programID, nameBuffer);
    }
    
}

bool Material::bindUniform(const std::string& uniformName,Uniform& uniformValue,MaterialInstanceID materialInstanceID) {
    decltype(uniformsMap)::const_iterator it;
    if((it = uniformsMap.find(uniformName)) == uniformsMap.end()) return false; //TODO: generateWarning
    
    if(!materialInstanceID.valid() || (usedInstances[it->first] != materialInstanceID) || uniformValue.dirty) {
        usedInstances[it->first] = materialInstanceID;
        return uniformValue.bind(it->second);
    }
    return false;
}
int Material::useScreenAttachments(const FrameBuffer& buffer,int startingIndex)
{

}
void Material::useInstance(MaterialInstanceID matInstance)
{
    MaterialInstance::uniformCollection& matUniforms = matInstance->getUniformCollection();
    for(auto& uniform : matUniforms) {
        bindUniform(uniform.first, uniform.second);
    }
}

Material Material::createDefaultMaterial()
{
    return Material(Directory::getDefaultFragemntShaderPath(),Directory::getDefaultVertexShaderPath(),{});
}

namespace Loader
{
    MaterialLoader materials;
};
