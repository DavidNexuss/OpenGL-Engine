#include "primitiveMesh.h"
#include <iostream>

using namespace std;

std::vector<float> PrimitiveMesh::interleavedData(int& vertexSize) const{

    vertexSize = 0;
    for (size_t i = 0; i < attributes.size(); i++) {
        vertexSize += attributes[i].size;
    }
    
    int count = attributes[0].buffer.size();
    std::vector<float> data(vertexSize * count);

    int dataIdx = 0;
    int vertex = 0;
    while(vertex < count)
    {
        int oldid = dataIdx;
        for (size_t buffer = 0; buffer < attributes.size(); buffer++) 
        {
            for (size_t idx = 0; idx < attributes[buffer].size; idx++)
            {
                data[dataIdx++] = attributes[buffer].buffer[vertex][idx];
            }
        }
        vertex++;
    }
    assert(dataIdx == data.size());
    return data;
}
Mesh PrimitiveMesh::generateMesh()
{
    if(!generated()) create();
    GLuint VAO,VBO,EBO;

    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    int vertexSize;
    auto data = interleavedData(vertexSize);
    glBufferData(GL_ARRAY_BUFFER,data.size() * sizeof(float),data.data(),GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(Standard::meshIndex), 
                 &indices[0], GL_STATIC_DRAW);

    int ptr = 0;
    for (size_t i = 0; i < attributes.size(); i++)
    {
        glVertexAttribPointer(attributes[i].attributeid,
                              attributes[i].size,
                              GL_FLOAT,GL_FALSE,
                              vertexSize * sizeof(float),
                              (void*)(ptr * sizeof(float)));
        glEnableVertexAttribArray(attributes[i].attributeid);
        ptr += attributes[i].size;
    }

    glBindVertexArray(0);
    Mesh mesh(VAO,VBO,EBO,indices.size());
    mesh.indexed = true;
    return mesh;
}

Mesh PrimitiveMesh::generateFromBuffer(const std::vector<float>& buffer,
                        const std::vector<std::pair<int,int>>& attributes)
{
    GLuint VAO,VBO;
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1,&VBO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,buffer.size() * sizeof(float),buffer.data(),GL_STATIC_DRAW);
    
    int stride = 0;
    int vsize = 0;

    for (size_t i = 0; i < attributes.size(); i++) {

        vsize += attributes[i].second;
    }

    for (size_t i = 0; i < attributes.size(); i++) {

        std::cerr << attributes[i].first << " " << attributes[i].second << " " << vsize << " " << stride << std::endl;
        glVertexAttribPointer(attributes[i].first,attributes[i].second,GL_FLOAT,GL_FALSE,vsize * sizeof(float),(void*)(stride * sizeof(float)));
        glEnableVertexAttribArray(attributes[i].first);
        stride += attributes[i].second;
    }

    glBindVertexArray(0);

    Mesh mesh(VAO,VBO,-1,buffer.size() / vsize);
    return mesh;    
}

std::vector<float> PrimitiveMesh::interleavedData(const std::vector<MeshGenerationBuffer>& buffers)
{
    int count = buffers[0].buffer.size() / buffers[0].size;
    int vertexSize = 0;
    for (size_t i = 0; i < buffers.size(); i++) {
        vertexSize += buffers[i].size;
    }
    
    #ifdef DEBUG
        //Extra check to ensure all buffers are the same boundary
        for (size_t i = 0; i < buffers.size(); i++) {
            assert(count == (buffers[i].buffer.size() / buffers[i].size));
        }
    #endif

    std::vector<float> result(count * vertexSize);

    int dataIdx = 0;
    int vertex = 0;
    while(vertex < count)
    {
        for (size_t attr = 0; attr < buffers.size(); attr++) 
        {
            int s = buffers[attr].size;
            for (size_t idx = 0; idx < s; idx++)
            {
                result[dataIdx++] = buffers[attr].buffer[vertex * s + idx];
            }
        }
        vertex++;
    }
    assert(dataIdx == result.size());
    return result;
}
Mesh PrimitiveMesh::generateFromBuffers(const std::vector<MeshGenerationBuffer>& buffers,int generationFlags)
{
    std::vector<float> readyData = interleavedData(buffers);
    std::vector<pair<int,int>> attributes(buffers.size());
    for (size_t i = 0; i < buffers.size(); i++)
    {
        attributes[i] = {buffers[i].attributeid,buffers[i].size};
    }
    
    return generateFromBuffer(readyData,attributes);
}