#pragma once
#include "material_instance.h"
#include "texture.h"
#include "world_material.h"
#include "standard.h"
#include "core.h"
#include "framebuffer.h"
#include "structures/storage.h"
#include <vector>

struct Material : public EngineComponent
{
    GLuint programID;

    std::vector<std::string> uniformNames;
    std::vector<GLuint> uniforms;
    std::vector<MaterialInstanceID> usedInstances;
    
    std::vector<GLuint> textureUniforms;
    std::vector<GLuint> screenTextureUniforms;

    bool isSkyboxMaterial = false;

    Material(const std::string& fragmentShaderPath,const std::string& vertexShaderPath,const std::vector<std::string>& uniforms);
    Material(const std::string& materialName,const std::vector<std::string>& uniforms);
    Material(const std::string& materialName);

    void loadShaderUniforms(const std::vector<std::string>& uniformsList);
    bool addTexture(Texture textureID,int textureUnit);
    
    void useInstance(MaterialInstanceID materialInstanceID);
    int useScreenAttachments(const FrameBuffer& buffer,int startingTexture = 0);

    inline void bind(const WorldMaterial& world) const {
        glUseProgram(programID);

        //Enable skybox if present and compatible with shader
        Texture skyTexture;
        if ((skyTexture = world.getSkyBoxTexture()) && isSkyBoxSensitive())
        {
            TextureLoader::useTexture(skyTexture,Standard::tSkyBox,GL_TEXTURE_CUBE_MAP);
            glUniform1i(uniforms[Standard::uSkyBox],Standard::tSkyBox);
        }
    }
    
    inline bool isSkyBoxSensitive() const {
        return uniforms[Standard::uSkyBox] != GL_INVALID_INDEX;
    }
    inline bool isLightSensitive() const {
        return uniforms[Standard::uLightPosition] != GL_INVALID_INDEX;
    }

    static Material createDefaultMaterial();

    inline void bindScreenTexture(GLuint textureID,int screenTextureID) {
        size_t textureUnit = textureUniforms.size() + screenTextureID;
        TextureLoader::useTexture(textureID,textureUnit,GL_TEXTURE_2D);
        glUniform1i(screenTextureUniforms[screenTextureID],textureUnit);
    }
};

using MaterialID = size_t;

namespace Loader {
    class MaterialLoader : public storage<Material> {
        
        MaterialID debugMaterialID;
        MaterialInstanceID debugMaterialInstanceID;

        std::vector<Material> materials;
        std::vector<size_t> usedMaterials;

        public:
        inline MaterialID add(const Material& mat) {
            usedMaterials.push_back(0);
            return storage<Material>::add(mat);
        }

        inline bool updateForFrame(size_t materialIdx,size_t frame) {
            if(usedMaterials[materialIdx] != frame){
                usedMaterials[materialIdx] = frame;
                return true;
            }
            return false;
        }
    };

    extern MaterialLoader materials;
};