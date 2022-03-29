#include "material.h"
#include "directory.h"
#include "debug.h"
#include "load_shader.h"

using namespace std;

std::string fixUniformName(const std::string& uniformName) {
    std::string newName = "";
    for (int i = 0; i < uniformName.length(); ++i) {
        if(uniformName[i] == '[') break;
        newName += uniformName[i];
    }
    return newName;
}
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
        string uniformname = fixUniformName(string(nameBuffer,length));
        std::cout << uniformname << std::endl;
        GLuint id = glGetUniformLocation(programID, uniformname.c_str());
        uniformsMap[uniformname] = id;
    }
}

Material::Material(const string& materialName) : Material(
            Directory::materialPrefix + materialName + "_fragment.glsl",
            Directory::materialPrefix + materialName + "_vertex.glsl") { }

GLuint Material::getUniform(const std::string& uniformName) {
    decltype(uniformsMap)::const_iterator it;
    if((it = uniformsMap.find(uniformName)) == uniformsMap.end()) {
        //std::cerr << "[MATERIAL " << programID << "] uniform not found " << uniformName << std::endl;
        return GL_INVALID_INDEX;
    }
    return it->second;
}

Material Material::createDefaultMaterial()
{
    return Material(Directory::getDefaultFragemntShaderPath(),Directory::getDefaultVertexShaderPath());
}

namespace Loader
{
    MaterialLoader materials;
};
