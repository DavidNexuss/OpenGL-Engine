#include "scene.h"
#include "material.h"
#include "renderer.h"

CameraID Scene::camera = 0;
float Scene::time = 0;

void Scene::update()
{
    CameraLoader::cameras[camera].update();
}

void Scene::flush()
{
    CameraLoader::cameras[camera].flush();
    glUniform1f(UNIFORMS(Standard::uTime),time);
}
