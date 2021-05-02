#pragma once
#include <stddef.h>

enum MeshRegionType
{
    REGION_VERTEX = 0,
    REGION_COLOR,
    REGION_UV,
    REGION_NORMAL,
    REGION_TANGENT,
    REGION_COUNT
};

struct MeshRegion
{
    size_t offset;
    size_t stride;
    size_t size;

    MeshRegion() : stride(-1) { }
    constexpr MeshRegion(size_t _offset,size_t _stride,size_t _size) : 
    offset(_offset),
    stride(_stride),
    size(_size)
    {}

    inline bool enabled() const { return stride != GL_INVALID_INDEX; }
};
