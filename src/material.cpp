#include "material.h"
#include "directory.h"
#include "debug.h"
#include "load_shader.h"
#include "id.h"

using namespace std;

Material::Material(const std::string& fragmentShaderPath,const std::string& vertexShaderPath,const list<string>& uniforms)
{
    programID = compileShader(vertexShaderPath.c_str(),fragmentShaderPath.c_str());
    
    if (programID < 0)
    {
        cerr << "Error loading shaders!!!" << endl;
        throw std::runtime_error("Error compiling shader");
    }

    loadShaderUniforms(uniforms);

}

Material::Material(const string& materialName,const list<string>& uniforms) : Material(
            Directory::materialPrefix + materialName + "_fragment.glsl",
            Directory::materialPrefix + materialName + "_vertex.glsl",uniforms) { }

Material::Material(const string& materialName) : Material(materialName,{}) { }

void Material::loadShaderUniforms(const list<string>& uniformsList)
{ 
    for(int i = 0; i < scene_uniform_count; i++)
        uniforms.push_back(-1);

    //Predefined uniforms lookup
    uniforms[UNIFORM_PROJECTION_MATRIX] = glGetUniformLocation(programID,"projectionMatrix");
    uniforms[UNIFORM_VIEW_MATRIX] = glGetUniformLocation(programID,"viewMatrix");
    uniforms[UNIFORM_TRANSFORM_MATRIX] = glGetUniformLocation(programID,"transformMatrix");
    uniforms[UNIFORM_NORMAL_MATRIX] = glGetUniformLocation(programID,"normalMatrix");
    uniforms[UNIFORM_TIME] = glGetUniformLocation(programID,"time");
    uniforms[UNIFORM_LIGHTCOLOR] = glGetUniformLocation(programID,"lightColor");
    uniforms[UNIFORM_LIGHTPOSITION] = glGetUniformLocation(programID,"lightPosition");
    uniforms[UNIFORM_LIGHTCOUNT] = glGetUniformLocation(programID,"lightCount");
    uniforms[UNIFORM_VIEW_POS] = glGetUniformLocation(programID,"viewPos");
    uniforms[UNIFORM_SKYBOX] = glGetUniformLocation(programID,"skybox");
    
    //User uniform lookup
    for(const auto& uniform : uniformsList)
    {
        GLuint uniform_location = glGetUniformLocation(programID,uniform.c_str());
        if(uniform_location == ID::invalid_id)
        {
            throw std::runtime_error("Asked uniform not found");
        }
        uniforms.emplace_back(uniform_location);
        usedInstances.emplace_back();
    }
    
    //TextureLoader uniform lookup
    GLuint location = 0;
    for (size_t i = 0; i < TextureLoader::maxTextureUnits and location != ID::invalid_id; i++)
    {
        string uniformName = "texture" + to_string(i);
        location = glGetUniformLocation(programID,uniformName.c_str());
        if(location != ID::invalid_id)
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
        if (usedInstances[i] != materialInstanceID || instance.uniformValues[i].forward)
        {
            usedInstances[i] = materialInstanceID;
            MaterialInstanceLoader::materialInstances[materialInstanceID].useUniform(i,uniforms[i + scene_uniform_count]);
            swap = true;
        }
    }

    for (size_t i = 0; i < textureUniforms.size(); i++)
    {
        if (instance.assignedTextureUnits[i] != ID::invalid_id)
        {
            TextureLoader::useTexture(instance.assignedTextureUnits[i],i,isSkyboxMaterial ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D);
            glUniform1i(textureUniforms[i],i);
        }
    }
    
    if (swap) REGISTER_MATERIAL_INSTANCE_SWAP();
}

string Material::getDefaultVertexShaderPath() 
{
    return string(Directory::materialPrefix) + "default_vertex.glsl";
}

string Material::getDefaultFragemntShaderPath()
{
    return string(Directory::materialPrefix) + "default_fragemnt.glsl";
}

Material Material::createDefaultMaterial()
{
    return Material(getDefaultFragemntShaderPath(),getDefaultVertexShaderPath(),{});
}

MaterialID MaterialLoader::debugMaterialID = -1;
MaterialInstanceID MaterialLoader::debugMaterialInstanceID = -1;
vector<Material> MaterialLoader::materials;
vector<size_t> MaterialLoader::usedMaterials;
MaterialID MaterialLoader::currentMaterial = -1;
