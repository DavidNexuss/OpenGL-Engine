#include "scene.h"
#include "material.h"
#include "renderer.h"

CameraID Scene::camera = 0;
float Scene::time = 0;

void Scene::update()
{
    Loader::cameras[camera].update();
}

void Scene::flush()
{
    Loader::cameras[camera].flush();
    glUniform1f(UNIFORMS(Standard::uTime),time);
}
