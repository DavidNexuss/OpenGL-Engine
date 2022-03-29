#include "uniform.h"
#include "core.h"
#include "debug.h"
#include "texture.h"
#include <iostream>
bool Uniform::bind(GLuint glUniformID) {

    switch(type)
    {
        case UniformType::VEC2:
            glUniform2fv(glUniformID,count,&VEC2[0]); break;
        case UniformType::VEC3:
            glUniform3fv(glUniformID,count,&VEC3[0]); break;
        case UniformType::VEC3PTR:
            glUniform3fv(glUniformID,count,&(*VEC3PTR)[0]); break;
        case UniformType::VEC4:
            glUniform4fv(glUniformID,count,&VEC4[0]); break;
        case UniformType::MAT2:
            glUniformMatrix2fv(glUniformID,count,false,&MAT2[0][0]); break;
        case UniformType::MAT3:
            glUniformMatrix3fv(glUniformID,count,false,&MAT3[0][0]); break;
        case UniformType::MAT4:
            glUniformMatrix4fv(glUniformID,count,false,&MAT4[0][0]); break;
        case UniformType::FLOAT:
            glUniform1f(glUniformID,FLOAT); break;
        case UniformType::BOOL:
        case UniformType::INT:
            glUniform1i(glUniformID,INT); break;
        case UniformType::SAMPLER2D:
            TextureLoader::useTexture(SAMPLER2D.texID,SAMPLER2D.unit,SAMPLER2D.mode);
            glUniform1i(glUniformID,SAMPLER2D.unit); break;
    }
    if(type != UniformType::SAMPLER2D) dirty = false;
    REGISTER_UNIFORM_FLUSH();
    return true;
}