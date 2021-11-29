#pragma once
#include <unordered_map>
#include <standard.h>
#include "tensor.h"
#include "mesh.h"
#include "ivec.h"
#include "primitiveMesh.h"


struct BaseBlockData {
    int materialID;
    int extraMaterialID;
};

struct VoxelEngineConfiguration {
    int chunkSize;
};

/**
 * ChucnkEngine template for generating a mesh
 */

template <typename PrimitiveMeshGenerator,typename BaseBlockData>
struct ChunkEngine : public PrimitiveMeshGenerator
{
    MeshID meshId;
    dynamic_tensor<BaseBlockData> m_blockData;
    Ivec offset;

    ChunkEngine(int chunkSize,Ivec _offset) : m_blockData(chunkSize),offset(_offset) { 
        meshID = MeshLoader::loadMesh(Mesh());
    }

    inline size_t size() const { return m_blockData.size(); }

    //Critical performance optimization due to virtuals... ??
    virtual void generateBlock(int x,int y,int z);

    void update()
    {
        clear();
        for (size_t x = 0; x < size(); x++) {
            for (size_t y = 0; y < size(); y++) {
                for (size_t z = 0; z < size(); z++) {
                    generateBlock(x,y,z);
                }
            }
        }

        MeshLoader::meshes[meshId] = PrimitiveMeshGenerator::generateMesh();
    }

    void setBlock(Ivec vec,const BlockData& data) { 
        m_blockData[vec.x][vec.y][vec.z] = data;
    }
};

template <typename BlockData>
struct VoxelEngine
{
    std::unordered_map<Ivec,ChunkEngine> world;
    VoxelEngineConfiguration configuration;

    VoxelEngine(const VoxelEngineConfiguration& _configuration) : configuration(_configuration) { }
    
    Ivec getChunkSpaceCoordinate(Ivec vec) { return vec / configuration.chunkSize; }
    Ivec getLocalSpaceCoordinate(Ivec vec) { return vec % configuration.chunkSize; }


    ChunkEngine& getChunk(Ivec vec) {
        auto it = world.find(vec);
        if(it == world.end()) return world[vec] = ChunkEngine(configuration.chunkSize,vec);
        return it->second;
    }

    void setBlock(Ivec vec,const BlockData& data) { 
        auto& chunk = getChunk(getChunkSpaceCoordinate(vec));
        chunk.setBlock(getLocalSpaceCoordinate(vec),data);
    } 
};