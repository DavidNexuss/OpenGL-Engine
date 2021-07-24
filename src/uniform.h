#pragma once
#include <glm/glm.hpp>


#define UNIFORMS_LIST(o) \
    o(VEC2,glm::vec2) o(VEC3,glm::vec3) o(VEC4,glm::vec4) \
    o(MAT2,glm::mat2) o(MAT3,glm::mat3) o(MAT4,glm::mat4) \
    o(FLOAT,float) o(BOOL,bool) o(INT,int)

enum UniformType
{
    #define UNIFORMS_ENUMS_DECLARATION(v,T) v ,
    UNIFORMS_LIST(UNIFORMS_ENUMS_DECLARATION)
    #undef UNIFORMS_ENUMS_DECLARATION
};


class Uniform
{
    public:
    union
    {
        #define UNIFORMS_UNION_DECLARATION(v,T) T v;
        UNIFORMS_LIST(UNIFORMS_UNION_DECLARATION)
        #undef UNIFORMS_UNION_DECLARATION
    };

    UniformType type;
    bool forward;

    Uniform() { }
    #define UNIFORMS_CONSTRUCTOR(v,T) constexpr Uniform(const T& _##v) : v(_##v),type(UniformType::v),forward(true) { }
    UNIFORMS_LIST(UNIFORMS_CONSTRUCTOR)
    #undef UNIFORMS_CONSTRUCTOR

    inline void update() { forward = true; }
    
};

using UniformID = size_t;
