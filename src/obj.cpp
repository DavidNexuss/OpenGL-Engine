#include "obj.h"
#include "mesh.h"
#include "directory.h"
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "standard.h"
using namespace std;

//Planning to deprecate....

enum AttributeEnum
{
    A_POSITION = 1 << Standard::aPosition,
    A_COLOR =    1 << Standard::aColor,
    A_UV =       1 << Standard::aUV,
    A_NORMAL =   1 << Standard::aNormal,
    A_TANGENT =  1 << Standard::aTangent,
    A_BITANGENT = 1 << Standard::aBiTangent
};

const int attributesCount = 6;
struct Vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 uv;
    glm::vec3 normal;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

const static size_t attributesSize[] = {3,3,2,3,3,3};

struct ObjectContext
{
    map<string,Texture> loadedTextures;
    map<aiMaterial*,MaterialInstanceID> loadedMaterials;
    MaterialID materialID;
};

ObjectContext ctx;

MeshID createMesh(const vector<Vertex>& vertices,const vector<unsigned int>& indices,const vector<size_t> attributes)
{
    GLuint VAO,VBO,EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
  
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);  

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), 
                 &indices[0], GL_STATIC_DRAW);

    size_t offset = 0;
    
    for (size_t i = 0; i < attributesCount; i++)
    {
        bool flag = (1 << i) & attributes[i];
        size_t size = attributesSize[i];
        if (flag)
        {
            glEnableVertexAttribArray(i);
            glVertexAttribPointer(i,size,GL_FLOAT, GL_FALSE,sizeof(Vertex),(void*)offset);
        }
        offset += size * sizeof(float);
    }

    glBindVertexArray(0);

    Mesh mesh(VAO,VBO,EBO,indices.size());
    mesh.indexed = true;
    return MeshLoader::loadMesh(mesh);
}
vector<Texture> loadMaterialTextures(aiMaterial *mat,aiTextureType type)
{
    vector<Texture> textures;
    for (size_t i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type,i,&str);

        auto it = ctx.loadedTextures.find(str.C_Str());
        if (it != ctx.loadedTextures.end()) textures.push_back(it->second);
        else
        {
            textures.push_back(ctx.loadedTextures[str.C_Str()] = TextureLoader::loadTexture(TextureData(str.C_Str())));
        }       
    }

    return textures;
}

MaterialInstanceID createMaterial(aiMesh* mesh,const aiScene* scene)
{

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    
    auto it = ctx.loadedMaterials.find(material);
    if(it != ctx.loadedMaterials.end())
    {
        return it->second;
    }

    vector<Texture> diffuseTextures = loadMaterialTextures(material,aiTextureType_DIFFUSE);
    vector<Texture> specularTextures = loadMaterialTextures(material,aiTextureType_SPECULAR);
    vector<Texture> normalTextures = loadMaterialTextures(material,aiTextureType_NORMALS);

    MaterialInstance minstance({Uniform(1.0f)});

    if(diffuseTextures.size() > 0)  minstance.setTexture(diffuseTextures[0],0);
    if(specularTextures.size() > 0) minstance.setTexture(specularTextures[0],1);
    if(normalTextures.size() > 0)   minstance.setTexture(normalTextures[0],2);

    return ctx.loadedMaterials[material] = MaterialInstanceLoader::loadMaterialInstance(minstance);
}
Model processMesh(aiMesh* mesh,const aiScene* scene)
{
    vector<Vertex> vertices(mesh->mNumVertices);
    vector<size_t> attributes(mesh->mNumVertices);
    for (size_t i = 0; i < mesh->mNumVertices; i++)
    {
        vertices[i].position = glm::vec3(
                            mesh->mVertices[i].x,
                            mesh->mVertices[i].y,
                            mesh->mVertices[i].z );
        vertices[i].normal = glm::vec3(
                            mesh->mNormals[i].x,
                            mesh->mNormals[i].y,
                            mesh->mNormals[i].z );

        attributes[i] |= A_POSITION | A_NORMAL;

        if(mesh->mColors[0])
        {
            vertices[i].color = glm::vec3(mesh->mColors[0][i].r,
                                          mesh->mColors[0][i].g,
                                          mesh->mColors[0][i].b);
            attributes[i] |= A_COLOR;
        }
        //onnly using one texture by now
        if(mesh->mTextureCoords[0])
        {
            vertices[i].uv = glm::vec2(mesh->mTextureCoords[0][i].x,
                                       mesh->mTextureCoords[0][i].y);

            vertices[i].tangent = glm::vec3(mesh->mTangents[i].x,
                                            mesh->mTangents[i].y,
                                            mesh->mTangents[i].z);

            vertices[i].bitangent = glm::vec3(mesh->mBitangents[i].x,
                                              mesh->mBitangents[i].y,
                                              mesh->mBitangents[i].z);

            attributes[i] |= A_UV | A_TANGENT | A_BITANGENT;
        }
    }

    vector<unsigned int> indices;
    for (size_t i = 0; i < mesh->mNumFaces; i++)
    {
        const aiFace& face = mesh->mFaces[i];
        for (size_t j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }

    MeshID meshid = createMesh(vertices, indices,attributes);
    MaterialInstanceID materialInstanceID = createMaterial(mesh,scene);

    Model model(meshid, ctx.materialID);
    model.materialInstanceID = materialInstanceID;
    model.cullFrontFace = true;
    return model;
}
void processNode(aiNode *node, ModelGroup& modelGroup,const aiScene *scene)
{
    for(size_t i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]]; 
        modelGroup.push_back(processMesh(mesh, scene));			
    }
    for(size_t i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i],modelGroup,scene);
    }
}

ModelGroup loadMeshFromFile(const std::string& path,MaterialID materialID,bool flipUv)
{
    ModelGroup modelGroup;
    string fullPath = Directory::objectPrefix + path;

    ctx.materialID = materialID;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(fullPath, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | (aiProcess_FlipUVs * flipUv) );
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
        return ModelGroup();
    }
    string directory = fullPath.substr(0, fullPath.find_last_of('/'));

    processNode(scene->mRootNode,modelGroup,scene);
    ctx = ObjectContext();

    return modelGroup;
}
