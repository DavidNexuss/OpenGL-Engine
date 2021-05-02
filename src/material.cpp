#include "material.h"
#include "directory.h"
#include "debug.h"
#include "load_shader.h"

using namespace std;

Material::Material(const string& materialName,const list<string>& uniforms)
{
    this->materialName = materialName;
    string fragmentPath = Directory::materialPrefix + materialName + "_fragment.glsl";
    string vertexPath = Directory::materialPrefix + materialName + "_vertex.glsl";

    programID = compileShader(vertexPath.c_str(),fragmentPath.c_str());
    
    if (programID < 0)
    {
        cerr << "Error loading shaders!!!" << endl;
        throw std::runtime_error("Error compiling shader");
    }

    loadShaderUniforms(uniforms);
}

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
    size_t i = 0;
    for(const auto& uniform : uniformsList)
    {
        GLuint uniform_location = glGetUniformLocation(programID,uniform.c_str());
        if(uniform_location == -1)
        {
            throw std::runtime_error("Asked uniform not found");
        }
        uniforms.emplace_back(uniform_location);
        usedInstances.emplace_back();
    }
    
    //TextureLoader uniform lookup
    GLuint location = 0;
    for (size_t i = 0; i < TextureLoader::maxTextureUnits and location != -1; i++)
    {
        string uniformName = "texture" + to_string(i);
        location = glGetUniformLocation(programID,uniformName.c_str());
        if(location != -1)
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
        if (instance.assignedTextureUnits[i] != -1)
        {
            TextureLoader::useTexture(instance.assignedTextureUnits[i],i,isSkyboxMaterial ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D);
            glUniform1i(textureUniforms[i],i);
        }
    }
    
    if (swap) REGISTER_MATERIAL_INSTANCE_SWAP();
}

MaterialID MaterialLoader::debugMaterialID = -1;
MaterialInstanceID MaterialLoader::debugMaterialInstanceID = -1;
vector<Material> MaterialLoader::materials;
vector<size_t> MaterialLoader::usedMaterials;
MaterialID MaterialLoader::currentMaterial = -1;
