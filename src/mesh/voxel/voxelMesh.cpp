#include "voxelMesh.h"
#include <iostream>

using namespace std;


VoxelMeshWorld::VoxelMeshWorld() {

}

void VoxelMeshWorld::createPart(Ivec origin,int material,unordered_set<Ivec,IvecHash>& marched,VoxelMeshGenerator& generator)
{
    marched.insert(origin);

    const Ivec directions[] = {
        Ivec{1,0,0},
        Ivec{-1,0,0},
        Ivec{0,1,0},
        Ivec{0,-1,0},
        Ivec{0,0,1},
        Ivec{0,0,-1},
    };

    const VoxelMeshGenerator::VoxelDirection dirs[] = 
    {
        VoxelMeshGenerator::RIGHT,
        VoxelMeshGenerator::LEFT,
        VoxelMeshGenerator::UP,
        VoxelMeshGenerator::DOWN,
        VoxelMeshGenerator::BACK,
        VoxelMeshGenerator::FRONT,
    };

    for(int i = 0; i < 6; i++)
    {
        Ivec other = origin + directions[i];
        auto v = world.find(other);

        if(v == world.end() || v->second == 0) {
            generator.addFace(origin.x,origin.y,origin.z,dirs[i]);
        }

        else if(v->second == material && marched.count(other) == 0) {
            marched.insert(other);
            createPart(other,material,marched,generator);
        }
    }
}

std::vector<VoxelMesh> VoxelMeshWorld::rasterize()
{
    vector<VoxelMesh> voxelMeshes;
    unordered_set<Ivec,IvecHash> marched;
    
    for(const auto& v : world) {
        if(marched.count(v.first) == 0 && v.second != 0) {
            VoxelMeshGenerator generator;
            generator.create();
            createPart(v.first,v.second,marched,generator);
            voxelMeshes.push_back({v.second,generator.generateMesh()});
        }
    }
    return voxelMeshes;
}
void VoxelMeshGenerator::create()
{
    attributes.emplace_back();
    attributes.emplace_back();
    attributes.emplace_back();

    attributes[0].attributeid = Standard::aPosition;
    attributes[1].attributeid = Standard::aNormal;
    attributes[2].attributeid = Standard::aUV;
    
    attributes[0].size = 3;
    attributes[1].size = 3;
    attributes[2].size = 2;

    attributes[0].buffer.reserve(256);
    attributes[1].buffer.reserve(256);
    attributes[2].buffer.reserve(256);
}

