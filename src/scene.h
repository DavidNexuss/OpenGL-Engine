#pragma once
#include <glm/glm.hpp>
#include "camera.h"

namespace Scene
{
    extern CameraID camera;
    extern float time;

    void update();
    void flush();
}
