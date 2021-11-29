#include "primitiveMesh.h"

struct MultiResPlane : public PrimitiveMesh
{
    float resolution;
    MultiResPlane(float _resolution);

    virtual void create() override;
};