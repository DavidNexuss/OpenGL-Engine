#include "glfw.h"
#include "mesh_builder.h"
#include "mesh_native.h"

namespace MeshBuilder
{
    Mesh createPrimitiveMesh(PrimitiveMeshType type,bool uv = false)
    {
        GLuint VAO,VBO;
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
    
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        int vertexCount = -1;
        const GLfloat* meshArrayPtr = nullptr;
        int vertexStride = uv ? 8 : 6;

        switch(type)
        {
            case Triangle:
            vertexCount = 3;
            meshArrayPtr = triangle_mesh;
            break;

            case Plain:
            vertexCount = 3;
            meshArrayPtr = textured_plain_mesh;
            break;

            case Cube:
            vertexCount = 36;
            meshArrayPtr = cube_mesh;
            break;

            case SkyBox:
            vertexCount = 36;
            meshArrayPtr = skyboxVertices;
        }

        Mesh mesh(meshArrayPtr,vertexCount,vertexStride);
        mesh.vao = VAO;
        mesh.vbo = VBO;
        if(type == SkyBox)
        {
            mesh.meshBuffer->bufferData();
            glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3 * sizeof(GLfloat),(void*)0);
            glEnableVertexAttribArray(0);
        }
        else
        {
            mesh.meshBuffer->generateNormals();
            mesh.meshBuffer->generateTangents();
            mesh.meshBuffer->bufferData();
            mesh.meshBuffer->bindRegions();
        }
        return mesh;
    }
}
