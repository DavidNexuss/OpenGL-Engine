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
        usedInstances.emplace_back();
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
}

int Material::useScreenAttachments(const FrameBuffer& buffer,int startingIndex)
{
	//TODO: Fix
    /*
    size_t i = 0;
    for (i = 0; i < buffer.textureAttachments.size() && (i + startingIndex) < screenTextureUniforms.size(); ++i) {
        size_t texId = textureUniforms.size() + i + startingIndex;
        size_t uniformId = i + startingIndex;

        if(texId >= Standard::maxUserTextureUnits) return i + startingIndex;

        TextureLoader::useTexture(buffer.textureAttachments[i],texId,GL_TEXTURE_2D);
        glUniform1i(screenTextureUniforms[uniformId],texId);
    }   
    return i + startingIndex;*/
}
void Material::useInstance(MaterialInstanceID matInstance)
{
    bool swap = false;

    // Set all uniforms
    for (size_t i = 0; i < usedInstances.size(); i++)
    {
        bool different;
        if ((different = usedInstances[i] != matInstance) || matInstance->uniformValues[i].dirty)
        {
            usedInstances[i] = matInstance;
            matInstance->useUniform(i,uniforms[i + Standard::uniformCount]);
            swap |= different;
        }
    }

    for (size_t i = 0; i < textureUniforms.size(); i++)
    {
        if (!Standard::is_invalid(matInstance->assignedTextureUnits[i]))
        {
            TextureLoader::useTexture(matInstance->assignedTextureUnits[i],i,isSkyboxMaterial ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D);
            glUniform1i(textureUniforms[i],i);
        }
    }
    
    if (swap) REGISTER_MATERIAL_INSTANCE_SWAP();
}

Material Material::createDefaultMaterial()
{
    return Material(Directory::getDefaultFragemntShaderPath(),Directory::getDefaultVertexShaderPath(),{});
}

namespace Loader
{
    MaterialLoader materials;
};
