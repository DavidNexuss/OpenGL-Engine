#pragma once
#include "mesh.h"

namespace MeshBuilder
{
    enum PrimitiveMeshType
    {
        Triangle,Plain,Cube,SkyBox
    };

    Mesh createPrimitiveMesh(PrimitiveMeshType type,bool uv);
};