void VoxelMeshGenerator::addFace(int i,int j,int z,VoxelDirection direction)
{
    int count = attributes[0].buffer.size();
    switch(direction)
    {
        case VoxelDirection::UP:

            attributes[0].buffer.push_back({i - 0.5f,j + 0.5f,z + 0.5f});
            attributes[0].buffer.push_back({i + 0.5f,j + 0.5f,z + 0.5f});
            attributes[0].buffer.push_back({i - 0.5f,j + 0.5f,z -0.5f});
            attributes[0].buffer.push_back({i + 0.5f,j + 0.5f,z -0.5f});

            attributes[1].buffer.push_back({0,1,0});
            attributes[1].buffer.push_back({0,1,0});
            attributes[1].buffer.push_back({0,1,0});
            attributes[1].buffer.push_back({0,1,0});

            attributes[2].buffer.push_back({1.0,1.0,0.0});
            attributes[2].buffer.push_back({1.0,0.0,0.0});
            attributes[2].buffer.push_back({0.0,1.0,0.0});
            attributes[2].buffer.push_back({0.0,0.0,0.0});

            indices.push_back(count + 1);
            indices.push_back(count);
            indices.push_back(count + 3);

            indices.push_back(count);
            indices.push_back(count + 2);
            indices.push_back(count + 3);

            break;
        
        case VoxelDirection::DOWN:
            
            attributes[0].buffer.push_back({i - 0.5f,j - 0.5f,z + 0.5f});
            attributes[0].buffer.push_back({i + 0.5f,j - 0.5f,z + 0.5f});
            attributes[0].buffer.push_back({i - 0.5f,j - 0.5f,z -0.5f});
            attributes[0].buffer.push_back({i + 0.5f,j - 0.5f,z -0.5f});

            attributes[1].buffer.push_back({0,-1,0});
            attributes[1].buffer.push_back({0,-1,0});
            attributes[1].buffer.push_back({0,-1,0});
            attributes[1].buffer.push_back({0,-1,0});

            attributes[2].buffer.push_back({1.0,1.0,0.0});
            attributes[2].buffer.push_back({1.0,0.0,0.0});
            attributes[2].buffer.push_back({0.0,1.0,0.0});
            attributes[2].buffer.push_back({0.0,0.0,0.0});

            indices.push_back(count);
            indices.push_back(count + 1);
            indices.push_back(count + 3);

            indices.push_back(count + 2);
            indices.push_back(count);
            indices.push_back(count + 3);

            break;
        
        case VoxelDirection::LEFT:

            attributes[0].buffer.push_back({i - 0.5f,j - 0.5f,z + 0.5f});
            attributes[0].buffer.push_back({i - 0.5f,j + 0.5f,z + 0.5f});
            attributes[0].buffer.push_back({i - 0.5f,j - 0.5f,z -0.5f});
            attributes[0].buffer.push_back({i - 0.5f,j + 0.5f,z -0.5f});

            attributes[1].buffer.push_back({-1,0,0});
            attributes[1].buffer.push_back({-1,0,0});
            attributes[1].buffer.push_back({-1,0,0});
            attributes[1].buffer.push_back({-1,0,0});

            attributes[2].buffer.push_back({1.0,1.0,0.0});
            attributes[2].buffer.push_back({1.0,0.0,0.0});
            attributes[2].buffer.push_back({0.0,1.0,0.0});
            attributes[2].buffer.push_back({0.0,0.0,0.0});

            indices.push_back(count + 1);
            indices.push_back(count);
            indices.push_back(count + 3);

            indices.push_back(count);
            indices.push_back(count + 2);
            indices.push_back(count + 3);

            break;

        case VoxelDirection::RIGHT:

            attributes[0].buffer.push_back({i + 0.5f,j - 0.5f,z + 0.5f});
            attributes[0].buffer.push_back({i + 0.5f,j + 0.5f,z + 0.5f});
            attributes[0].buffer.push_back({i + 0.5f,j - 0.5f,z -0.5f});
            attributes[0].buffer.push_back({i + 0.5f,j + 0.5f,z -0.5f});

            attributes[1].buffer.push_back({1,0,0});
            attributes[1].buffer.push_back({1,0,0});
            attributes[1].buffer.push_back({1,0,0});
            attributes[1].buffer.push_back({1,0,0});

            attributes[2].buffer.push_back({1.0,1.0,0.0});
            attributes[2].buffer.push_back({1.0,0.0,0.0});
            attributes[2].buffer.push_back({0.0,1.0,0.0});
            attributes[2].buffer.push_back({0.0,0.0,0.0});

            indices.push_back(count);
            indices.push_back(count + 1);
            indices.push_back(count + 3);

            indices.push_back(count + 2);
            indices.push_back(count);
            indices.push_back(count + 3);

            break;

        case VoxelDirection::FRONT:
            
            attributes[0].buffer.push_back({i + 0.5f,j - 0.5f,z + 0.5f});
            attributes[0].buffer.push_back({i + 0.5f,j + 0.5f,z + 0.5f});
            attributes[0].buffer.push_back({i - 0.5f,j - 0.5f,z + 0.5f});
            attributes[0].buffer.push_back({i - 0.5f,j + 0.5f,z + 0.5f});

            attributes[1].buffer.push_back({0,0,1});
            attributes[1].buffer.push_back({0,0,1});
            attributes[1].buffer.push_back({0,0,1});
            attributes[1].buffer.push_back({0,0,1});

            attributes[2].buffer.push_back({1.0,1.0,0.0});
            attributes[2].buffer.push_back({1.0,0.0,0.0});
            attributes[2].buffer.push_back({0.0,1.0,0.0});
            attributes[2].buffer.push_back({0.0,0.0,0.0});

            indices.push_back(count + 1);
            indices.push_back(count);
            indices.push_back(count + 3);

            indices.push_back(count);
            indices.push_back(count + 2);
            indices.push_back(count + 3);
            break;

        case VoxelDirection::BACK:
            
            attributes[0].buffer.push_back({i + 0.5f,j - 0.5f,z -0.5f});
            attributes[0].buffer.push_back({i + 0.5f,j + 0.5f,z -0.5f});
            attributes[0].buffer.push_back({i - 0.5f,j - 0.5f,z -0.5f});
            attributes[0].buffer.push_back({i - 0.5f,j + 0.5f,z -0.5f});

            attributes[1].buffer.push_back({0,0,-1});
            attributes[1].buffer.push_back({0,0,-1});
            attributes[1].buffer.push_back({0,0,-1});
            attributes[1].buffer.push_back({0,0,-1});

            attributes[2].buffer.push_back({1.0,1.0,0.0});
            attributes[2].buffer.push_back({1.0,0.0,0.0});
            attributes[2].buffer.push_back({0.0,1.0,0.0});
            attributes[2].buffer.push_back({0.0,0.0,0.0});

            indices.push_back(count);
            indices.push_back(count + 1);
            indices.push_back(count + 3);

            indices.push_back(count + 2);
            indices.push_back(count);
            indices.push_back(count + 3);
            break;

    }
}