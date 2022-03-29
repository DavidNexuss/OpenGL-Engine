#pragma once
#include "material_instance.h"
#include "texture.h"
#include "standard.h"
#include "core.h"
#include "framebuffer.h"
#include "structures/storage.h"
#include <vector>
#include <unordered_map>
#include <string>

struct Material : public EngineComponent
{
    GLuint programID;

    std::unordered_map<std::string,MaterialInstanceID> usedInstances;
    std::unordered_map<std::string,GLuint> uniformsMap;

    bool isSkyboxMaterial = false;

    Material(const std::string& fragmentShaderPath,const std::string& vertexShaderPath);
    Material(const std::string& materialName);
    GLuint getUniform(const std::string& name);

    static Material createDefaultMaterial();
};

namespace Loader {
    class MaterialLoader : public storage<Material> {
        
        size_t debugMaterialID;
        MaterialInstanceID debugMaterialInstanceID;

        std::vector<Material> materials;
        std::vector<size_t> usedMaterials;

        public:
        inline size_t add(const Material& mat) {
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


using MaterialID = storage_pointer<Loader::MaterialLoader,Loader::materials>;

