#pragma once
#include <iostream>
#include "core.h"

class Debug
{
    public:
    static int materialSwaps;
    static int materialInstanceSwaps;
    static int meshSwaps;
    static int textureSwaps;
    static int uniformsFlush;
    static int lightFlush;

    static int missingUniforms;
    static float lastTime;
    static float currentTime;

    inline static void reset()
    {
        materialSwaps = 0;
        materialInstanceSwaps = 0;
        meshSwaps = 0;
        textureSwaps = 0;
        uniformsFlush = 0;
        lightFlush = 0;
        lastTime = currentTime;
        currentTime = glfwGetTime();
    }

    inline static void print()
    {

        std::cerr << "Frame debug information: " << std::endl;
        std::cerr << "Frame rate: " << ( 1.0f / (currentTime - lastTime) ) << std::endl;
        std::cerr << "Material swaps :" << materialSwaps << std::endl;
        std::cerr << "Matrial instance swap: " << materialInstanceSwaps << std::endl;
        std::cerr << "Uniform flushs: " << uniformsFlush << std::endl;
        std::cerr << "Mesh swaps :" << meshSwaps << std::endl;
        std::cerr << "Texture swaps :" << textureSwaps << std::endl;
        std::cerr << "Light flush :" << lightFlush << std::endl;
        std::cerr << "----" << std::endl;
        std::cerr << "Missing uniforms: " << missingUniforms << std::endl;
        std::cerr << "----" << std::endl;
    }

    static void glError(GLenum source,
            GLenum type,
            GLuint id,
            GLenum severity,
            GLsizei length,
            const GLchar *message,
            const void *userParam);
};

#define DEBUG
#ifdef DEBUG 
    #define REGISTER_MISSED_UNIFORM() Debug::missingUniforms++
    #define REGISTER_FRAME() Debug::reset()
    #define REGISTER_MATERIAL_SWAP() Debug::materialSwaps++
    #define REGISTER_MESH_SWAP() Debug::meshSwaps++
    #define REGISTER_TEXTURE_SWAP() Debug::textureSwaps++
    #define REGISTER_MATERIAL_INSTANCE_SWAP() Debug::materialInstanceSwaps++
    #define REGISTER_UNIFORM_FLUSH() Debug::uniformsFlush++
    #define REGISTER_LIGHT_FLUSH() Debug::lightFlush++
    #define LOG_FRAME() Debug::print()    
#else
    #define REGISTER_MISSED_UNIFORM()
    #define REGISTER_FRAME()
    #define REGISTER_MATERIAL_SWAP()
    #define REGISTER_MESH_SWAP()
    #define REGISTER_TEXTURE_SWAP()
    #define REGISTER_MATERIAL_INSTANCE_SWAP()
    #define REGISTER_UNIFORM_FLUSH()
    #define REGISTER_LIGHT_FLUSH()
    #define LOG_FRAME()
#endif
