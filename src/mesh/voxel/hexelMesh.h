#include <mesh/primitiveMesh.h>

/**
 * Simple non indexed hexagonal voxels generators for creating VBOs
 */
struct HexelMeshGenerator : public PrimitiveMesh
{
    enum HexelDirection
    {
        UP = 0,
        DOWN,
        SIDE1,
        SIDE2,
        SIDE3,
        SIDE4,
        SIDE5,
        SIDE6
    };

    virtual void create() final;

    void addHex(int x,int y,int z);
    void addFace(int x,int y,int z,HexelDirection direction);
};