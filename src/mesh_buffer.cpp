#include "mesh_buffer.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>

MeshBuffer::MeshBuffer(const GLfloat* raw_meshBuffer,int _vertexCount,int _vertexStride) : 
vertexCount(_vertexCount),
vertexStride(_vertexStride),
meshBuffer(_vertexCount * _vertexStride),
regions(REGION_COUNT)
{
    memcpy(&meshBuffer[0],raw_meshBuffer,vertexCount * vertexStride * sizeof(GLfloat));
}

size_t MeshBuffer::allocateRegion()
{
    size_t ptr = meshBuffer.size();
    meshBuffer.resize(meshBuffer.size() + vertexCount * 3);
    return ptr;
}

void MeshBuffer::generateNormals()
{
    size_t normalsPointer = allocateRegion();
    for (size_t i = 0,j = 0; i < vertexCount * vertexStride; i += 3 * vertexStride, j += 9)
    {
        const glm::vec3 x = *(glm::vec3 *)&meshBuffer[i];
        const glm::vec3 y = *(glm::vec3 *)&meshBuffer[i + vertexStride];
        const glm::vec3 z = *(glm::vec3 *)&meshBuffer[i + vertexStride*2];
        glm::vec3 normal = -glm::normalize(glm::cross(x - y,x - z));
        *(glm::vec3*)&meshBuffer[normalsPointer + j] = normal;
        *(glm::vec3*)&meshBuffer[normalsPointer + j + 3] = normal;
        *(glm::vec3*)&meshBuffer[normalsPointer + j + 6] = normal;
    }
    regions[REGION_NORMAL] = {normalsPointer,3,3};
}

void MeshBuffer::generateTangents()
{
    size_t tangentsPointer = allocateRegion();
    for (size_t i = 0,j = 0; i < vertexCount * vertexStride; i += 3 * vertexStride, j += 9)
    {
        const glm::vec3 v0 = *(glm::vec3 *)&meshBuffer[i];
        const glm::vec3 v1 = *(glm::vec3 *)&meshBuffer[i + vertexStride];
        const glm::vec3 v2 = *(glm::vec3 *)&meshBuffer[i + vertexStride*2];
        const glm::vec2 uv0 = *(glm::vec3 *)&meshBuffer[i + 6];
        const glm::vec2 uv1 = *(glm::vec3 *)&meshBuffer[i + vertexStride + 6];
        const glm::vec2 uv2 = *(glm::vec3 *)&meshBuffer[i + vertexStride*2 + 6];
        
        glm::vec3 deltaPos1 = v1-v0;
        glm::vec3 deltaPos2 = v2-v0;
        // UV delta
        glm::vec2 deltaUV1 = uv1-uv0;
        glm::vec2 deltaUV2 = uv2-uv0;
        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tangent = (deltaPos1 * deltaUV2.y   - deltaPos2 * deltaUV1.y)*r;
        tangent = glm::normalize(tangent);
        *(glm::vec3*)&meshBuffer[tangentsPointer + j] = tangent;
        *(glm::vec3*)&meshBuffer[tangentsPointer + j + 3] = tangent;
        *(glm::vec3*)&meshBuffer[tangentsPointer + j + 6] = tangent;
    }
    
    regions[REGION_TANGENT] = {tangentsPointer,3,3};
}


void MeshBuffer::bindRegions() const
{
    size_t rowSize = vertexStride * sizeof(GLfloat);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,rowSize,(void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,rowSize,(void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    if (vertexStride > 6)
    {
        glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,rowSize,(void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    
    for (size_t i = REGION_NORMAL; i < regions.size(); i++)
    {
        if (regions[i].enabled())
        {
            glVertexAttribPointer(i,regions[i].size,GL_FLOAT,GL_FALSE,regions[i].stride * sizeof(float),(void*)(regions[i].offset * sizeof(float)));
            glEnableVertexAttribArray(i);
        }
    }
}