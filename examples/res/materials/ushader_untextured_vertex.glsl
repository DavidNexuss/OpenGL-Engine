#version 330 core

uniform mat3 uNormalMatrix;
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uTransformMatrix;

layout (location = 0) in vec3 aVertex;
layout (location = 1) in vec3 aColor;
layout (location = 3) in vec3 aNormal;

out vec3 fragPos;
out vec3 normal;
out vec3 color;

void main()
{
    gl_Position = (uProjectionMatrix * uViewMatrix * uTransformMatrix * vec4(aVertex,1.0));
    fragPos = (uTransformMatrix * vec4(aVertex,1.0)).xyz;
    normal = uNormalMatrix * aNormal;
    color = aColor;
}
