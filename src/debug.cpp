#include "debug.h"
#include <iostream>

int Debug::materialSwaps = 0;
int Debug::materialInstanceSwaps = 0;
int Debug::meshSwaps = 0;
int Debug::textureSwaps = 0;
int Debug::uniformsFlush = 0;
int Debug::lightFlush = 0;

int Debug::missingUniforms = 0;

float Debug::lastTime = 0.0f;
float Debug::currentTime = 0.0f;

int Debug::watchesAdded = 0;
int Debug::watchesRemoved = 0;

void Debug::glError(GLenum source,
        GLenum type,
        GLuint id,
        GLenum severity,
        GLsizei length,
        const GLchar *message,
        const void *userParam)
{
   std::cerr << "[GL " << severity << "] " << message << std::endl;
    if(severity > 35000) throw new std::runtime_error("");
}
