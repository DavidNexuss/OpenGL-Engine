#include "directory.h"

using namespace std;
/** 
 * @brief Engine default paths
 */
namespace Directory
{
    const char* texturePrefix = "res/textures/";
    const char* materialPrefix = "res/materials/";
    const char* objectPrefix = "res/objects/";
    const char* fontPaths = "res/fonts/";

    string getDefaultVertexShaderPath() 
    {
        return string(Directory::materialPrefix) + "default_vertex.glsl";
    }

    string getDefaultFragemntShaderPath()
    {
        return string(Directory::materialPrefix) + "default_fragemnt.glsl";
    }
}
