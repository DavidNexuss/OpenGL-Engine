#include "sphereMesh.h"
#include <standard.h>
#include <cmath>

using namespace std;
using namespace glm;

SphereMesh::SphereMesh(float _radius, float _segments,float _rings) : 
    radius(_radius), segments(_segments), rings(_rings)
{

}

void SphereMesh::create() {

    //VBO Cretion
    int n = (rings + 1) * (segments + 1);

    attributes.emplace_back(n);
    attributes.emplace_back(n);
    attributes.emplace_back(n);
    
    attributes[0].size = 3;
    attributes[1].size = 2;
    attributes[2].size = 3;

    attributes[0].attributeid = Standard::aPosition;
    attributes[1].attributeid = Standard::aUV;
    attributes[2].attributeid = Standard::aNormal;
    
    float x,y,z;
    float nx,ny,nz;
    float invLength = 1.0f / radius;

    float xy;
    float ringStep = M_PI / rings;
    float segmentsStep = 2 * M_PI / segments;

    for (size_t i = 0; i <= rings; i++)
    {
        float ringAngle = M_PI / 2.0f - i * ringStep;
        xy = radius * cosf(ringAngle);
        z = radius * sinf(ringAngle);

        for (size_t j = 0; j <= segments; j++)
        {
            float segmentsAngle = j * segmentsStep;

            x = xy * cosf(segmentsAngle);
            y = xy * sinf(segmentsAngle);

            attributes[0].buffer[i * segments + j] = {x,y,z};

            float s = float(j)/segments;
            float t = float(i)/segmentsStep;

            attributes[1].buffer[i * segments + j] = glm::vec3(s,t,0);

            nx = x * invLength;
            ny = y * invLength;
            nz = z * invLength;

            attributes[2].buffer[i * segments + j] = glm::vec3(nx,ny,nz);
        }
    }

    //Indices creation

    int k1, k2;
    for(int i = 0; i < rings; ++i)
    {
        k1 = i * (segments + 1);     // beginning of current stack
        k2 = k1 + segments + 1;      // beginning of next stack

        for(int j = 0; j < segments; ++j, ++k1, ++k2)
        {
            // 2 triangles per sector excluding first and last stacks
            // k1 => k2 => k1+1
            if(i != 0)
            {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }

            // k1+1 => k2 => k2+1
            if(i != (segments-1))
            {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }

        }
    }
}