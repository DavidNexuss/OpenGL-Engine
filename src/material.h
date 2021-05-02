#pragma once
#include "material_instance.h"
#include "texture.h"
#include <vector>
#include <list>

enum UniformBasics
{
    UNIFORM_PROJECTION_MATRIX = 0,
    UNIFORM_VIEW_MATRIX,
    UNIFORM_TRANSFORM_MATRIX,
    UNIFORM_NORMAL_MATRIX,
    UNIFORM_TIME,
    UNIFORM_LIGHTCOLOR,
    UNIFORM_LIGHTPOSITION,
    UNIFORM_LIGHTCOUNT,
    UNIFORM_VIEW_POS,
    UNIFORM_SKYBOX,
    UNIFORM_COUNT
};

struct Material
{
    const static int scene_uniform_count = UNIFORM_COUNT;
    
    GLuint programID;
    std::vector<GLuint> uniforms;
    std::vector<MaterialInstanceID> usedInstances;
    
    std::string materialName;
    std::vector<GLuint> textureUniforms;

    bool isSkyboxMaterial = false;

    Material(const std::string& materialName,const std::list<std::string>& uniforms);

    void loadShaderUniforms(const std::list<std::string>& uniformsList);

    bool addTexture(Texture textureID,int textureUnit);

    void useInstance(MaterialInstanceID materialInstanceID);

    inline void bind()
    {
        glUseProgram(programID);
        if (uniforms[UNIFORM_SKYBOX] != GL_INVALID_INDEX)
        {
            glUniform1i(uniforms[UNIFORM_SKYBOX],TextureLoader::bindSkyBox());
        }
    }
    
    inline bool isLightSensitive() const
    {
        return uniforms[UNIFORM_LIGHTPOSITION] != GL_INVALID_INDEX;
    }

};

#define UNIFORM_SET MaterialLoader::get_current()->uniforms

using MaterialID = size_t;
class MaterialLoader
{
    public:
    static MaterialID debugMaterialID;
    static MaterialInstanceID debugMaterialInstanceID;

    static std::vector<Material> materials;
    static std::vector<size_t> usedMaterials;

    static MaterialID currentMaterial;
    
    inline static MaterialID loadMaterial(const Material& material)
    {
        materials.push_back(material);
        usedMaterials.push_back(0);
        MaterialID mat = materials.size() - 1;
        return mat;
    }
};

#define UNIFORMS(x) MaterialLoader::materials[MaterialLoader::currentMaterial].uniforms[x]
#define CURRENT_MATERIAL() MaterialLoader::materials[MaterialLoader::currentMaterial]
