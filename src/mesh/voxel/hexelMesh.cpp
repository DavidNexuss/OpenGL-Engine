#include "hexelMesh.h"
#include <glm/ext.hpp>
#include <iostream>

void HexelMeshGenerator::create() 
{
    attributes.emplace_back(256,3,Standard::aPosition);
    attributes.emplace_back(256,3,Standard::aNormal);
    attributes.emplace_back(256,2,Standard::aUV);
}

void HexelMeshGenerator::addHex(int x,int y,int z){
    for (size_t i = 0; i < 8; i++) {
        addFace(x,y,z,HexelDirection(i));
    }
}
void HexelMeshGenerator::addFace(int i,int j,int k,HexelDirection direction)
{
    int count = attributes[0].buffer.size();
    const static float h = std::sqrt(3.0f)/2.0f;
    const static float w = 1.0f/2.0f;

    const glm::vec3 offset = glm::vec3(w*1.5,0,h*0.5) * float(i)  + glm::vec3(-w * 1.5,0,h*0.5) * float(k) + glm::vec3(0,j * 0.5,0);

    const auto addSideFace = [&](glm::vec3 offset,int side)
    {
        const auto rotationMatrix = glm::mat3(glm::rotate(glm::mat4(1.0f),side * float(M_PI) * 2.0f / 6.0f,glm::vec3(0,1,0)));
        const static std::array<glm::vec3,4> vertices = {
            glm::vec3(-w/2.0,(-w + 0.5) / 2.0,0.5 * h),
            glm::vec3(-w/2.0,( w + 0.5) / 2.0,0.5 * h),
            glm::vec3(w/2.0, ( w + 0.5) / 2.0,0.5 * h),
            glm::vec3(w/2.0, (-w + 0.5) / 2.0,0.5 * h)
        };

        for(const auto& v : vertices) {
            attributes[0].buffer.push_back(v * rotationMatrix + offset);
            attributes[1].buffer.push_back(glm::vec3{0,0,1} * rotationMatrix);
            attributes[2].buffer.push_back({v.x + 0.5,v.y + 0.5,0});
        }

        indices.push_back(count);
        indices.push_back(count + 1);
        indices.push_back(count + 2);


        indices.push_back(count + 2);
        indices.push_back(count + 3);
        indices.push_back(count);
    };

    const auto addUpFace = [&](glm::vec3 offset)
    {
        const std::array<glm::vec3,6> vertices = {
            glm::vec3{- 0.5f,   0.5f,  0.0f},
            glm::vec3{- 0.5f*w, 0.5f,  0.5f*h},
            glm::vec3{- 0.5f*w, 0.5f, -0.5f*h},
            glm::vec3{  0.5f,   0.5f,  0.0f},
            glm::vec3{  0.5f*w, 0.5f,  0.5f*h},
            glm::vec3{  0.5f*w, 0.5f, -0.5f*h}
        };

        for(const auto& v : vertices) { 
            attributes[0].buffer.push_back(v + offset);
            attributes[1].buffer.push_back({0,1,0});
            attributes[2].buffer.push_back({v.x + 0.5,v.z + 0.5,0.0});
        }

        indices.push_back(count + 1);
        indices.push_back(count);
        indices.push_back(count + 2);

        indices.push_back(count + 3);
        indices.push_back(count + 4);
        indices.push_back(count + 5);
        
        indices.push_back(count + 1);
        indices.push_back(count + 2);
        indices.push_back(count + 4);

        indices.push_back(count + 4);
        indices.push_back(count + 2);
        indices.push_back(count + 5);
    };

    switch (direction) {

    case HexelDirection::SIDE6:
    case HexelDirection::SIDE5:
    case HexelDirection::SIDE4:
    case HexelDirection::SIDE3:
    case HexelDirection::SIDE2:
    case HexelDirection::SIDE1:
        addSideFace(offset,direction - 2);
        break;
    case HexelDirection::UP:
        addUpFace(offset);
        break;
    default:
        break;
    }
}