#version 330 core

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 transformMatrix;

layout (location = 0) in vec3 aVertex;

void main()
{
    gl_Position = (projectionMatrix * viewMatrix * transformMatrix * vec4(aVertex,1.0));
}
