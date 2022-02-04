#pragma once
#include <cstddef>
#include <array>
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
    
    //Engine uniforms
    enum Uniforms
    {
        uProjectionMatrix =      0,      /* mat4       Camera projection matrix */
        uViewMatrix =            1,      /* mat4       Camera view matrix */
        uTransformMatrix =       2,      /* mat4       Camera transformation matrix */
        uNormalMatrix =          3,      /* mat3       Camera normal matrix */
        uTime =                  4,      /* float      Global world time in seconds */
        uLightColor =            5,      /* vec3       Light color */
        uLightPosition =         6,      /* vec3       World Light position */
        uLightCount =            7,      /* int        Light Count */
        uViewPos =               8,      /* vec3       World camera position */
        uSkyBox =                9,      /* sampler2D  SkyBox cubemap */
        uniformCount                           // Used internally to get the count of engine uniforms
    };

    const static std::array<const char*,uniformCount> UniformsNames =  {
        "uProjectionMatrix", 
        "uViewMatrix", 
        "uTransformMatrix",
        "uNormalMatrix",
        "uTime",
        "uLightColor",
        "uLightPosition",
        "uLightCount",
        "uViewPos",
        "uSkybox"
    };

    //Engine reserved textureUnits
    enum TextureUnits
    {
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
    
    const static size_t engineInvalid = -1;
    const static unsigned int glInvalid = -1;

    template <typename T>
    bool is_invalid(const T& val) {
        return val == T(-1);
    }
}
