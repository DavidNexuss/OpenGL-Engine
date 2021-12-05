#include "multiResPlane.h"
#include <cmath>
MultiResPlane::MultiResPlane(float _resolution) : resolution(_resolution)
{

}

void MultiResPlane::create()
{
    size_t l = resolution + 2;
    int n = l*l;
    int vertex = initiliazeAttribute(n,3,Standard::aPosition);

    for (size_t i = 0; i < l; i++)
    {
        for (size_t j = 0; j < l; j++)
        {
            attributes[vertex].buffer[i*l + j] = glm::vec3(i/float(l),0,j/float(l));        
        }
    }

    for (size_t i = 1; i < l; i++)
    {
        for (size_t j = 1; j < l; j++)
        {
            indices.push_back((i - 1)*l + (j - 1));             //A
            indices.push_back((i)*l + (j - 1));                 //B
            indices.push_back((i - 1)*l + j);                   //C

            indices.push_back((i)*l + (j - 1));             //A
            indices.push_back((i)*l + j);                       //D
            indices.push_back((i - 1)*l + j);                   //C
        }   
    }
}