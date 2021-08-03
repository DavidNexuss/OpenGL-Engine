#include "primitiveMesh.h"

struct SphereMesh : public PrimitiveMesh {

    float radius;
    float segments;
    float rings;

    SphereMesh(float _radius,float _segments,float _rings);
    virtual void create() override;
};