#pragma once
#include <cstddef>
#include <string>
#include "glfw.h"

/**
 * This namespace defines constants and conventions used by the engine that the user
 * must follow for the right use of the engine
 */
namespace Standard
{
    //Vertex Attribute standard pointers
    //Attribute values are explicitly defined for avoiding inconsistencies with shaders
    enum VertexAttributes
    {
        aPosition =     0,
        aColor =        1,
        aUV =           2,
        aNormal =       3,
        aTangent =      4,
        aBiTangent =    5
    };

    //Engine world aspect materials
    enum WorldMaterialAspect {
        wGlobal = 0,
        wCamera,
        wSky,
        wCount
    };

    //Engine uniforms
    #define UNIFORM_LIST(f)\
        f(uProjectionMatrix)              /* mat4       Camera projection matrix */ \
        f(uViewMatrix)                    /* mat4       Camera view matrix */ \
        f(uTransformMatrix)               /* mat4       Camera transformation matrix */ \
        f(uNormalMatrix)                  /* mat3       Camera normal matrix */ \
        f(uTime)                          /* float      Global world time in seconds */ \
        f(uLightColor)                    /* vec3       Light color */ \
        f(uLightPosition)                 /* vec3       World Light position */ \
        f(uLightCount)                    /* int        Light Count */ \
        f(uViewPos)                       /* vec3       World camera position */ \
        f(uSkyBox)                        /* sampler2D  SkyBox cubemap */ \
        f(uShadowMap)                     /* sampler2D  ShadowMap depth information */ \
        f(uLightSpaceMatrix)              /* mat4 Light space matrix */ \
        
    #define CHARACTER_LIST(O) const static std::string O = #O;
    
    UNIFORM_LIST(CHARACTER_LIST)
    
    #undef UNIFORM_LIST
    #undef CHARACTER_LIST

    //Engine reserved textureUnits
    enum TextureUnits
    {
        tShadowMap = 13,
        tCreation = 14,
        tSkyBox = 15
    };

    const static size_t  maxTextureUnits = 16;
    const static size_t  maxUserTextureUnits = tSkyBox;

    //Engine Shader specific constants
    const static size_t maxLights = 6;

    //Default window hints
    const static int defaultWindowWidth = 1280;
    const static int defaultlWindowHeight = 720;
    const static int defaultOpenglMajorVersion = 4;
    const static int defaultOpenglMinorVersion = 5;

    const static int defaultMssaLevel = 4;

    //Mesh index size
    //Normally a 16bit unsigned int limit for indexes count should be more than enough for any mesh
    using meshIndex = unsigned short;
    
    const static uint32_t meshIndexGL = GL_UNSIGNED_SHORT;  //For glDrawElements call
    const static bool interleaveVBOData = true;
    
    const static unsigned int glInvalid = -1;

    inline bool is_invalid(unsigned int glVal) {return glVal == glInvalid; }

    struct GLIdentifier {
        GLuint identifier = GL_INVALID_INDEX;
        inline void operator=(GLuint identifier) {this->identifier = identifier;}
        inline operator GLuint() const {return identifier; }
    };
}
