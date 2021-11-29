#include "ivec.h"
#include <mesh/primitiveMesh.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <unordered_set>
#include "tensor.h"
/**
 * This module provides with basic functionality for creating voxeled objects in an adaptive size world.
 * Each voxel has a voxelMaterialID that identifies its material. This module generates a VBO for evert continous
 * group of voxels with same material and returns a mesh.
 * 
 * This is not aimed to be a performant VoxelEngine but to be a simple way of creating minimal scale voxelized geometry
 * to place around the scene
 */


/*
* Simple non indexed voxel mesh generator for creating voxel VBOs.
*/
struct VoxelMeshGenerator : public PrimitiveMesh
{
    enum VoxelDirection
    {
        UP = 0,
        RIGHT,
        DOWN,
        LEFT,
        FRONT,
        BACK
    };

    void addFace(int x,int y,int z,VoxelDirection direction);
    void create() final;
};
/*
* This class holds the mesh information of a group of compatible voxels
*/
struct VoxelMesh
{
    int voxelMaterialID;
    Mesh mesh;
};

struct VoxelMeshWorld
{
    tensor<std::array<int,1>,int,1> val = {1};
    std::unordered_map<Ivec,int,IvecHash> world;
    void createPart(Ivec origin,int material,std::unordered_set<Ivec,IvecHash>& marched,VoxelMeshGenerator& generator);

    public:
    inline int& at(int x,int y,int z) { 
        return world[{x,y,z}]; 
    }

    VoxelMeshWorld();
    std::vector<VoxelMesh> rasterize();
};
