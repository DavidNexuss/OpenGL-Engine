#include "primitiveMesh.h"
#include <glm/glm.hpp>
#include <unordered_map>
#include <unordered_set>

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
    virtual void create() override;
};

/*
*  
*/
struct VoxelMesh : public Mesh
{
    int voxelMaterialID;
};

struct Ivec {
    int x,y,z;

    Ivec(int _x,int _y,int _z) : x(_x), y(_y), z(_z) { }

    bool operator==(const Ivec& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    Ivec operator+(const Ivec& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }
};

struct IvecHash
{
    size_t operator()(const Ivec& ivec) const {
        return ivec.x + ivec.y + ivec.z;
    }
};

struct VoxelMeshWorld
{
    std::unordered_map<Ivec,int,IvecHash> world;

    void createPart(Ivec origin,int material,std::unordered_set<Ivec,IvecHash>& marched,VoxelMeshGenerator& generator);

    public:
    inline int& at(int x,int y,int z) { 
        return world[{x,y,z}]; 
    }

    VoxelMeshWorld();
    std::vector<VoxelMesh> rasterize();
};
