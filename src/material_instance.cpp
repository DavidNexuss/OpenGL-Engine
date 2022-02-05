#include "material_instance.h"
#include "debug.h"
#include <vector>

void MaterialInstance::useUniform(UniformID id,GLuint glUniformID)
{
    if(uniformValues.size() <= id)
    {
        std::cerr << "Warning: uniform not found" << std::endl;
        return;
    }

    uniformValues[id].dirty = false;
    const Uniform& current = uniformValues[id];

    switch(current.type)
    {
        case UniformType::VEC2:
        glUniform2fv(glUniformID,1,&current.VEC2[0]); break;
        case UniformType::VEC3:
        glUniform3fv(glUniformID,1,&current.VEC3[0]); break;
        case UniformType::VEC4:
        glUniform4fv(glUniformID,1,&current.VEC4[0]); break;
        case UniformType::MAT2:
        glUniformMatrix2fv(glUniformID,1,false,&current.MAT2[0][0]); break;
        case UniformType::MAT3:
        glUniformMatrix3fv(glUniformID,1,false,&current.MAT3[0][0]); break;
        case UniformType::MAT4:
        glUniformMatrix4fv(glUniformID,1,false,&current.MAT4[0][0]); break;
        case UniformType::FLOAT:
        glUniform1f(glUniformID,current.FLOAT); break;
        case UniformType::BOOL:
        case UniformType::INT:
        glUniform1i(glUniformID,current.INT);
        break;
    }
    REGISTER_UNIFORM_FLUSH();
}

storage<MaterialInstance> Loader::materialInstances;
