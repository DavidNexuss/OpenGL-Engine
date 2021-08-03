#include "material.h"
#include "directory.h"
#include "debug.h"
#include "load_shader.h"

using namespace std;

Material::Material(const std::string& fragmentShaderPath,const std::string& vertexShaderPath,const vector<string>& uniforms)
{
    programID = compileShader(vertexShaderPath.c_str(),fragmentShaderPath.c_str());
    
    if (programID < 0)
    {
        cerr << "Error loading shaders!!!" << endl;
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
        if(uniform_location == Standard::invalidId)
        {
            throw std::runtime_error("Asked uniform not found");
        }
        uniforms.emplace_back(uniform_location);
        usedInstances.emplace_back();
    }
    
    //TextureLoader uniform lookup
    GLuint location = 0;
    for (size_t i = 0; i < TextureLoader::maxTextureUnits and location != Standard::invalidId; i++)
    {
        string uniformName = "texture" + to_string(i);
        location = glGetUniformLocation(programID,uniformName.c_str());
        if(location != Standard::invalidId)
        {
            textureUniforms.push_back(location);
        }
    }

}
void Material::useInstance(MaterialInstanceID materialInstanceID)
{
    bool swap = false;
    const MaterialInstance& instance = MaterialInstanceLoader::get(materialInstanceID);

    // Set all uniforms
    for (size_t i = 0; i < usedInstances.size(); i++)
    {
        bool different;
        if ((different = usedInstances[i] != materialInstanceID) || instance.uniformValues[i].dirty)
        {
            usedInstances[i] = materialInstanceID;
            MaterialInstanceLoader::materialInstances[materialInstanceID].useUniform(i,uniforms[i + Standard::uniformCount]);
            swap |= different;
        }
    }

    for (size_t i = 0; i < textureUniforms.size(); i++)
    {
        if (instance.assignedTextureUnits[i] != Standard::invalidId)
        {
            TextureLoader::useTexture(instance.assignedTextureUnits[i],i,isSkyboxMaterial ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D);
            glUniform1i(textureUniforms[i],i);
        }
    }
    
    if (swap) REGISTER_MATERIAL_INSTANCE_SWAP();
}

Material Material::createDefaultMaterial()
{
    return Material(Directory::getDefaultFragemntShaderPath(),Directory::getDefaultVertexShaderPath(),{});
}

MaterialID MaterialLoader::debugMaterialID = Standard::invalidId;
MaterialID MaterialLoader::currentMaterial = Standard::invalidId;
MaterialInstanceID MaterialLoader::debugMaterialInstanceID = Standard::invalidId;
vector<Material> MaterialLoader::materials;
vector<size_t> MaterialLoader::usedMaterials;
