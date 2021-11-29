#pragma once
#include <cstddef>
/**
 * Simle 3D integer vector
 */

struct Ivec {
    
    int x,y,z;

    Ivec(int _x,int _y,int _z) : x(_x), y(_y), z(_z) { }

    inline bool operator==(const Ivec& other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    inline Ivec operator+(const Ivec& other) const {
        return {x + other.x, y + other.y, z + other.z};
    }

    inline int operator[](size_t idx) const
    {
        switch(idx) {
            default:
            case 0: return x;
            case 1: return y;
            case 2: return z;
        }
    } 

    inline Ivec operator/(int d) {
        return {x/d,y/d,z/d};
    }

    inline Ivec operator%(int d) {
        return {x%d,y%d,z%d};
    }

};

struct IvecHash
{
    size_t operator()(const Ivec& ivec) const {
        return ivec.x + ivec.y + ivec.z;
    }
};