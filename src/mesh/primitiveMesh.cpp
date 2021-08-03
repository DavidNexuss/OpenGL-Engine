#include "primitiveMesh.h"
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
    glBufferData(GL_ARRAY_BUFFER,data.size(),data.data(),GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(size_t), 
                 &indices[0], GL_STATIC_DRAW);

    int ptr = 0;
    for (size_t i = 0; i < attributes.size(); i++)
    {
        glVertexAttribPointer(attributes[i].attributeid,attributes[i].size,GL_FLOAT,GL_FALSE,vertexSize * sizeof(float),(void*)(ptr * sizeof(float)));
        glEnableVertexAttribArray(attributes[i].attributeid);
        ptr += attributes[i].size;
    }

    glBindVertexArray(0);
    return Mesh(VAO,VBO,EBO,indices.size());
}