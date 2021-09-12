#include "model.h"
#include "mesh_builder.h"
#include "texture.h"
#include <list>
using namespace std;

sorted_storage<Model> ModelLoader::models;

bool Model::lastcullFrontFace = false;