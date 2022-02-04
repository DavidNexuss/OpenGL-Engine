#include "model.h"
#include "mesh_builder.h"
#include "texture.h"
#include <list>
using namespace std;

sorted_storage<Model> Loader::models;

bool Model::lastcullFrontFace = false;