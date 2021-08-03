#version 330 core
out vec3 color;

in vec3 pos;
uniform samplerCube uSkybox;

void main()
{    
    color = texture(uSkybox,pos).xyz;
}